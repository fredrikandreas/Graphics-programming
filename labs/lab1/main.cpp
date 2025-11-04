// #include area (SECTION 1)
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

struct GLFWwindow;

int main(int argc, char** argv)
{
    std::cout << "Hello World!" << std::endl;
    
    // GLFW initialization code (SECTION 2)
    if (!glfwInit())
    {
        std::cout << "Failed to init GLFW" << std::endl;
        return -1;
    }

    // OpenGL initialization code (SECTION 3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // OpenGL data transfer code (SECTION 4)
    GLFWwindow* window = glfwCreateWindow(800, 800, "Lab 2", nullptr, nullptr);
    if (!window) 
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Application loop code (SECTION 5)
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }

    // Termination code (SECTION 6)
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
