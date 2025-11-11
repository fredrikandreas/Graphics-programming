#include "include/InputController.h"
#include <cmath>

float InputController::s_angleX = 0.0f;
float InputController::s_angleY = 0.0f;
float InputController::s_angVelX = 0.0f;
float InputController::s_angVelY = 0.0f;

glm::ivec2 InputController::s_selectedTile = glm::ivec2(0, 0);

PieceRenderer *InputController::s_renderer = nullptr;

float InputController::s_azimuthDeg = 45.0f;
float InputController::s_radius = 7.0f;

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
        case GLFW_KEY_ENTER:
            s_renderer->selectOrMove(s_selectedTile);
        case GLFW_KEY_H:
            s_azimuthDeg = wrap360(s_azimuthDeg - s_orbitStep);
            break;
        case GLFW_KEY_L:
            s_azimuthDeg = wrap360(s_azimuthDeg + s_orbitStep);
            break;

        case GLFW_KEY_O:
            s_radius = std::min(s_radius + s_radiusStep, s_radiusMax);
            break;
        case GLFW_KEY_P:
            s_radius = std::max(s_radius - s_radiusStep, s_radiusMin);
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

void InputController::setPieceRenderer(PieceRenderer *renderer)
{
    s_renderer = renderer;
}

float InputController::azimuthDeg() { return s_azimuthDeg; }
float InputController::radius() { return s_radius; }