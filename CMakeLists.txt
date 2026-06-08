cmake_minimum_required(VERSION 3.14)
project(apple_music_rpc_cpp LANGUAGES CXX OBJCXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_OBJCXX_STANDARD 20)
set(CMAKE_OBJCXX_STANDARD_REQUIRED ON)

add_subdirectory(external/discord_ipc_cpp)

set(OBJCXX_SOURCES
  src/apple_music.mm
  src/itunes_api.mm
  src/notif_binder.mm
  src/objc_bridge.mm
)

add_executable(apple_music_rpc_cpp
  src/main.cpp
  src/handler.cpp
  src/music_types.cpp
  src/utils.cpp
  ${OBJCXX_SOURCES}
)

target_include_directories(apple_music_rpc_cpp PRIVATE
  external/
  src/
)

target_compile_options(apple_music_rpc_cpp PRIVATE
  -Wall
  -Wextra

  -O3
  -pthread
)

set_source_files_properties(${OBJCXX_SOURCES} PROPERTIES
  COMPILE_OPTIONS "-fobjc-arc"
)

target_link_libraries(apple_music_rpc_cpp PRIVATE
  discord_ipc_cpp
  "-framework Foundation"
  "-framework AppKit"
  "-framework ScriptingBridge"
)
