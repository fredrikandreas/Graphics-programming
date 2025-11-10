# Create GLFWApplication library
add_library(Shader Shader.cpp)

# Create alias so it can be used as Framework::GLFWApplication
add_library(Framework::Shader ALIAS Shader)

# Provide include path to others
target_include_directories(Shader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link dependencies like GLFW & GLAD
target_link_libraries(Shader PUBLIC glad glfw glm)