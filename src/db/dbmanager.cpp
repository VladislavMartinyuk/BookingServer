#include "dbmanager.h"

using namespace db;

DBManager &DBManager::instance()
{
    static DBManager ins;
    return ins;
}

void DBManager::setAppEnv(const std::shared_ptr<AppEnv> &env)
{
    m_env = env;
    if (!m_env) {
        return;
    }
    m_threadPool = std::make_unique<asio::thread_pool>(env->dbConnectionsCount());
}

void DBManager::createPools()
{
    auto f = asio::co_spawn(
        *m_threadPool,
        [this]() -> asio::awaitable<void> {
            co_await m_mariaDBConnection.createPool(m_env->dbConnectionsCount(), m_env);
        },
        asio::use_future);
    f.wait();
}

asio::awaitable<mysql::results> DBManager::mariaDBQuery(std::string_view query)
{
    auto conn = m_mariaDBConnection.getConnection();
    mysql::results result = co_await conn->query(query);
    m_mariaDBConnection.putConnection(std::move(conn));
    co_return result;
}

asio::awaitable<mysql::execution_state> DBManager::mariaDBExecute(std::string_view query)
{
    auto conn = m_mariaDBConnection.getConnection();
    mysql::execution_state result = co_await conn->execute(query);
    m_mariaDBConnection.putConnection(std::move(conn));
    co_return result;
}
