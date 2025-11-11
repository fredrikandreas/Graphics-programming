#include <GLFW/glfw3.h>

class InputController
{
public:
    static void install(GLFWwindow *window);

    static void update(float dt);
    static float angleX(); // degrees
    static float angleY(); // degrees

private:
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static float s_angleX, s_angleY;
    static float s_angVelX, s_angVelY;
    static constexpr float s_rotationSpeed = 90.0f; // deg/s per key
};
