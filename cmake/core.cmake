set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package(Boost REQUIRED)
find_package(spdlog REQUIRED)

add_library(BookingCore
    ${SRC}/http/base/server.h ${SRC}/http/base/server.cpp
    ${SRC}/http/base/router.h ${SRC}/http/base/router.cpp
    ${SRC}/http/base/session.h ${SRC}/http/base/session.cpp
    ${SRC}/http/shared/shared.h
    ${SRC}/http/shared/variantvalue.h ${SRC}/http/shared/variantvalue.cpp
    ${SRC}/http/base/middleware/irequestmiddleware.h
    ${SRC}/http/base/middleware/requestloggingmiddleware.h
    ${SRC}/http/api/irequesthandler.h
    ${SRC}/http/api/apimanager.h
    ${SRC}/http/api/apimanager.cpp
    ${SRC}/http/api/v1/usershandler.h ${SRC}/http/api/v1/usershandler.cpp
)

target_include_directories(BookingCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(BookingCore PUBLIC Boost::boost spdlog::spdlog)