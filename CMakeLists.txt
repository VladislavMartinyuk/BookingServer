cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(BookingServer LANGUAGES CXX)

include(cmake/core.cmake)

add_executable(BookingServer src/main.cpp)

target_link_libraries(BookingServer PRIVATE BookingCore)

enable_testing()
add_subdirectory(tests)