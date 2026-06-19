#ifndef MARIADBCONNECTION_H
#define MARIADBCONNECTION_H

#include <boost/mysql.hpp>
#include <memory>

#include "idbconnection.h"

namespace db {

namespace mysql = boost::mysql;

class MariaDBConnection : public IDBConnection
{
public:
    MariaDBConnection();

    asio::awaitable<bool> asyncConnect() override;
    asio::awaitable<bool> asyncClose() override;

    asio::awaitable<mysql::results> query(std::string_view query);
    asio::awaitable<mysql::execution_state> execute(std::string_view executeQuery);

private:
    std::unique_ptr<asio::strand<asio::any_io_executor>> m_strand;
    std::unique_ptr<mysql::tcp_ssl_connection> m_conn;
    asio::ssl::context m_sslContext;
};

} // namespace db

#endif