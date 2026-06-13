#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>
#include <thread>
#include <vector>

#include "router.h"

namespace http {

namespace asio = boost::asio;
namespace beast = boost::beast;

using asio::ip::tcp;

class Server
{
public:
    static Server &instance();

    Server &miltithreaded(bool isMultithreaded);
    Server &port(unsigned short port);
    void start();
    void stop();

private:
    Server();

    asio::io_context m_ioc;
    asio::executor_work_guard<asio::io_context::executor_type> m_ioGuard;
    tcp::acceptor m_acceptor;
    asio::signal_set m_signalsSet;

    bool m_isMutithreaded{true};
    unsigned short m_port;
    std::vector<std::thread> m_threads;
    void setupAcceptor();
    asio::awaitable<void> asyncAccept();
    void waitSignals();

    std::shared_ptr<Router> m_router;
};

} // namespace httt

#endif // SERVER_H
