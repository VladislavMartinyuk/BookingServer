#ifndef SHARED_H
#define SHARED_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <vector>

#include "variantvalue.h"

namespace http {

using StringResponse = boost::beast::http::response<boost::beast::http::string_body>;
using StringRequest = boost::beast::http::request<boost::beast::http::string_body>;
using AwaitableStringResponse = boost::asio::awaitable<StringResponse>;

struct URIParams
{
    std::unordered_map<std::string, VariantValue> routeValue;
    std::unordered_map<std::string, VariantValue> queryValue;
};

} // namespace http

class Utils
{
public:
    static std::vector<std::string> splitString(const std::string &str, const char spliSymbol)
    {
        if (str.empty()) {
            return {};
        }
        std::vector<std::string> result;
        std::string current;
        for (char c : str) {
            if (c == spliSymbol) {
                result.push_back(std::move(current));
            } else {
                current += c;
            }
        }
        result.push_back(std::move(current));
        return result;
    }
};

#endif // SHARED_H
