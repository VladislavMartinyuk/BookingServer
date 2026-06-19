#ifndef IDBCONNECTION_H
#define IDBCONNECTION_H

#include <boost/asio.hpp>
#include <memory>

#include "appenv.h"

namespace db {

namespace asio = boost::asio;

class IDBConnection
{
public:
    IDBConnection() = default;
    virtual ~IDBConnection() = default;

    void setAppEnv(const std::shared_ptr<AppEnv>& env) { m_env = env; }
    virtual asio::awaitable<bool> asyncConnect() = 0;
    virtual asio::awaitable<bool> asyncClose() = 0;

protected:
    std::shared_ptr<AppEnv> m_env;
};

} // namespace db

#endif