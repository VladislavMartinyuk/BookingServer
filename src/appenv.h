#ifndef APPENV_H
#define APPENV_H

#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include <boost/process.hpp>
#include <memory>

namespace mysql = boost::mysql;
namespace asio = boost::asio;

class AppEnv
{
public:
    AppEnv();

private:
    int m_dbConnectionsCount;
    int m_httpIoThreadCounts;
    int m_cpuThreadCounts;
    std::shared_ptr<mysql::connection_pool> m_dbPool;
    std::shared_ptr<asio::thread_pool> m_dbThreadPool;
    std::shared_ptr<asio::thread_pool> m_cpuThreadPool;

    struct DBConnectionParametrs
    {
        std::string_view dbName;
        std::string_view pass;
        std::string_view host;
        std::string_view port;
        std::string_view user;
    } dbConnParams;

    void readLocalEnvFile();
    void tryCreateDBPool(const DBConnectionParametrs &connParams);
};

#endif // APPENV_H
