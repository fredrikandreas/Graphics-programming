# Create GLFWApplication library
add_library(Rendering ShadersDataType.h VertexBuffer.h)

# Create alias so it can be used as Framework::GLFWApplication
add_library(Framework::Rendering ALIAS Rendering)

# Provide include path to others
target_include_directories(Rendering PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link dependencies like GLFW & GLAD
target_link_libraries(Rendering PUBLIC glad glfw glm)