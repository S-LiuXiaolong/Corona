cmake_minimum_required (VERSION 3.6)

project(EngineBase)

if(PLATFORM_WIN32)
    set(WIN32_RESOURCES
        src/Win32/resources/Win32AppResource.h
        src/Win32/resources/directx11-logo.bmp
        src/Win32/resources/directx12-logo.bmp
        src/Win32/resources/vulkan-logo.bmp
        src/Win32/resources/opengl-logo.bmp
        src/Win32/resources/Win32AppResource.rc
    )

    function(append_engine_base_win32_source TARGET_NAME)
        get_target_property(ENGINE_BASE_SOURCE_DIR EngineBase SOURCE_DIR)
        # We need to add .res file to executable target to make resources available
        set(RES_FILE ${ENGINE_BASE_SOURCE_DIR}/src/Win32/resources/Win32AppResource.rc)
        target_sources(${TARGET_NAME} PRIVATE ${RES_FILE})
        source_group("resources" FILES ${RES_FILE})
    endfunction()

else()
    message(FATAL_ERROR "Unknown platform")
endif()

set(SOURCE 
    src/EngineApp.cpp
    src/EngineBase.cpp
    src/Win32/EngineAppWin32.cpp
    src/Win32/InputControllerWin32.cpp
)
set(INCLUDE 
    include/EngineApp.hpp
    include/EngineBase.hpp
    include/Win32/InputControllerWin32.hpp
    include/InputController.hpp
)

add_library(EngineBase STATIC ${SOURCE} ${INCLUDE})
set_common_target_properties(EngineBase)

target_include_directories(EngineBase
PUBLIC
    include
)

if(MSVC)
    target_compile_options(EngineBase PRIVATE -DUNICODE)

    if(PLATFORM_UNIVERSAL_WINDOWS)
        # Disable w4189: local variable is initialized but not referenced
        # Disable w4063: case is not a valid value for switch of enum
        # Consume the windows runtime extensions (/ZW)
        target_compile_options(EngineBase INTERFACE /wd4189 /wd4063 /ZW)
    endif()
endif()

get_supported_backends(ENGINE_LIBRARIES)

target_link_libraries(EngineBase 
PRIVATE 
    Diligent-BuildSettings
PUBLIC
    Diligent-Common
    Diligent-GraphicsTools
    Diligent-TextureLoader
    Diligent-TargetPlatform
    Diligent-Imgui
    Diligent-GraphicsAccessories
    ${ENGINE_LIBRARIES}
    Diligent-NativeAppBase
)

source_group("src" FILES ${SOURCE})
source_group("include" FILES ${INCLUDE})

if(PLATFORM_WIN32)
    target_sources(EngineBase PRIVATE ${WIN32_RESOURCES})
    source_group("resources" FILES ${WIN32_RESOURCES})
endif()

set_target_properties(EngineBase PROPERTIES
    FOLDER Engine
)
