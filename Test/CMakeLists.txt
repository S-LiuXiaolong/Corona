add_executable(AssetLoaderTest AssetLoaderTest.cpp)
target_link_libraries(AssetLoaderTest Common)

add_executable(GeomMathTest GeomMathTest.cpp)
target_link_libraries(GeomMathTest GeomMath)

add_executable(OgexParserTest OgexParserTest.cpp)
target_link_libraries(OgexParserTest Common)
# target_link_libraries(OgexParserTest 
#             debug  ${XG_LIBRARY_DEBUG} ${OPENGEX_LIBRARY_DEBUG} ${OPENDDL_LIBRARY_DEBUG})
# target_link_libraries(OgexParserTest 
#             optimized  ${XG_LIBRARY_RELEASE} ${OPENGEX_LIBRARY_RELEASE} ${OPENDDL_LIBRARY_RELEASE})
# __add_xg_platform_dependencies(OgexParserTest)
target_link_libraries(OgexParserTest 
            debug  ${XG_LIBRARY_DEBUG} 
            debug  ${OPENGEX_LIBRARY_DEBUG} 
            debug  ${OPENDDL_LIBRARY_DEBUG}
            debug  ${TINYGLTF_LIBRARY_DEBUG}
            )
target_link_libraries(OgexParserTest 
            optimized  ${XG_LIBRARY_RELEASE} 
            optimized  ${OPENGEX_LIBRARY_RELEASE} 
            optimized  ${OPENDDL_LIBRARY_RELEASE}
            optimized  ${TINYGLTF_LIBRARY_RELEASE}
            )

add_executable(SceneObjectTest SceneObjectTest.cpp)
target_link_libraries(SceneObjectTest Common)
target_link_libraries(SceneObjectTest 
            debug ${XG_LIBRARY_DEBUG})
target_link_libraries(SceneObjectTest 
            optimized ${XG_LIBRARY_RELEASE})
# __add_xg_platform_dependencies(SceneObjectTest)

add_executable(tinygltfTest tinygltfTest.cpp)
target_link_libraries(tinygltfTest Common)
target_link_libraries(tinygltfTest 
            debug ${TINYGLTF_LIBRARY_DEBUG})
target_link_libraries(tinygltfTest 
            optimized ${TINYGLTF_LIBRARY_RELEASE})