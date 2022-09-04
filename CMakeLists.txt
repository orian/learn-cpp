cmake_minimum_required(VERSION 3.21)

project(learn_cpp)

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -Wextra -g0")

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(threadpool)

add_executable(learn_cpp main.cpp)
add_executable(benchmark_modulo benchmark_modulo.cpp)
add_executable(benchmark_op benchmark_op.cpp)