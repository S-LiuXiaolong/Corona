add_library(Common
Allocator.cpp
AssetLoader.cpp
BaseApplication.cpp
GraphicsManager.cpp
Image.cpp
MemoryManager.cpp
SceneManager.cpp
SceneObject.cpp
glTFSceneObject.cpp
main.cpp
)

target_link_libraries(Common GeomMath)

# if (CMAKE_BUILD_TYPE STREQUAL "Debug")
#     find_library(XG_LIBRARY crossguid-dgb PATHS ${MYGE_EXTERNAL_LIBRARY_PATH})
# endif()
#     
# if (CMAKE_BUILD_TYPE STREQUAL "Release")
#     find_library(XG_LIBRARY crossguid PATHS ${MYGE_EXTERNAL_LIBRARY_PATH})
# endif()

set(CORONA_LIBRARIES_LIST crossguid OpenDDL OpenGEX)

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

MESSAGE( STATUS "CORONA_EXLIBRARY: " ${CORONA_EXLIBRARY} )

find_library(XG_LIBRARY_DEBUG           xg PATHS ${MYGE_EXTERNAL_LIBRARY_PATH}/Debug)
find_library(XG_LIBRARY_RELEASE         xg PATHS ${MYGE_EXTERNAL_LIBRARY_PATH}/Release)

find_library(OPENDDL_LIBRARY_DEBUG      OpenDDL PATHS ${MYGE_EXTERNAL_LIBRARY_PATH}/Debug)
find_library(OPENDDL_LIBRARY_RELEASE    OpenDDL PATHS ${MYGE_EXTERNAL_LIBRARY_PATH}/Release)

find_library(OPENGEX_LIBRARY_DEBUG      OpenGEX PATHS ${MYGE_EXTERNAL_LIBRARY_PATH}/Debug)
find_library(OPENGEX_LIBRARY_RELEASE    OpenGEX PATHS ${MYGE_EXTERNAL_LIBRARY_PATH}/Release)

find_library(TINYGLTF_LIBRARY_DEBUG      tinyglTF PATHS ${MYGE_EXTERNAL_LIBRARY_PATH}/Debug)
find_library(TINYGLTF_LIBRARY_RELEASE    tinyglTF PATHS ${MYGE_EXTERNAL_LIBRARY_PATH}/Release)
# target_link_libraries(Common ${XG_LIBRARY} ${OPENDDL_LIBRARY} ${OPENGEX_LIBRARY})

if(WIN32)
    add_definitions(-DGUID_WINDOWS)
endif()

