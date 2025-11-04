#include "GLFWApplication.h"

// Init function (can be extended in subclasses)
GLFWwindow *GLFWApplication::Init()
{
    // 1. Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return nullptr;
    }

    // 2. Setup OpenGL context version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // 3. Create window
    GLFWwindow *window = glfwCreateWindow(width, height, appName.c_str(), nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    // 4. Initialize GLAD (loads OpenGL functions)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    return window;
}

void GLFWApplication::Poll()
{
    glfwPollEvents();
}

void GLFWApplication::Swap(GLFWwindow *window)
{
    glfwSwapBuffers(window);
}

void GLFWApplication::Destroy(GLFWwindow *window)
{
    glfwDestroyWindow(window);
}

GLFWApplication::~GLFWApplication()
{
    glfwTerminate();
};