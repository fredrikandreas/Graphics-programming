add_executable(lab3 main.cpp)

target_include_directories(lab3 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(lab3 PRIVATE glfw)
target_link_libraries(lab3 PRIVATE glad)
target_link_libraries(lab3 PRIVATE glm)
target_link_libraries(lab3 PRIVATE GLFWApplication)
target_link_libraries(lab3 PRIVATE GeometricTools)
target_link_libraries(lab3 PRIVATE Rendering)
target_link_libraries(lab3 PRIVATE Shader)