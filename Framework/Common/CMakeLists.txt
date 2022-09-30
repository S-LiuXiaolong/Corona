add_library(Common
Allocator.cpp
AssetLoader.cpp
BaseApplication.cpp
GraphicsManager.cpp
Image.cpp
MemoryManager.cpp
SceneManager.cpp
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

find_library(XG_LIBRARY_DEBUG           crossguid-dgb PATHS ${MYGE_EXTERNAL_LIBRARY_PATH}/Debug)
find_library(XG_LIBRARY_RELEASE         crossguid PATHS ${MYGE_EXTERNAL_LIBRARY_PATH}/Release)

find_library(OPENDDL_LIBRARY_DEBUG      OpenDDL PATHS ${MYGE_EXTERNAL_LIBRARY_PATH}/Debug)
find_library(OPENDDL_LIBRARY_RELEASE    OpenDDL PATHS ${MYGE_EXTERNAL_LIBRARY_PATH}/Release)

find_library(OPENGEX_LIBRARY_DEBUG      OpenGEX PATHS ${MYGE_EXTERNAL_LIBRARY_PATH}/Debug)
find_library(OPENGEX_LIBRARY_RELEASE    OpenGEX PATHS ${MYGE_EXTERNAL_LIBRARY_PATH}/Release)
# target_link_libraries(Common ${XG_LIBRARY} ${OPENDDL_LIBRARY} ${OPENGEX_LIBRARY})

if(WIN32)
    add_definitions(-DGUID_WINDOWS)
endif()

