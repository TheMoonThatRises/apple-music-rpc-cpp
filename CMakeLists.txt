cmake_minimum_required(VERSION 4.0)
project(apple_music_rpc_cpp)

file(GLOB_RECURSE sources ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(apple_music_rpc ${sources})

target_compile_options(apple_music_rpc PUBLIC -Wall -Wextra -O3 -pthread -std=c++20)
