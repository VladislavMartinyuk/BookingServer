#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>
#include <thread>
#include <vector>

#include "http/api/apimanager.h"
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
    std::shared_ptr<Router> router() const;

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
    ApiManager m_apiManager;
};

#define GET_ROUTE(Route, Handler) \
    Server::instance().router()->addRoute("GET", \
                                          Route, \
                                          [this](StringRequest req, \
                                                 URIParams par) -> AwaitableStringResponse { \
                                              co_return co_await this->Handler(std::move(req), \
                                                                               std::move(par)); \
                                          });

#define POST_ROUTE(Route, Handler) \
    Server::instance().router()->addRoute("POST", \
                                          Route, \
                                          [this](StringRequest req, \
                                                 URIParams par) -> AwaitableStringResponse { \
                                              co_return co_await this->Handler(std::move(req), \
                                                                               std::move(par)); \
                                          });

#define PATCH_ROUTE(Route, Handler) \
    Server::instance().router()->addRoute("PATCH", \
                                          Route, \
                                          [this](StringRequest req, \
                                                 URIParams par) -> AwaitableStringResponse { \
                                              co_return co_await this->Handler(std::move(req), \
                                                                               std::move(par)); \
                                          });

#define DELETE_ROUTE(Route, Handler) \
    Server::instance().router()->addRoute("DELETE", \
                                          Route, \
                                          [this](StringRequest req, \
                                                 URIParams par) -> AwaitableStringResponse { \
                                              co_return co_await this->Handler(std::move(req), \
                                                                               std::move(par)); \
                                          });

} // namespace http

#endif // SERVER_H
