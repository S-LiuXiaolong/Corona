add_library(Common
Allocator.cpp
AssetLoader.cpp
BaseApplication.cpp
Image.cpp
GraphicsManager.cpp
MemoryManager.cpp
SceneManager.cpp
InputManager.cpp
SceneObject.cpp
SceneNode.cpp
Scene.cpp
main.cpp
)

find_library(XG_LIBRARY_DEBUG           xg PATHS ${MYGE_EXTERNAL_LIBRARY_PATH}/Debug)
find_library(XG_LIBRARY_RELEASE         xg PATHS ${MYGE_EXTERNAL_LIBRARY_PATH}/Release)

find_library(TINYGLTF_LIBRARY_DEBUG      tinyglTF PATHS ${MYGE_EXTERNAL_LIBRARY_PATH}/Debug)
find_library(TINYGLTF_LIBRARY_RELEASE    tinyglTF PATHS ${MYGE_EXTERNAL_LIBRARY_PATH}/Release)

find_library(ZLIB_LIBRARY_DEBUG      zlibstaticd PATHS ${MYGE_EXTERNAL_LIBRARY_PATH}/Debug)
find_library(ZLIB_LIBRARY_RELEASE    zlibstatic PATHS ${MYGE_EXTERNAL_LIBRARY_PATH}/Release)

find_library(LODEPNG_LIBRARY_DEBUG      lodepng PATHS ${MYGE_EXTERNAL_LIBRARY_PATH}/Debug)
find_library(LODEPNG_LIBRARY_RELEASE    lodepng PATHS ${MYGE_EXTERNAL_LIBRARY_PATH}/Release)

find_library(JPEG_LIBRARY_DEBUG      jpeg-static PATHS ${MYGE_EXTERNAL_LIBRARY_PATH}/Debug)
find_library(JPEG_LIBRARY_RELEASE    jpeg-static PATHS ${MYGE_EXTERNAL_LIBRARY_PATH}/Release)

find_library(TURBOJPEG_LIBRARY_DEBUG      turbojpeg-static PATHS ${MYGE_EXTERNAL_LIBRARY_PATH}/Debug)
find_library(TURBOJPEG_LIBRARY_RELEASE    turbojpeg-static PATHS ${MYGE_EXTERNAL_LIBRARY_PATH}/Release)

find_library(GLFW_LIBRARY_DEBUG      glfw3 PATHS ${MYGE_EXTERNAL_LIBRARY_PATH}/Debug)
find_library(GLFW_LIBRARY_RELEASE    glfw3 PATHS ${MYGE_EXTERNAL_LIBRARY_PATH}/Release)

target_link_libraries(Common GeomMath)
target_link_libraries(Common 
            debug ${XG_LIBRARY_DEBUG}
            debug ${TINYGLTF_LIBRARY_DEBUG}
            debug ${ZLIB_LIBRARY_DEBUG}
            debug ${LODEPNG_LIBRARY_DEBUG}
            debug ${JPEG_LIBRARY_DEBUG}
            debug ${TURBOJPEG_LIBRARY_DEBUG}
            )
target_link_libraries(Common 
            optimized ${XG_LIBRARY_RELEASE}
            optimized ${TINYGLTF_LIBRARY_RELEASE}
            optimized ${ZLIB_LIBRARY_RELEASE}
            optimized ${LODEPNG_LIBRARY_RELEASE}
            optimized ${JPEG_LIBRARY_RELEASE}
            optimized ${TURBOJPEG_LIBRARY_RELEASE}
            )

# if (CMAKE_BUILD_TYPE STREQUAL "Debug")
#     find_library(XG_LIBRARY crossguid-dgb PATHS ${MYGE_EXTERNAL_LIBRARY_PATH})
# endif()
#     
# if (CMAKE_BUILD_TYPE STREQUAL "Release")
#     find_library(XG_LIBRARY crossguid PATHS ${MYGE_EXTERNAL_LIBRARY_PATH})
# endif()

# set(CORONA_LIBRARIES_LIST crossguid OpenDDL OpenGEX)

# foreach(search_lib ${CORONA_LIBRARIES_LIST})
#     find_library(CORONA_EXLIB_DEBUG NAMES ${search_lib} 
#         PATHS
#         ${MYGE_EXTERNAL_LIBRARY_PATH}/Debug
#     )
# 
#     find_library(CORONA_EXLIB_RELEASE NAMES ${search_lib}
#         PATHS
#         ${MYGE_EXTERNAL_LIBRARY_PATH}/Release
#     )
# 
#     set(CORONA_EXLIBRARY optimized ${CORONA_EXLIB_DEBUG} debug ${CORONA_EXLIB_RELEASE})
# endforeach(search_lib)

if(WIN32)
    add_definitions(-DGUID_WINDOWS)
endif()

