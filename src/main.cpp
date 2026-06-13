#include "http/base/server.h"

int main()
{
    http::Server::instance().miltithreaded(true).port(8080).start();
    return 0;
}
