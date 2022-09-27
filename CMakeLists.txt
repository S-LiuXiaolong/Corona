cmake_minimum_required(VERSION 3.6)
set(CMAKE_C_COMPILER "clang-cl")
set(CMAKE_C_FLAGS_DEBUG "/Debug")
set(CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O4 -DNDEBUG")

set(CMAKE_C_STANDARD 11)

set (CMAKE_CXX_COMPILER             "clang-cl")
set (CMAKE_CXX_FLAGS_DEBUG          "/Debug")
set (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set (CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 /Debug")

set(CMAKE_CXX_STANDARD 17)

project(CoronaEngine)
include_directories("${PROJECT_SOURCE_DIR}/Framework/Common")
include_directories("${PROJECT_SOURCE_DIR}/Framework/Interface")

add_subdirectory(Framework)
add_subdirectory(Platform/Windows)