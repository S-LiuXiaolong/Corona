cmake_minimum_required (VERSION 3.6)

project(Corona-EngineBase)

if(PLATFORM_WIN32)
    set(SOURCE 
        src/Win32/EngineAppWin32.cpp
        src/Win32/InputControllerWin32.cpp
        src/EngineApp.cpp
    )
    set(INCLUDE 
        include/EngineApp.hpp
        include/Win32/InputControllerWin32.hpp
    )
    set(WIN32_RESOURCES
        src/Win32/resources/Win32AppResource.h
        src/Win32/resources/directx11-logo.bmp
        src/Win32/resources/directx12-logo.bmp
        src/Win32/resources/vulkan-logo.bmp
        src/Win32/resources/opengl-logo.bmp
        src/Win32/resources/Win32AppResource.rc
    )

    function(append_engine_base_win32_source TARGET_NAME)
        get_target_property(ENGINE_BASE_SOURCE_DIR Corona-EngineBase SOURCE_DIR)
        # We need to add .res file to executable target to make resources available
        set(RES_FILE ${ENGINE_BASE_SOURCE_DIR}/src/Win32/resources/Win32AppResource.rc)
        target_sources(${TARGET_NAME} PRIVATE ${RES_FILE})
        source_group("resources" FILES ${RES_FILE})
    endfunction()

elseif(PLATFORM_UNIVERSAL_WINDOWS)

    # Windows Runtime types cannot be included into static libraries
    # https://social.msdn.microsoft.com/Forums/en-US/269db513-64ef-4817-a025-43954f614eb3/lnk4264-why-are-static-libraries-not-recommended-when-authoring-windows-runtime-types?forum=winappswithnativecode
    # So as a workaround, we will include all source files into the target app project
    function(append_engine_base_uwp_source TARGET_NAME)
        get_target_property(ENGINE_BASE_SOURCE_DIR Corona-EngineBase SOURCE_DIR)

        set(ENGINE_BASE_UWP_SOURCE
            ${ENGINE_BASE_SOURCE_DIR}/src/UWP/ImguiUWPEventHelper.cpp
            ${ENGINE_BASE_SOURCE_DIR}/src/UWP/EngineAppUWP.cpp
            ${ENGINE_BASE_SOURCE_DIR}/src/UWP/InputControllerEventHandlerUWP.cpp
            ${ENGINE_BASE_SOURCE_DIR}/src/EngineApp.cpp
        )

        set(ENGINE_BASE_UWP_INCLUDE
            ${ENGINE_BASE_SOURCE_DIR}/src/UWP/ImguiUWPEventHelper.h
            ${ENGINE_BASE_SOURCE_DIR}/src/UWP/InputControllerEventHandlerUWP.h
            ${ENGINE_BASE_SOURCE_DIR}/include/EngineApp.hpp
            ${ENGINE_BASE_SOURCE_DIR}/include/UWP/InputControllerUWP.hpp
        )

        set(ENGINE_BASE_UWP_INCLUDE_DIR
            ${ENGINE_BASE_SOURCE_DIR}/src/UWP
        )

        target_sources(${TARGET_NAME} PRIVATE ${ENGINE_BASE_UWP_SOURCE} ${ENGINE_BASE_UWP_INCLUDE})
        source_group("src\\EngineBase" FILES ${ENGINE_BASE_UWP_SOURCE})
        source_group("include\\EngineBase" FILES ${ENGINE_BASE_UWP_INCLUDE})
        target_include_directories(${TARGET_NAME} PRIVATE ${ENGINE_BASE_UWP_INCLUDE_DIR})
    endfunction()

elseif(PLATFORM_ANDROID)
    set(SOURCE
        src/Android/InputControllerAndroid.cpp
        src/Android/EngineAppAndroid.cpp
        src/EngineApp.cpp
    )
    set(INCLUDE 
        include/Android/InputControllerAndroid.hpp
        include/EngineApp.hpp
    )
elseif(PLATFORM_LINUX)
    set(SOURCE 
        src/Linux/InputControllerLinux.cpp
        src/Linux/EngineAppLinux.cpp
        src/EngineApp.cpp
    )
    set(INCLUDE 
        include/Linux/InputControllerLinux.hpp
        include/EngineApp.hpp
    )
elseif(PLATFORM_MACOS)

    set(SOURCE
        src/MacOS/InputControllerMacOS.cpp
        src/MacOS/EngineAppMacOS.mm
        src/EngineApp.cpp
    )
    set(INCLUDE
        Include/MacOS/InputControllerMacOS.hpp
        include/EngineApp.hpp
    )

elseif(PLATFORM_IOS)
    set(SOURCE
        src/IOS/InputControllerIOS.cpp
        src/IOS/EngineAppIOS.cpp
        src/EngineApp.cpp
    )
    set(INCLUDE
        include/IOS/InputControllerIOS.hpp
        include/EngineApp.hpp
    )

elseif(PLATFORM_TVOS)
    set(SOURCE
        src/TVOS/EngineAppTVOS.cpp
        src/EngineApp.cpp
    )
    set(INCLUDE
        include/EngineApp.hpp
    )

elseif(PLATFORM_EMSCRIPTEN)
    set(SOURCE
        src/Emscripten/InputControllerEmscripten.cpp
        src/Emscripten/EngineAppEmscripten.cpp
        src/EngineApp.cpp
    )
    set(INCLUDE
        include/Emscripten/InputControllerEmscripten.hpp
        include/EngineApp.hpp
    )

    function(append_engine_base_emscripten_source TARGET_NAME)     
        get_target_property(ENGINE_BASE_SOURCE_DIR Corona-EngineBase SOURCE_DIR)
        set(HTML_TEMPLATE_FILE 
            ${ENGINE_BASE_SOURCE_DIR}/src/Emscripten/resources/emscripten_template.html
        )          
        target_link_options(${TARGET_NAME} PRIVATE "SHELL: --shell-file '${HTML_TEMPLATE_FILE}'")   
    endfunction()
    
else()
    message(FATAL_ERROR "Unknown platform")
endif()

list(APPEND SOURCE
    src/FirstPersonCamera.cpp
    src/EngineBase.cpp
)

list(APPEND INCLUDE
    include/FirstPersonCamera.hpp
    include/InputController.hpp
    include/EngineBase.hpp
)


add_library(Corona-EngineBase STATIC ${SOURCE} ${INCLUDE})
set_common_target_properties(Corona-EngineBase)

target_include_directories(Corona-EngineBase
PUBLIC
    include
)

if(MSVC)
    target_compile_options(Corona-EngineBase PRIVATE -DUNICODE)

    if(PLATFORM_UNIVERSAL_WINDOWS)
        # Disable w4189: local variable is initialized but not referenced
        # Disable w4063: case is not a valid value for switch of enum
        # Consume the windows runtime extensions (/ZW)
        target_compile_options(Corona-EngineBase INTERFACE /wd4189 /wd4063 /ZW)
    endif()
endif()

get_supported_backends(ENGINE_LIBRARIES)

target_link_libraries(Corona-EngineBase 
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

if(PLATFORM_UNIVERSAL_WINDOWS)
    target_link_libraries(Corona-EngineBase PRIVATE dxguid.lib)
elseif(PLATFORM_ANDROID)
    target_link_libraries(Corona-EngineBase PRIVATE GLESv3 PUBLIC native_app_glue)
elseif(PLATFORM_LINUX)
    target_link_libraries(Corona-EngineBase PRIVATE XCBKeySyms GL X11)
elseif(PLATFORM_MACOS OR PLATFORM_IOS)

endif()

source_group("src" FILES ${SOURCE})
source_group("include" FILES ${INCLUDE})

if(PLATFORM_WIN32)
    target_sources(Corona-EngineBase PRIVATE ${WIN32_RESOURCES})
    source_group("resources" FILES ${WIN32_RESOURCES})
endif()

set_target_properties(Corona-EngineBase PROPERTIES
    FOLDER Engine
)
