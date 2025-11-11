#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <GLFWApplication.h>

class Lab4Application : public GLFWApplication
{
public:
    Lab4Application(const std::string name, const int w, const int h, const int major, const int minor)
        : GLFWApplication(name, w, h, major, minor), appName(name), width(w), height(h), versionMajor(major), versionMinor(minor) {}

    GLuint LoadTexture(const std::string &filepath, GLuint slot);
    GLuint LoadCubeMap(const std::array<std::string,3> &filepaths, GLuint slot);

private:
    std::string appName;
    int width;
    int height;
    int versionMajor;
    int versionMinor;
};