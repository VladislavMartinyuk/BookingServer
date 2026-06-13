#include "router.h"

#include "shared.h"

#include <iostream>

using namespace http;

Router::Router()
    : m_head(std::make_unique<Node>())
{}

void Router::addRoute(const std::string &method, const std::string &uri, HandleMethod &&handler)
{
    auto splittedUri = Utils::splitString(uri, '/');
    splittedUri.push_back(method);
    auto current = m_head.get();
    for (const auto &field : splittedUri) {
        if (!isDynamicUriField(field)) {
            current = createStaticUriNode(field, current);
        } else {
            current = createDynamicUriNode(field, current);
        }
    }
    if (current) {
        current->handler = std::move(handler);
    }
}

AwaitableStringResponse Router::processRequest(const StringRequest &request)
{
    std::string uri = request.target();
    std::string method = request.method_string();

    URIParams params;
    params.queryValue = createUriQueryParams(uri);

    if (!params.queryValue.empty()) {
        auto splittedWithQuery = Utils::splitString(uri, '?');
        uri = splittedWithQuery.at(0);
    }

    auto splittedUri = Utils::splitString(uri, '/');
    splittedUri.push_back(method);

    auto current = m_head.get();
    for (const auto &field : splittedUri) {
        if (current->staticHandlers.contains(field)) {
            current = current->staticHandlers[field].get();
        } else if (current->dynamicChild) {
            current = current->dynamicChild.get();
            params.routeValue[current->dynamicFieldName] = field;
        }
    }

    if (current && current->handler) {
        co_return co_await current->handler(std::move(request), std::move(params));
    }

    co_return StringResponse{};
}

bool Router::isDynamicUriField(std::string_view field)
{
    return field.find('{') != std::string_view::npos && field.find('}') != std::string_view::npos;
}

Node *Router::createStaticUriNode(const std::string &field, Node *current)
{
    if (!current) {
        return nullptr;
    }

    if (!current->staticHandlers.contains(field)) {
        auto newNode = std::make_unique<Node>();
        current->staticHandlers[field] = std::move(newNode);
        return current->staticHandlers[field].get();
    }

    return current->staticHandlers[field].get();
}

Node *Router::createDynamicUriNode(const std::string &field, Node *current)
{
    if (!current) {
        return nullptr;
    }

    auto fieldCopy = field;
    fieldCopy.erase(fieldCopy.begin());
    fieldCopy.erase(fieldCopy.end() - 1);

    auto node = std::make_unique<Node>();
    node->dynamicFieldName = fieldCopy;
    current->dynamicChild = std::move(node);

    return current->dynamicChild.get();
}

std::unordered_map<std::string, VariantValue> Router::createUriQueryParams(const std::string &uri)
{
    std::unordered_map<std::string, VariantValue> result;

    auto idx = uri.find('?');
    if (idx == std::string::npos) {
        return result;
    }

    auto needSubstr = uri.substr(idx + 1);
    auto splittedParams = Utils::splitString(needSubstr, '&');
    for (const auto &param : splittedParams) {
        auto fieldWithValue = Utils::splitString(param, '=');
        if (fieldWithValue.size() == 2) {
            result[fieldWithValue.at(0)] = fieldWithValue.at(1);
        }
    }

    return result;
}
