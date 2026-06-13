#ifndef USERSHANDLER_H
#define USERSHANDLER_H

#include "http/api/irequesthandler.h"
#include "http/shared/shared.h"

namespace http::v1 {

class UsersHandler : public IRequestHandler
{
public:
    UsersHandler();

    void registerRoutes() override;

private:
    AwaitableStringResponse allUsers(StringRequest req, URIParams params);
};

} // namespace http::v1

#endif // USERSHANDLER_H
