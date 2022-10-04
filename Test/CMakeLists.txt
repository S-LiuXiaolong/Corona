add_executable(AssetLoaderTest AssetLoaderTest.cpp)
target_link_libraries(AssetLoaderTest Common)

add_executable(GeomMathTest GeomMathTest.cpp)
target_link_libraries(GeomMathTest GeomMath)

add_executable(tinygltfTest tinygltfTest.cpp)
target_link_libraries(tinygltfTest Common)
target_link_libraries(tinygltfTest 
            debug ${TINYGLTF_LIBRARY_DEBUG})
target_link_libraries(tinygltfTest 
            optimized ${TINYGLTF_LIBRARY_RELEASE})

add_executable(mygltfTest mygltfTest.cpp)
target_link_libraries(mygltfTest Common)
target_link_libraries(mygltfTest 
            debug  ${XG_LIBRARY_DEBUG} 
            debug  ${TINYGLTF_LIBRARY_DEBUG}
            )
target_link_libraries(mygltfTest 
            optimized  ${XG_LIBRARY_RELEASE} 
            optimized  ${TINYGLTF_LIBRARY_RELEASE}
            )

add_executable(gltfSceneManagerTest gltfSceneManagerTest.cpp)
target_link_libraries(gltfSceneManagerTest Common)
target_link_libraries(gltfSceneManagerTest 
            debug  ${XG_LIBRARY_DEBUG} 
            debug  ${TINYGLTF_LIBRARY_DEBUG}
            )
target_link_libraries(gltfSceneManagerTest 
            optimized  ${XG_LIBRARY_RELEASE} 
            optimized  ${TINYGLTF_LIBRARY_RELEASE}
            )