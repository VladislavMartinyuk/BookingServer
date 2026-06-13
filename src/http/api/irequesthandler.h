#ifndef IREQUESTHANDLER_H
#define IREQUESTHANDLER_H

namespace http {

class IRequestHandler
{
public:
    IRequestHandler() = default;
    virtual ~IRequestHandler() = default;

    virtual void registerRoutes() = 0;
};

} // namespace http

#endif // IREQUESTHANDLER_H
