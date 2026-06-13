#include "usershandler.h"

#include "http/base/server.h"

using namespace http::v1;

UsersHandler::UsersHandler() {}

void UsersHandler::registerRoutes()
{
    GET_ROUTE("/api/v1/users", allUsers);
}

http::AwaitableStringResponse UsersHandler::allUsers(StringRequest req, URIParams params)
{
    StringResponse response;
    response.body() = "Hello world";
    response.prepare_payload();

    co_return response;
}
