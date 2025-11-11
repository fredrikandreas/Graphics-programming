#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class InputController
{
public:
    static void install(GLFWwindow *window);

    static void update(float dt, glm::ivec2 &selectedTile);
    static float angleX(); // degrees
    static float angleY(); // degrees

    static void selectTile(const glm::ivec2 &tile);
    static glm::ivec2 getSelectedTile();

private:
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static float s_angleX, s_angleY;
    static float s_angVelX, s_angVelY;
    static constexpr float s_rotationSpeed = 90.0f; // deg/s per key
    static glm::ivec2 s_selectedTile;
};
