#include "appenv.h"
#include "http/base/server.h"

#include <memory>

int main()
{
    auto env = std::make_shared<AppEnv>();

    http::Server::instance().miltithreaded(true).port(8080).start();

    return 0;
}