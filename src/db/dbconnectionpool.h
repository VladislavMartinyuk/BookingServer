#ifndef DBCONNECTIONPOOL_H
#define DBCONNECTIONPOOL_H

#include <boost/asio.hpp>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

#include "appenv.h"

namespace db {

namespace asio = boost::asio;

template<typename T>
class DBConnectionPool
{
public:
    DBConnectionPool() = default;
    ~DBConnectionPool() = default;

    DBConnectionPool(const DBConnectionPool &) = delete;
    DBConnectionPool &operator=(const DBConnectionPool &) = delete;

    asio::awaitable<bool> createPool(int connectionsCount, const std::shared_ptr<AppEnv> &env)
    {
        for (int i = 0; i < connectionsCount; i++) {
            auto conn = std::make_unique<T>();
            conn->setAppEnv(env);
            bool isConnected = co_await conn->asyncConnect();
            if (!isConnected) {
                co_return false;
            }
            {
                std::unique_lock lock(m_mutex);
                m_connections.push(std::move(conn));
            }
            m_cv.notify_one();
        }

        co_return true;
    }

    std::unique_ptr<T> getConnection()
    {
        std::unique_lock lock(m_mutex);

        m_cv.wait(lock, [this]() { return !m_connections.empty(); });

        auto conn = std::move(m_connections.front());
        m_connections.pop();
        return conn;
    }

    void putConnection(std::unique_ptr<T> conn)
    {
        {
            std::unique_lock lock(m_mutex);
            m_connections.push(std::move(conn));
        }
        m_cv.notify_one();
    }

private:
    std::queue<std::unique_ptr<T>> m_connections;
    std::condition_variable m_cv;
    std::mutex m_mutex;
};

} // namespace db

#endif