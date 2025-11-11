#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "PieceRenderer.h"

class InputController
{
public:
    static void install(GLFWwindow *window);

    static void update(float dt, glm::ivec2 &selectedTile);
    static float angleX(); // degrees
    static float angleY(); // degrees

    static float azimuthDeg();
    static float radius(); 

    static void setPieceRenderer(PieceRenderer* renderer); 
    static void selectTile(const glm::ivec2 &tile);
    static glm::ivec2 getSelectedTile();

private:
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static float s_angleX, s_angleY;
    static float s_angVelX, s_angVelY;
    static constexpr float s_rotationSpeed = 90.0f; // deg/s per key
    static glm::ivec2 s_selectedTile;

    static float s_azimuthDeg;           // around Y axis
    static float s_radius;               // camera distance
    static constexpr float s_orbitStep = 45.0f; // deg per key press
    static constexpr float s_radiusMin = 3.5f;  // sensible near limit
    static constexpr float s_radiusMax = 12.0f; // sensible far limit
    static constexpr float s_radiusStep = 0.5f; // per key press

    static PieceRenderer* s_renderer;  
};
