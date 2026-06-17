#include "apimanager.h"

#include "http/api/v1/usershandler.h"

using namespace http;

ApiManager::ApiManager()
{
    m_v1Handlers.push_back(std::make_unique<v1::UsersHandler>());
}

void ApiManager::registerRoutes() const
{
    for (const auto &handler : m_v1Handlers) {
        handler->registerRoutes();
    }
}
