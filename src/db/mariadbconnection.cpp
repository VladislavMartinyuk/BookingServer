#include "mariadbconnection.h"

#include <spdlog/spdlog.h>

using namespace db;

MariaDBConnection::MariaDBConnection()
    : m_sslContext(asio::ssl::context::tls_client)
{}

asio::awaitable<bool> MariaDBConnection::asyncConnect()
{
    try {
        auto exector = co_await asio::this_coro::executor;
        m_strand = std::make_unique<asio::strand<asio::any_io_executor>>(asio::make_strand(exector));
        m_conn = std::make_unique<mysql::tcp_ssl_connection>(*m_strand, m_sslContext);

        if (!m_env) {
            co_return false;
        }

        auto mariaDBConnParams = m_env->getMariaDBConnectionParametrs();
        mysql::handshake_params params(mariaDBConnParams.user,
                                       mariaDBConnParams.pass,
                                       mariaDBConnParams.dbName);
        asio::ip::tcp::resolver resolver(*m_strand);
        auto endPoints = co_await resolver.async_resolve(mariaDBConnParams.host, mariaDBConnParams.port, asio::use_awaitable);
        co_await m_conn->async_connect(*endPoints.begin(), params, asio::use_awaitable);         
        co_return true;                      
    } catch (const boost::system::system_error& err) {
        spdlog::warn("Failed connection to MariaDB: {}", err.what());
        co_return false;
    } catch (const std::exception& e) {
        spdlog::warn("Unknow exeption when connect to MariaDB: {}", e.what());
        co_return false;
    }
}

asio::awaitable<bool> MariaDBConnection::asyncClose()
{
    if (!m_conn) {
        co_return false;
    }

    try {
        co_await m_conn->async_close(asio::use_awaitable);
    } catch (const boost::system::system_error& err) {
        spdlog::warn("Failed close MariaDB connection: {}", err.what());
        co_return false;
    } catch (const std::exception& e) {
        spdlog::warn("Unknow exception when close MariaDB connection: {}", e.what());
        co_return false;
    }

    co_return false;
}

asio::awaitable<mysql::results> MariaDBConnection::query(std::string_view query)
{
    mysql::results result;
    co_await m_conn->async_execute(query, result, asio::use_awaitable);
    co_return result; 
}

asio::awaitable<mysql::execution_state> MariaDBConnection::execute(std::string_view query)
{
    mysql::execution_state result;
    co_await m_conn->async_start_execution(query, result, asio::use_awaitable);
    co_return result;
}