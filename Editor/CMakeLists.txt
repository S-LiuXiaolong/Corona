add_executable(CoronaEditor
    EditorLogic.cpp EditorConfig.cpp)

IF(WIN32)
    target_link_libraries(CoronaEditor
        CoronaEngineD3d12
    )
ENDIF(WIN32)