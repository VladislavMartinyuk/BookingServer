#ifndef REQUESTLOGGINGMIDDLEWARE_H
#define REQUESTLOGGINGMIDDLEWARE_H

#include "irequestmiddleware.h"

#include <spdlog/spdlog.h>

namespace http {

class RequestLoggingMiddleware : public IRequestMiddleware
{
public:
    RequestLoggingMiddleware() = default;

    void process(const StringRequest &request) override
    {
        auto target = request.target();
        auto method = request.method_string();
        std::string body = request.body();
        auto formatted = std::format("Http request. Target: {}, Method: {}, Body: {}",
                                     std::move(target),
                                     std::move(method),
                                     std::move(body));
        spdlog::info(formatted);
    }
};

} // namespace http

#endif // REQUESTLOGGINGMIDDLEWARE_H
