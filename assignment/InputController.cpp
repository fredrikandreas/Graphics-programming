#include "include/InputController.h"
#include <cmath>

float InputController::s_angleX = 0.0f;
float InputController::s_angleY = 0.0f;
float InputController::s_angVelX = 0.0f;
float InputController::s_angVelY = 0.0f;

glm::ivec2 InputController::s_selectedTile = glm::ivec2(0, 0);

static float wrap360(float a)
{
    a = std::fmod(a, 360.0f);
    return (a < 0.0f) ? a + 360.0f : a;
}

void InputController::install(GLFWwindow *window)
{
    glfwSetKeyCallback(window, &InputController::keyCallback);
}

void InputController::update(float dt, glm::ivec2 &selectedTile)
{
    s_angleX = wrap360(s_angleX + s_angVelX * dt);
    s_angleY = wrap360(s_angleY + s_angVelY * dt);

    selectedTile = s_selectedTile;
}

float InputController::angleX() { return s_angleX; }
float InputController::angleY() { return s_angleY; }

void InputController::keyCallback(GLFWwindow *window, int key, int, int action, int)
{
    /*
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_UP:
            s_angVelY -= s_rotationSpeed;
            break;
        case GLFW_KEY_DOWN:
            s_angVelY += s_rotationSpeed;
            break;
        case GLFW_KEY_LEFT:
            s_angVelX -= s_rotationSpeed;
            break;
        case GLFW_KEY_RIGHT:
            s_angVelX += s_rotationSpeed;
            break;
        case GLFW_KEY_Q:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        default:
            break;
        }
    }
    else if (action == GLFW_RELEASE)
    {
        switch (key)
        {
        case GLFW_KEY_UP:
            s_angVelY += s_rotationSpeed;
            break;
        case GLFW_KEY_DOWN:
            s_angVelY -= s_rotationSpeed;
            break;
        case GLFW_KEY_LEFT:
            s_angVelX += s_rotationSpeed;
            break;
        case GLFW_KEY_RIGHT:
            s_angVelX -= s_rotationSpeed;
            break;
        default:
            break;
        }
    }
    */
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
        case GLFW_KEY_Q:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        case GLFW_KEY_UP:
            s_selectedTile.y = glm::clamp(s_selectedTile.y - 1, 0, 7);
            break;
        case GLFW_KEY_DOWN:
            s_selectedTile.y = glm::clamp(s_selectedTile.y + 1, 0, 7);
            break;
        case GLFW_KEY_LEFT:
            s_selectedTile.x = glm::clamp(s_selectedTile.x - 1, 0, 7);
            break;
        case GLFW_KEY_RIGHT:
            s_selectedTile.x = glm::clamp(s_selectedTile.x + 1, 0, 7);
            break;
        }
    }
}

void InputController::selectTile(const glm::ivec2 &tile)
{
    s_selectedTile.x = glm::clamp(tile.x, 0, 7);
    s_selectedTile.y = glm::clamp(tile.y, 0, 7);
}

glm::ivec2 InputController::getSelectedTile()
{
    return s_selectedTile;
}
