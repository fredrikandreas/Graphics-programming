add_executable(lab4 main.cpp)

target_include_directories(lab4 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(lab4 PRIVATE glfw)
target_link_libraries(lab4 PRIVATE glad)
target_link_libraries(lab4 PRIVATE glm)
target_link_libraries(lab4 PRIVATE GLFWApplication)
target_link_libraries(lab4 PRIVATE GeometricTools)
target_link_libraries(lab4 PRIVATE Rendering)
target_link_libraries(lab4 PRIVATE Shader)