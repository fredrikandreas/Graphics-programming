# Create GLFWApplication library
add_library(GLFWApplication GLFWApplication.cpp)

# Create alias so it can be used as Framework::GLFWApplication
add_library(Framework::GLFWApplication ALIAS GLFWApplication)

# Provide include path to others
target_include_directories(GLFWApplication PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link dependencies like GLFW & GLAD
target_link_libraries(GLFWApplication PUBLIC glad glfw)