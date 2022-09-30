add_library(Common
Allocator.cpp
BaseApplication.cpp
AssetLoader.cpp
GraphicsManager.cpp
MemoryManager.cpp
main.cpp
)
target_link_libraries(Common GeomMath)

find_library(XG_LIBRARY crossguid PATHS ${MYGE_EXTERNAL_LIBRARY_PATH})
target_link_libraries(Common ${XG_LIBRARY})

if(WIN32)
    add_definitions(-DGUID_WINDOWS)
endif()

add_subdirectory(test)
