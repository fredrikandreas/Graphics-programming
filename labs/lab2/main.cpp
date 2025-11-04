#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFWApplication.h>

#include <iostream>

char *APP_NAME = "Lab 2";
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int V_MAJOR = 4;
const int V_MINOR = 1;

int main(int argc, char const *argv[])
{
    GLFWApplication app = GLFWApplication(APP_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, V_MAJOR, V_MINOR);
    GLFWwindow *window = app.Init();
    
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        app.Swap(window);
        app.Poll();
    }

    app.Destroy(window);

    return 0;
}
