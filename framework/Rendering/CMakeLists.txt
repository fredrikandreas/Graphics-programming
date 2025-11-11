project(Rendering)
# Create GLFWApplication library
add_library(Rendering VertexBuffer.cpp IndexBuffer.cpp VertexArray.cpp TextureManager.cpp OrthographicCamera.cpp PerspectiveCamera.cpp)

# Create alias so it can be used as Framework::GLFWApplication
add_library(Framework::Rendering ALIAS Rendering)

# Provide include path to others
target_include_directories(Rendering PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link dependencies like GLFW & GLAD
target_link_libraries(Rendering PUBLIC glad glfw glm stb)

target_compile_definitions(${PROJECT_NAME} PRIVATE STB_IMAGE_IMPLEMENTATION)
target_compile_definitions(${PROJECT_NAME} PRIVATE STB_IMAGE_STATIC)