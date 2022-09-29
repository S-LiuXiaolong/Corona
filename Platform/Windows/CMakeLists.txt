add_library(WindowsApplication WindowsApplication.cpp)

add_executable(MyGameEngineD3d12 D3d12Application.cpp)
target_link_libraries(MyGameEngineD3d12 WindowsApplication Common D3dRHI d3d12 dxgi d3dcompiler)

add_subdirectory(test)

