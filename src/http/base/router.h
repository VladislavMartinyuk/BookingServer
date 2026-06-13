#ifndef ROUTER_H
#define ROUTER_H

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#include "http/shared/shared.h"
#include "http/shared/variantvalue.h"
#include "middleware/irequestmiddleware.h"

namespace http {

struct Node;

using HandleMethod = std::function<AwaitableStringResponse(StringRequest, URIParams)>;

class Router
{
public:
    Router();

    void addRoute(const std::string &method, const std::string &uri, HandleMethod &&handler);
    AwaitableStringResponse processRequest(const StringRequest &request);
    void registerLocalMiddleware(const std::string &method,
                                 const std::string &uri,
                                 std::unique_ptr<IRequestMiddleware> middleware);

private:
    std::unique_ptr<Node> m_head;
    std::vector<std::unique_ptr<IRequestMiddleware>> m_globalRequestMiddleware;

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
    std::vector<std::unique_ptr<IRequestMiddleware>> localMiddlewares;
};

} // namespace http

#endif // ROUTER_H
