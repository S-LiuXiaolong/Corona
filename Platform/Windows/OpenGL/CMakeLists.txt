find_package(OpenGL REQUIRED)

add_executable(MyGameEngineOpenGL 
    main.cpp
    glad.c
    )

target_link_libraries(MyGameEngineOpenGL Common ${OPENGL_LIBRARIES})
target_link_libraries(MyGameEngineOpenGL 
            debug ${GLFW_LIBRARY_DEBUG}
            )
target_link_libraries(MyGameEngineOpenGL 
            optimized ${GLFW_LIBRARY_RELEASE}
            )