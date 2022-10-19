cmake_minimum_required(VERSION 3.6)

set(CMAKE_C_STANDARD 11)

set(CMAKE_CXX_STANDARD 17)

project(CoronaEngine)
include_directories("${PROJECT_SOURCE_DIR}/Framework/Ability")
include_directories("${PROJECT_SOURCE_DIR}/Framework/Common")
include_directories("${PROJECT_SOURCE_DIR}/Framework/GeomMath")
include_directories("${PROJECT_SOURCE_DIR}/Framework/Geometries")
include_directories("${PROJECT_SOURCE_DIR}/Framework/Interface")
include_directories("${PROJECT_SOURCE_DIR}/Framework/Parser")
include_directories("${PROJECT_SOURCE_DIR}/RHI")

IF(${UNIX})
    set(MYGE_TARGET_PLATFORM "Linux")
ELSEIF(${WIN32})
    add_definitions(/D _CRT_SECURE_NO_WARNINGS)
    set(MYGE_TARGET_PLATFORM "Windows")
ENDIF(${UNIX})
include_directories("${PROJECT_SOURCE_DIR}/Platform/${MYGE_TARGET_PLATFORM}")

# IF (CMAKE_BUILD_TYPE MATCHES "Debug")
#     set(MYBUILD_TYPE_DEFINE "Debug")
# ELSEIF (CMAKE_BUILD_TYPE MATCHES "Release")
#     set(MYBUILD_TYPE_DEFINE "Release")
# ENDIF()

set(MYGE_EXTERNAL_ROOT ${PROJECT_SOURCE_DIR}/External/${MYGE_TARGET_PLATFORM}/)
include_directories("${MYGE_EXTERNAL_ROOT}/include")
# don't know how to get pre-build type in cmake
# set(MYGE_EXTERNAL_LIBRARY_PATH ${MYGE_EXTERNAL_ROOT}/lib/${MYBUILD_TYPE_DEFINE}/)
set(MYGE_EXTERNAL_LIBRARY_PATH ${MYGE_EXTERNAL_ROOT}/lib)

# include(CTest)
add_subdirectory(Asset)
add_subdirectory(Editor)
add_subdirectory(Framework)
add_subdirectory(Platform)
add_subdirectory(RHI)
add_subdirectory(Test)
