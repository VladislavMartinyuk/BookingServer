#ifndef APIMANAGER_H
#define APIMANAGER_H

#include <memory>
#include <vector>

#include "http/api/irequesthandler.h"

namespace http {

class ApiManager
{
public:
    ApiManager();

    void registerRoutes();

private:
    std::vector<std::unique_ptr<IRequestHandler>> m_v1Handlers;
};

} // namespace http

#endif // APIMANAGER_H
