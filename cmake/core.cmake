set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package(Boost REQUIRED)

add_library(BookingCore
    ${SRC}/http/server.h ${SRC}/http/server.cpp
    ${SRC}/http/router.h ${SRC}/http/router.cpp
    ${SRC}/http/session.h ${SRC}/http/session.cpp
    ${SRC}/http/shared.h
    ${SRC}/http/variantvalue.h ${SRC}/http/variantvalue.cpp)

target_include_directories(BookingCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(BookingCore PUBLIC Boost::boost)