#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

class GLFWApplication
{
private:
    std::string appName;
    int width;
    int height;
    int versionMajor;
    int versionMinor;

public:
    // Constructor and Destructor
    GLFWApplication(const std::string name, const int w, const int h, const int major, const int minor) : appName(name), width(w), height(h), versionMajor(major), versionMinor(minor) {};
    ~GLFWApplication();

    // Initialization
    virtual GLFWwindow *Init(); // Virtual function with default behavior.

    // Run function
    // virtual unsigned Run() const = 0; // Pure virtual function that must be redefined.

    // Poll events
    void Poll();

    // Swap buffers
    void Swap(GLFWwindow *window);

    // Destroy window
    void Destroy(GLFWwindow *window);
};
