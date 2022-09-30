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

find_library(XG_LIBRARY crossguid-dgb PATHS ${MYGE_EXTERNAL_LIBRARY_PATH})
find_library(OPENDDL_LIBRARY OpenDDL PATHS ${MYGE_EXTERNAL_LIBRARY_PATH})
find_library(OPENGEX_LIBRARY OpenGEX PATHS ${MYGE_EXTERNAL_LIBRARY_PATH})
# target_link_libraries(Common ${XG_LIBRARY} ${OPENDDL_LIBRARY} ${OPENGEX_LIBRARY})

if(WIN32)
    add_definitions(-DGUID_WINDOWS)
endif()

