cmake_minimum_required(VERSION 3.10)
project(ExpressionTree)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(inc)

add_executable(expression_tree src/main.cpp)

add_executable(run_tests tests/test_expression_tree.cpp)
