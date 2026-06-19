#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <boost/asio.hpp>
#include <memory>

#include "mariadbconnection.h"
#include "dbconnectionpool.h"
#include "appenv.h"

namespace db {

namespace asio = boost::asio;

class DBManager 
{
public:
    DBManager(const DBManager &) = delete;
    DBManager &operator=(const DBManager &) = delete;

    static DBManager &instance();
    void setAppEnv(const std::shared_ptr<AppEnv> &env);
    void createPools();

    asio::awaitable<mysql::results> mariaDBQuery(std::string_view query);
    asio::awaitable<mysql::execution_state> mariaDBExecute(std::string_view query);

private:
    DBManager() = default;
    std::shared_ptr<AppEnv> m_env;
    std::unique_ptr<asio::thread_pool> m_threadPool;
    DBConnectionPool<MariaDBConnection> m_mariaDBConnection;
};

} // namespace db

#endif