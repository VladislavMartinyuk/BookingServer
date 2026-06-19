#include "appenv.h"

#include <boost/algorithm/string.hpp>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

#include <boost/asio.hpp>

boost::asio::awaitable<void> a();

AppEnv::AppEnv()
{
    readLocalEnvFile();

    if (auto envDBThreadsCount = std::getenv("DB_CONNECTIONS_COUNT")) {
        m_dbConnectionsCount = std::stoi(envDBThreadsCount);
    }

    m_mariaDBConnParams.dbName = std::getenv("MARIADB_DATABASE") ? std::getenv("MARIADB_DATABASE") : "";
    m_mariaDBConnParams.pass = std::getenv("MARIADB_ROOT_PASSWORD") ? std::getenv("MARIADB_ROOT_PASSWORD")
                                                             : "";
    m_mariaDBConnParams.host = std::getenv("MARIADB_DOCKER_HOST") ? std::getenv("MARIADB_DOCKER_HOST")
                                                           : "";
    m_mariaDBConnParams.port = std::getenv("MARIADB_PORT") ? std::getenv("MARIADB_PORT") : "";
    m_mariaDBConnParams.user = std::getenv("MARIADB_USER") ? std::getenv("MARIADB_USER") : "";
}

void AppEnv::readLocalEnvFile()
{
    std::ifstream file(".env.example");
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

DBConnectionParametrs AppEnv::getMariaDBConnectionParametrs() const
{
    return m_mariaDBConnParams;
}

int AppEnv::dbConnectionsCount() const
{
    return m_dbConnectionsCount;
}