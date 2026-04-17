cmake_minimum_required(VERSION 3.10)
project(module14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(unthreaded unthreaded.cpp)
target_link_libraries(unthreaded Threads::Threads)

add_executable(threaded threaded.cpp)
target_link_libraries(threaded Threads::Threads)
