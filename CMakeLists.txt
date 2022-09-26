cmake_minimum_required(VERSION 3.6)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

project(CoronaEngine)
include_directories("${PROJECT_SOURCE_DIR}/Framework/Common")
include_directories("${PROJECT_SOURCE_DIR}/Framework/Interface")

add_subdirectory(Framework)
add_subdirectory(Empty)