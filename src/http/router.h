#ifndef ROUTER_H
#define ROUTER_H

#include <functional>
#include <memory>
#include <unordered_map>

#include "shared.h"
#include "variantvalue.h"

namespace http {

struct URIParams;
struct Node;

using HandleMethod = std::function<AwaitableStringResponse(StringRequest, URIParams)>;

class Router
{
public:
    Router();

    void addRoute(const std::string &method, const std::string &uri, HandleMethod &&handler);
    AwaitableStringResponse processRequest(const StringRequest &request);

private:
    std::unique_ptr<Node> m_head;

    bool isDynamicUriField(std::string_view field);
    Node *createStaticUriNode(const std::string &field, Node *current);
    Node *createDynamicUriNode(const std::string &field, Node *current);
    std::unordered_map<std::string, VariantValue> createUriQueryParams(const std::string &uri);
};

struct Node
{
    std::unordered_map<std::string, std::unique_ptr<Node>> staticHandlers;
    std::unique_ptr<Node> dynamicChild;
    HandleMethod handler;
    std::string dynamicFieldName;
};

struct URIParams
{
    std::unordered_map<std::string, VariantValue> routeValue;
    std::unordered_map<std::string, VariantValue> queryValue;
};

} // namespace http

#endif // ROUTER_H
