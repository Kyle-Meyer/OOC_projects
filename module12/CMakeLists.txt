cmake_minimum_required(VERSION 3.15)
project(Poker CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SRC src/*.cpp)
add_executable(poker ${SRC})
target_include_directories(poker PRIVATE inc)
