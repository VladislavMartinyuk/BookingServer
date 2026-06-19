#include <memory>

#include "appenv.h"
#include "db/dbmanager.h"
#include "http/base/server.h"

int main()
{
    auto env = std::make_shared<AppEnv>();

    db::DBManager::instance().setAppEnv(env);
    db::DBManager::instance().createPools();

    http::Server::instance().miltithreaded(true).port(8080).start();

    return 0;
}