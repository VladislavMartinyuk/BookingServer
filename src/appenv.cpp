#include "appenv.h"

#include <boost/algorithm/string.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

AppEnv::AppEnv()
{
    readLocalEnvFile();

    if (auto envDBThreadsCount = std::getenv("DB_CONNECTIONS_COUNT")) {
        m_dbConnectionsCount = std::stoi(envDBThreadsCount);
        m_dbThreadPool = std::make_shared<asio::thread_pool>(m_dbConnectionsCount);
    }

    dbConnParams.dbName = std::getenv("MARIADB_DATABASE") ? std::getenv("MARIADB_DATABASE") : "";
    dbConnParams.pass = std::getenv("MARIADB_ROOT_PASSWORD") ? std::getenv("MARIADB_ROOT_PASSWORD")
                                                             : "";
    dbConnParams.host = std::getenv("MARIADB_DOCKER_HOST") ? std::getenv("MARIADB_DOCKER_HOST")
                                                           : "";
    dbConnParams.port = std::getenv("MARIADB_PORT") ? std::getenv("MARIADB_PORT") : "";
    dbConnParams.user = std::getenv("MARIADB_USER") ? std::getenv("MARIADB_USER") : "";

    std::cout << dbConnParams.user << " " << dbConnParams.dbName << " " << dbConnParams.host
              << dbConnParams.pass << dbConnParams.port << std::endl;
}

void AppEnv::readLocalEnvFile()
{
    std::ifstream file(".env");
    if (!file.is_open()) {
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        boost::trim(line);
        if (line.empty() || line[0] == '#')
            continue;

        std::vector<std::string> tokens;
        size_t sep = line.find('=');
        if (sep != std::string::npos) {
            std::string key = boost::trim_copy(line.substr(0, sep));
            std::string value = boost::trim_copy(line.substr(sep + 1));

#ifdef _WIN32
            _putenv_s(key.c_str(), value.c_str());
#else
            setenv(key.c_str(), value.c_str(), 1);
#endif
        }
    }
}

void AppEnv::tryCreateDBPool(const DBConnectionParametrs &connParams)
{
    if (!m_dbThreadPool) {
        return;
    }

    mysql::pool_params params;
    params.username = connParams.user;
    params.password = connParams.pass;
    params.database = connParams.dbName;
    params.max_size = m_dbConnectionsCount;

    asio::io_context temp_ioc;
    asio::ip::tcp::resolver resolver(temp_ioc);
    boost::system::error_code ec;

    std::string_view final_host = connParams.host;
    std::string_view final_port = connParams.port;

    resolver.resolve(final_host, final_port, ec);

    if (ec) {
        final_host = "127.0.0.1";
    }

    params.server_address.emplace_host_and_port(std::string(final_host),
                                                std::stoi(std::string(final_port)));

    std::cout << "[DB Init] Connection pool created for host: " << final_host << std::endl;
    m_dbPool = std::make_shared<mysql::connection_pool>(*m_dbThreadPool, std::move(params));

    m_dbPool->async_run(asio::detached);
}
