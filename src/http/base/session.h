#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>

#include "router.h"

namespace http {

namespace asio = boost::asio;
namespace beast = boost::beast;

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(const std::shared_ptr<Router> &router, asio::ip::tcp::socket socket);

    void start();

private:
    asio::strand<asio::any_io_executor> m_strand;
    std::shared_ptr<Router> m_router;
    beast::tcp_stream m_stream;
    asio::awaitable<void> loop();
};

} // namespace httt

#endif // SESSION_H
