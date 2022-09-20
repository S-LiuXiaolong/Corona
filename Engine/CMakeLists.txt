cmake_minimum_required (VERSION 3.6)

function(add_submodule APP_SUBMODULE IDE_FOLDER SOURCE INCLUDE SHADERS ASSETS)

    set_source_files_properties(${SHADERS} PROPERTIES VS_TOOL_OVERRIDE "None")
    set(ALL_ASSETS ${ASSETS} ${SHADERS})
    add_target_platform_app(${APP_SUBMODULE} "${SOURCE}" "${INCLUDE}" "${ALL_ASSETS}")

    set_source_files_properties(${ALL_ASSETS} PROPERTIES 
        VS_DEPLOYMENT_LOCATION "."
        MACOSX_PACKAGE_LOCATION "Resources" 
    )

    if(PLATFORM_WIN32)
        set_target_properties(${APP_SUBMODULE} PROPERTIES 
            VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        )
        copy_required_dlls(${APP_SUBMODULE})
        append_engine_base_win32_source(${APP_SUBMODULE})
    elseif(PLATFORM_UNIVERSAL_WINDOWS)
        append_engine_base_uwp_source(${APP_SUBMODULE})
        package_required_dlls(${APP_SUBMODULE})
    endif()

    target_include_directories(${APP_SUBMODULE}
    PRIVATE	
        src
    )

    target_link_libraries(${APP_SUBMODULE}
    PRIVATE
        # On Linux we must have Diligent-NativeAppBase go first, otherwise the linker 
        # will fail to resolve Diligent::CreateApplication() function.
        Diligent-NativeAppBase
        Diligent-BuildSettings
        Corona-EngineBase
    )
    set_common_target_properties(${APP_SUBMODULE})

    if(MSVC)
        # Disable MSVC-specific warnings
        # - w4201: nonstandard extension used: nameless struct/union
        target_compile_options(${APP_SUBMODULE} PRIVATE /wd4201)
    endif()

    set_target_properties(${APP_SUBMODULE} PROPERTIES
        FOLDER ${IDE_FOLDER}
    )

    source_group("src" FILES ${SOURCE} ${INCLUDE})
    source_group("assets" FILES ${ALL_ASSETS})	

    target_sources(${APP_SUBMODULE} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/readme.md")
    set_source_files_properties(
        "${CMAKE_CURRENT_SOURCE_DIR}/readme.md" PROPERTIES HEADER_FILE_ONLY TRUE
    )

    if(PLATFORM_WIN32 OR PLATFORM_LINUX)
        # Copy assets to target folder
        add_custom_command(TARGET ${APP_SUBMODULE} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_CURRENT_SOURCE_DIR}/assets"
                "\"$<TARGET_FILE_DIR:${APP_SUBMODULE}>\"")
    endif()

    if(PLATFORM_MACOS AND VULKAN_LIB_PATH)
        # Configure rpath so that executables can find vulkan library
        set_target_properties(${APP_SUBMODULE} PROPERTIES
            BUILD_RPATH "${VULKAN_LIB_PATH}"
        )
    endif()

    if(PLATFORM_EMSCRIPTEN)
        set(RESOURCE_PATH "${PROJECT_SOURCE_DIR}/assets/")
        target_link_options(${APP_SUBMODULE} PRIVATE "SHELL: -s SINGLE_FILE -s ALLOW_MEMORY_GROWTH=1 --preload-file '${RESOURCE_PATH}@'")
        append_engine_base_emscripten_source(${APP_SUBMODULE})
    endif()

    if(CORONA_BUILD_ENGINE)
        # Install instructions
        file(RELATIVE_PATH TUTORIAL_REL_PATH "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
        install(TARGETS     ${APP_SUBMODULE}
                DESTINATION "${CMAKE_INSTALL_BINDIR}/${TUTORIAL_REL_PATH}/$<CONFIG>")

        if(PLATFORM_LINUX OR PLATFORM_WIN32)
            install(DIRECTORY   "${CMAKE_CURRENT_SOURCE_DIR}/assets/"
                    DESTINATION "${CMAKE_INSTALL_BINDIR}/${TUTORIAL_REL_PATH}/$<CONFIG>")
        endif()

        if(PLATFORM_WIN32)
            get_supported_backends(BACKEND_LIBRARIES)
            install(TARGETS  ${BACKEND_LIBRARIES}
                    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}/${TUTORIAL_REL_PATH}/$<CONFIG>"
                    LIBRARY DESTINATION "${CMAKE_INSTALL_BINDIR}/${TUTORIAL_REL_PATH}/$<CONFIG>"
                    ARCHIVE DESTINATION "${CMAKE_INSTALL_BINDIR}/${TUTORIAL_REL_PATH}/$<CONFIG>")
        endif()

        if(PLATFORM_LINUX)
            set_target_properties(${APP_SUBMODULE} PROPERTIES
                INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/${DILIGENT_CORE_DIR}/${CMAKE_BUILD_TYPE}"
            )
        endif()
    endif()

endfunction()

if(TARGET Diligent-NativeAppBase AND TARGET Diligent-TextureLoader AND TARGET Diligent-Imgui)
    add_subdirectory(EngineBase)
    add_subdirectory(CoronaEditor)
    add_subdirectory(AnimationTest)
endif()

# Create a custom target to run source code formatting validation command
add_format_validation_target(Engine "${CMAKE_CURRENT_SOURCE_DIR}" Engine)
