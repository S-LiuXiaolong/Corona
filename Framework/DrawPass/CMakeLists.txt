add_library(
    ForwardRenderPass.cpp
    HUDPass.cpp
    ShadowMapPass.cpp
)
target_include_directories(DrawPass PUBLIC GeomMath)
target_link_libraries(DrawPass PUBLIC GeomMath)