#ifndef IREQUESTMIDDLEWARE_H
#define IREQUESTMIDDLEWARE_H

#include "http/shared/shared.h"

namespace http {

class IRequestMiddleware
{
public:
    IRequestMiddleware() = default;
    virtual ~IRequestMiddleware() = default;

    virtual void process(const StringRequest &request) = 0;
};

} // namespace http

#endif // IREQUESTMIDDLEWARE_H
