add_executable(AssetLoaderTest AssetLoaderTest.cpp)
target_link_libraries(AssetLoaderTest Common)
target_link_libraries(AssetLoaderTest 
            debug  ${XG_LIBRARY_DEBUG} 
            debug  ${TINYGLTF_LIBRARY_DEBUG}
            )
target_link_libraries(AssetLoaderTest 
            optimized  ${XG_LIBRARY_RELEASE} 
            optimized  ${TINYGLTF_LIBRARY_RELEASE}
            )

add_executable(GeomMathTest GeomMathTest.cpp)
target_link_libraries(GeomMathTest GeomMath)

add_executable(tinygltfTest tinygltfTest.cpp)
target_link_libraries(tinygltfTest Common)
target_link_libraries(tinygltfTest 
            debug ${TINYGLTF_LIBRARY_DEBUG})
target_link_libraries(tinygltfTest 
            optimized ${TINYGLTF_LIBRARY_RELEASE})

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

add_executable(JpegParserTest JpegParserTest.cpp)
target_link_libraries(JpegParserTest Common)
add_test(NAME TEST_JpegParser COMMAND JpegParserTest)