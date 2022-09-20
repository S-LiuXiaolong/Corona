cmake_minimum_required (VERSION 3.6)

project(AnimationTest CXX)

set(SOURCE
    src/ImguiDemo.cpp
    ${DILIGENT_DEAR_IMGUI_PATH}/imgui_demo.cpp
)

set(INCLUDE
    src/ImguiDemo.hpp
)

file(GLOB CESIUM_MAN_MODEL LIST_DIRECTORIES false assets/models/CesiumMan/*.*)

set(MODELS ${CESIUM_MAN_MODEL})
set(TEXTURES assets/textures/papermill.ktx)

set(RENDER_STATES assets/render_states/RenderStates.json)

set(ASSETS
    ${MODELS}
    ${TEXTURES}
    ${RENDER_STATES}
)

add_submodule("AnimationTest" "Engine" "${SOURCE}" "${INCLUDE}" "${ALL_SHADERS}" "${ASSETS}")
target_link_libraries(AnimationTest 
PRIVATE
    Diligent-AssetLoader 
    Diligent-RenderStateNotation 
    DiligentFX
)

foreach(FILE ${EXTERNAL_SHADERS})
    # Copy external shaders
    add_custom_command(TARGET AnimationTest PRE_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/${FILE}" "${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders"
    )
endforeach(FILE)

set_source_files_properties(${ALL_SHADERS} PROPERTIES
    VS_DEPLOYMENT_LOCATION "shaders"
    MACOSX_PACKAGE_LOCATION Resources/shaders
)

set_source_files_properties(${RENDER_STATES} PROPERTIES
    VS_DEPLOYMENT_LOCATION "render_states"
    MACOSX_PACKAGE_LOCATION "Resources/render_states"
)

set_source_files_properties(${TEXTURES} PROPERTIES
    VS_DEPLOYMENT_LOCATION "textures"
    MACOSX_PACKAGE_LOCATION "Resources/textures"
)

set_source_files_properties(${CESIUM_MAN_MODEL} PROPERTIES
    VS_DEPLOYMENT_LOCATION "models/CesiumMan"
    MACOSX_PACKAGE_LOCATION "Resources/models/CesiumMan"
)

# We have to use a different group name (Assets with capital A) to override grouping that was set by add_sample_app
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/assets PREFIX Assets FILES ${ASSETS} ${SHADERS})
source_group("Assets\\shaders" FILES ${EXTERNAL_SHADERS})
