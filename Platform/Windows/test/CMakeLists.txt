add_executable(TextureLoadTest TextureLoadTest.cpp)
target_link_libraries(TextureLoadTest WindowsApplication Common D2dRHI d2d1)
target_link_libraries(TextureLoadTest 
            debug  ${XG_LIBRARY_DEBUG} 
            debug  ${TINYGLTF_LIBRARY_DEBUG}
            )
target_link_libraries(TextureLoadTest 
            optimized  ${XG_LIBRARY_RELEASE} 
            optimized  ${TINYGLTF_LIBRARY_RELEASE}
            )