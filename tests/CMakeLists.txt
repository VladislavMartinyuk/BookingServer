find_package(GTest REQUIRED)

add_executable(tests router_test.cpp)

target_link_libraries(tests PRIVATE gtest::gtest BookingCore)