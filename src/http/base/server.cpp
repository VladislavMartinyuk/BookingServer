#include "server.h"
#include <iostream>

#include "session.h"

using namespace http;

Server &Server::instance()
{
    static Server inst;
    return inst;
}

Server &Server::miltithreaded(bool isMultithreaded)
{
    m_isMutithreaded = isMultithreaded;
    return *this;
}

Server &Server::port(unsigned short port)
{
    m_port = port;
    return *this;
}

void Server::start()
{
    m_apiManager.registerRoutes();

    setupAcceptor();
    waitSignals();

    asio::co_spawn(m_ioc, asyncAccept(), asio::detached);

    auto kThreads = m_isMutithreaded ? std::thread::hardware_concurrency() : 1;
    if (kThreads == 0) {
        kThreads = 2;
    }

    m_threads.reserve(kThreads);
    for (int i = 0; i < kThreads; i++) {
        m_threads.emplace_back([this]() { m_ioc.run(); });
    }

    for (auto &t : m_threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

void Server::stop()
{
    m_acceptor.close();
    m_ioGuard.reset();
    m_signalsSet.clear();
    m_ioc.stop();
}

std::shared_ptr<Router> Server::router() const
{
    return m_router;
}

Server::Server()
    : m_ioGuard(asio::make_work_guard(m_ioc))
    , m_acceptor(m_ioc)
    , m_signalsSet(m_ioc, SIGINT, SIGTERM)
    , m_router(std::make_shared<Router>())
{}

void Server::setupAcceptor()
{
    tcp::endpoint endpoint(tcp::v4(), m_port);
    m_acceptor.open(endpoint.protocol());
    m_acceptor.set_option(tcp::acceptor::reuse_address(true));
    m_acceptor.bind(endpoint);
    m_acceptor.listen();
}

asio::awaitable<void> Server::asyncAccept()
{
    try {
        for (;;) {
            tcp::socket socket = co_await m_acceptor.async_accept(asio::use_awaitable);
            auto session = std::make_shared<Session>(m_router, std::move(socket));
            session->start();
        }
    } catch (const std::exception &e) {
        std::cout << "Server exception: " << e.what();
    }
}

void Server::waitSignals()
{
    m_signalsSet.async_wait([this](boost::system::error_code ec, int signal_number) {
        if (!ec) {
            std::cout << "\nПолучен сигнал остановки (" << signal_number
                      << "). Начинаем Graceful Shutdown..." << std::endl;
            stop();
        }
    });
}
