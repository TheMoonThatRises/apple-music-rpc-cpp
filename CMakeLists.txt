cmake_minimum_required(VERSION 3.14)
project(apple_music_rpc_cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src/discord_ipc_cpp)

add_executable(apple_music_rpc_cpp src/apple_music_rpc_cpp/main.cpp)

target_include_directories(apple_music_rpc_cpp PRIVATE
  src/apple_music_rpc_cpp/include
)

target_sources(apple_music_rpc_cpp PRIVATE
  src/apple_music_rpc_cpp/utils.cpp
)

target_link_libraries(apple_music_rpc_cpp PRIVATE discord_ipc_cpp)
target_compile_options(apple_music_rpc_cpp PRIVATE -Wall -Wextra -O3 -pthread)
