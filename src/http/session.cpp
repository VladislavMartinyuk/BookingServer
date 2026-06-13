#include "session.h"

using namespace http;

Session::Session(const std::shared_ptr<Router> &router, asio::ip::tcp::socket socket)
    : m_strand(asio::make_strand(socket.get_executor()))
    , m_router(router)
    , m_stream(std::move(socket))
{}

void Session::start()
{
    auto self = shared_from_this();
    asio::co_spawn(
        m_strand, [self]() -> asio::awaitable<void> { co_await self->loop(); }, asio::detached);
}

asio::awaitable<void> Session::loop()
{
    try {
        beast::flat_buffer buffer;
        for (;;) {
            m_stream.expires_after(std::chrono::seconds(30));
            beast::http::request<beast::http::string_body> request;
            co_await beast::http::async_read(m_stream, buffer, request);

            beast::http::response<beast::http::string_body> response;
            response.body() = R"({"Hello": "world"})";
            response.set(beast::http::field::content_type, "application/json");
            response.prepare_payload();

            bool keep_alive = response.keep_alive();

            co_await beast::http::async_write(m_stream, std::move(response), asio::use_awaitable);

            if (!keep_alive) {
                break;
            }
        }
    } catch (const std::exception &e) {
        co_return;
    }
    boost::system::error_code ec;
    m_stream.socket().shutdown(asio::ip::tcp::socket::shutdown_both, ec);
}
