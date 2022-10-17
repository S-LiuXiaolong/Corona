add_executable(AssetLoaderTest AssetLoaderTest.cpp)
target_link_libraries(AssetLoaderTest Common)

add_executable(GeomMathTest GeomMathTest.cpp)
target_link_libraries(GeomMathTest GeomMath)

add_executable(tinygltfTest tinygltfTest.cpp)
target_link_libraries(tinygltfTest Common)

add_executable(gltfSceneManagerTest gltfSceneManagerTest.cpp)
target_link_libraries(gltfSceneManagerTest Common)

add_executable(JpegParserTest JpegParserTest.cpp)
target_link_libraries(JpegParserTest Common)

add_executable(LodePngTest LodePngTest.cpp)
target_link_libraries(LodePngTest Common)

add_executable(LibJpegTest LibJpegTest.cpp)
target_link_libraries(LibJpegTest Common)

# add_executable(SceneObjectTest SceneObjectTest.cpp)
# target_link_libraries(SceneObjectTest Common)

add_executable(SceneLoadingTest SceneLoadingTest.cpp)
target_link_libraries(SceneLoadingTest Common)
