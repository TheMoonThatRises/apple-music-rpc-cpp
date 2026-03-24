cmake_minimum_required(VERSION 3.14)
project(apple_music_rpc_cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/discord_ipc_cpp)

add_executable(apple_music_rpc_cpp
  src/main.cpp
)

target_include_directories(apple_music_rpc_cpp PRIVATE
  external/
  src/
)

target_sources(apple_music_rpc_cpp PRIVATE
  src/apple_music.mm
  src/handler.cpp
  src/itunes_api.mm
  src/music_types.cpp
  src/notif_binder.mm
  src/objc_bridge.mm
  src/utils.cpp
)

target_link_libraries(apple_music_rpc_cpp PRIVATE discord_ipc_cpp)
target_link_libraries(apple_music_rpc_cpp PRIVATE
  "-framework Foundation"
  "-framework AppKit"
  "-framework ScriptingBridge"
)
target_compile_options(apple_music_rpc_cpp PRIVATE -Wall -Wextra -O3 -pthread)
