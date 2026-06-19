#ifndef APPENV_H
#define APPENV_H

#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include <boost/process.hpp>

namespace mysql = boost::mysql;
namespace asio = boost::asio;

struct DBConnectionParametrs
{
    std::string dbName;
    std::string pass;
    std::string host;
    std::string port;
    std::string user;
};

class AppEnv
{
public:
    AppEnv();

    DBConnectionParametrs getMariaDBConnectionParametrs() const;
    int dbConnectionsCount() const;

private:
    int m_dbConnectionsCount;
    int m_httpIoThreadCounts;

    DBConnectionParametrs m_mariaDBConnParams;

    void readLocalEnvFile();
};

#endif // APPENV_H
