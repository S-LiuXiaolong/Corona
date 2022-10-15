# Build HLSL shaders
add_custom_target(Engine_Asset_Shaders
COMMAND echo "Start processing Engine Shaders"
)

set(HLSL_SHADER_SOURCES 
            default.vert default.frag
            pbr.vert pbr.frag
        )

IF(WIN32)
#     set(GLSL_VALIDATOR ${PROJECT_SOURCE_DIR}/External/Windows/bin/glslangValidator.exe)
#     set(SPIRV_CROSS    ${PROJECT_SOURCE_DIR}/External/Windows/bin/SPIRV-Cross.exe)
     set(DXC            dxc.exe)
# ELSE(WIN32)
#     set(GLSL_VALIDATOR ${PROJECT_SOURCE_DIR}/External/${MYGE_TARGET_PLATFORM}/bin/glslangValidator)
#     set(SPIRV_CROSS    ${PROJECT_SOURCE_DIR}/External/${MYGE_TARGET_PLATFORM}/bin/spirv-cross)
ENDIF(WIN32)

IF(WIN32)
    # Compile HLSL shader sources
    set(SHADER_BIN_DIR ${PROJECT_BINARY_DIR}/Asset/Shaders/HLSL)

    add_custom_command(TARGET Engine_Asset_Shaders PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADER_BIN_DIR}
    )

    foreach(SHADER IN LISTS HLSL_SHADER_SOURCES)
        set(SHADER_BIN ${SHADER_BIN_DIR}/${SHADER}.cso)
        if (SHADER MATCHES "^([a-zA-Z0-9_]*)\.vert$")
            add_custom_command(TARGET Engine_Asset_Shaders POST_BUILD
                COMMAND ${DXC} -T vs_6_1 -E ${CMAKE_MATCH_1}_vert_main -Fo ${SHADER_BIN} -I ..\\..\\..\\ -I ..\\..\\..\\Framework\\Common ${SHADER}.hlsl
                COMMENT "Compile ${SHADER}.hlsl --> ${SHADER}.cso"
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Asset/Shaders/HLSL
                DEPENDS ${PROJECT_SOURCE_DIR}/Asset/Shaders/HLSL/${SHADER}.hlsl
            )
        elseif (SHADER MATCHES "^([a-zA-Z0-9_]*)\.frag$")
            add_custom_command(TARGET Engine_Asset_Shaders POST_BUILD
                COMMAND ${DXC} -T ps_6_1 -E ${CMAKE_MATCH_1}_frag_main -Fo ${SHADER_BIN} -I ..\\..\\..\\ -I ..\\..\\..\\Framework\\Common ${SHADER}.hlsl
                COMMENT "Compile ${SHADER}.hlsl --> ${SHADER}.cso"
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Asset/Shaders/HLSL
                DEPENDS ${PROJECT_SOURCE_DIR}/Asset/Shaders/HLSL/${SHADER}.hlsl
            )
        elseif (SHADER MATCHES "^([a-zA-Z0-9_]*)\.comp$")
            add_custom_command(TARGET Engine_Asset_Shaders POST_BUILD
                COMMAND ${DXC} -T cs_6_1 -E ${CMAKE_MATCH_1}_comp_main -Fo ${SHADER_BIN} -I ..\\..\\..\\ -I ..\\..\\..\\Framework\\Common ${SHADER}.hlsl
                COMMENT "Compile ${SHADER}.hlsl --> ${SHADER}.cso"
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Asset/Shaders/HLSL
                DEPENDS ${PROJECT_SOURCE_DIR}/Asset/Shaders/HLSL/${SHADER}.hlsl
            )
        elseif (SHADER MATCHES "^([a-zA-Z0-9_]*)\.geom$")
            add_custom_command(TARGET Engine_Asset_Shaders POST_BUILD
                COMMAND ${DXC} -T gs_6_1 -E ${CMAKE_MATCH_1}_geom_main -Fo ${SHADER_BIN} -I ..\\..\\..\\ -I ..\\..\\..\\Framework\\Common ${SHADER}.hlsl
                COMMENT "Compile ${SHADER}.hlsl --> ${SHADER}.cso"
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Asset/Shaders/HLSL
                DEPENDS ${PROJECT_SOURCE_DIR}/Asset/Shaders/HLSL/${SHADER}.hlsl
            )
        endif ()
    endforeach(SHADER)
ENDIF(WIN32)

add_custom_command(TARGET Engine_Asset_Shaders
                   COMMAND ${CMAKE_COMMAND} -E 
                   copy_directory ${PROJECT_BINARY_DIR}/Asset/Shaders/HLSL ${PROJECT_SOURCE_DIR}/Asset/Shaders/HLSL)



    