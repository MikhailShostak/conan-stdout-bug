cmake_minimum_required(VERSION 3.15)
project(Wrapper)

find_package(Boost REQUIRED)

add_executable(Boost_Wrapper boost_main.cpp)
target_link_libraries(Boost_Wrapper boost::boost)

add_executable(WinAPI_Wrapper winapi_main.cpp)
