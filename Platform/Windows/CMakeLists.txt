add_library(WindowsApplication WindowsApplication.cpp)

add_executable(CoronaEngineD3d12 D3d12Application.cpp)
target_link_libraries(CoronaEngineD3d12 WindowsApplication Common D3dRHI d3d12 dxgi d3dcompiler)

add_subdirectory(test)
