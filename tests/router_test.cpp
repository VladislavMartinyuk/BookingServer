#include <gtest/gtest.h>

#include "http/base/router.h"

class HttpRouterFixture : public ::testing::Test
{
protected:
    http::Router m_router;

    void SetUp() override
    {
        m_router.addRoute("GET",
                          "/users",
                          [](http::StringRequest,
                             http::URIParams params) -> http::AwaitableStringResponse {
                              http::StringResponse response;
                              if (params.queryValue.empty()) {
                                  response.body() = "users_plain_get";
                              } else {
                                  auto userId = params.queryValue.at("userId").toString();
                                  auto orderId = params.queryValue.at("orderId").toString();
                                  auto formatted = std::format("userId:{}orderId:{}",
                                                               std::move(userId),
                                                               std::move(orderId));
                                  response.body() = std::move(formatted);
                              }
                              response.prepare_payload();
                              co_return response;
                          });

        m_router.addRoute("POST",
                          "/users",
                          [](http::StringRequest,
                             http::URIParams params) -> http::AwaitableStringResponse {
                              http::StringResponse response;
                              if (params.queryValue.empty()) {
                                  response.body() = "users_plain_post";
                              } else {
                                  response.body() = "users_with_query_post";
                              }
                              response.prepare_payload();
                              co_return response;
                          });

        m_router.addRoute("GET",
                          "/users/{userId}/orders/{orderId}",
                          [](http::StringRequest,
                             http::URIParams params) -> http::AwaitableStringResponse {
                              http::StringResponse response;
                              auto userId = params.routeValue.at("userId").toString();
                              auto orderId = params.routeValue.at("orderId").toString();
                              auto formatted = std::format("userId:{}orderId:{}",
                                                           std::move(userId),
                                                           std::move(orderId));
                              response.body() = std::move(formatted);
                              response.prepare_payload();
                              co_return response;
                          });
    }
};

TEST_F(HttpRouterFixture, GetUsersWithotQueryParams)
{
    boost::asio::io_context ioContext;

    auto f = boost::asio::co_spawn(
        ioContext,
        [this]() -> http::AwaitableStringResponse {
            http::StringRequest test1{boost::beast::http::verb::get, "/users", 11};
            co_return co_await m_router.processRequest(test1);
        },
        boost::asio::use_future);

    ioContext.run();

    auto result = f.get();
    EXPECT_EQ(result.body(), "users_plain_get");
}

TEST_F(HttpRouterFixture, PostUsersWithotQueryParams)
{
    boost::asio::io_context ioContext;

    auto f = boost::asio::co_spawn(
        ioContext,
        [this]() -> http::AwaitableStringResponse {
            http::StringRequest test1{boost::beast::http::verb::post, "/users", 11};
            co_return co_await m_router.processRequest(test1);
        },
        boost::asio::use_future);

    ioContext.run();

    auto result = f.get();
    EXPECT_EQ(result.body(), "users_plain_post");
}

TEST_F(HttpRouterFixture, GetDynamicFields)
{
    boost::asio::io_context ioContext;

    auto f = boost::asio::co_spawn(
        ioContext,
        [this]() -> http::AwaitableStringResponse {
            http::StringRequest test2{boost::beast::http::verb::get, "/users/11/orders/12", 11};
            co_return co_await m_router.processRequest(test2);
        },
        boost::asio::use_future);

    ioContext.run();

    auto result = f.get();
    EXPECT_EQ(result.body(), "userId:11orderId:12");
}

TEST_F(HttpRouterFixture, GetQueryParams)
{
    boost::asio::io_context ioContext;

    auto f = boost::asio::co_spawn(
        ioContext,
        [this]() -> http::AwaitableStringResponse {
            http::StringRequest test3{boost::beast::http::verb::get,
                                      "/users?userId=666&orderId=1111",
                                      11};
            co_return co_await m_router.processRequest(test3);
        },
        boost::asio::use_future);

    ioContext.run();

    auto result = f.get();
    EXPECT_EQ(result.body(), "userId:666orderId:1111");
}