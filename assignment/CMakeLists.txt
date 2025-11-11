project(assignment)

add_executable(${PROJECT_NAME} main.cpp BoardRenderer.cpp GameCamera.cpp InputController.cpp PieceRenderer.cpp shaders.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)
target_link_libraries(${PROJECT_NAME} PRIVATE GLFWApplication)
target_link_libraries(${PROJECT_NAME} PRIVATE GeometricTools)
target_link_libraries(${PROJECT_NAME} PRIVATE Rendering)
target_link_libraries(${PROJECT_NAME} PRIVATE Shader)
target_link_libraries(${PROJECT_NAME} PRIVATE stb)

add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/cube_texture.png
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/cube_texture.png)

  add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/floor_texture.png
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/floor_texture.png)

target_compile_definitions(${PROJECT_NAME} PRIVATE
  TEXTURES_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/")

target_compile_definitions(${PROJECT_NAME} PRIVATE STB_IMAGE_IMPLEMENTATION)
target_compile_definitions(${PROJECT_NAME} PRIVATE STB_IMAGE_STATIC)
