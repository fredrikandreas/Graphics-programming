#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFWApplication.h>
#include <GeometricTools.h>
#include <VertexBuffer.h>
#include <IndexBuffer.h>
#include <VertexArray.h>
#include <Shader.h>
#include <RenderCommands.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <TextureManager.h>
#include <Camera.h>
#include <PerspectiveCamera.h>
#include <OrthographicCamera.h>
#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"

#include "GameCamera.h"
#include "BoardRenderer.h"
#include "PieceRenderer.h"
#include "InputController.h"

static const char *APP_NAME = "Assignment";
static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;
static const int GL_MAJOR = 4;
static const int GL_MINOR = 1;

glm::ivec2 selectedTile = {0, 0};

int main(int, char **)
{
    GLFWApplication app(APP_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, GL_MAJOR, GL_MINOR);
    GLFWwindow *window = app.Init();
    glfwSwapInterval(1);

    // GL state
    RenderCommands::EnableDepthTesting();
    int fbw, fbh;
    glfwGetFramebufferSize(window, &fbw, &fbh);
    RenderCommands::SetViewport(0, 0, fbw, fbh);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    RenderCommands::SetClearColor({0.2f, 0.2f, 0.2f, 1.0f});

    // Input
    InputController::install(window);

    // Cameras
    GameCamera cameras;
    auto &cam = cameras.perspective();

    // Renderers
    BoardRenderer board(8, 8);
    board.init(cam);

    PieceRenderer pieces;
    pieces.init(cam, board, cameras.eye());

    InputController::setPieceRenderer(&pieces);

    // Loop
    double last = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        double now = glfwGetTime();
        float dt = static_cast<float>(now - last);
        last = now;

        InputController::update(dt, selectedTile);

        glm::vec3 boardCenter(0.0f, 0.0f, 0.0f);

        // Orbit parameters from input

        // orbit camera values
        float azimuth = glm::radians(InputController::azimuthDeg());
        float radius = InputController::radius();
        float elevation = glm::radians(35.0f); // fixed elevation angle

        // spherical orbit
        float rc = radius * std::cos(elevation);
        glm::vec3 eye(
            boardCenter.x + rc * std::cos(azimuth),
            boardCenter.y + radius * std::sin(elevation),
            boardCenter.z + rc * std::sin(azimuth));

        std::cout<<glm::to_string(eye)<<std::endl;


        // Apply orbit to your PerspectiveCamera
        cam.SetPosition(eye);
        cam.SetLookAt(boardCenter);
        cam.SetUpVector(glm::vec3(0.0f, 1.0f, 0.0f));

        RenderCommands::Clear();
        RenderCommands::SetSolidMode();

        board.setAmbient(1.0f);
        board.setSelectedTile(selectedTile);
        board.draw();

        pieces.draw(static_cast<float>(now), selectedTile);

        app.Swap(window);
        app.Poll();
    }

    app.Destroy(window);
    return 0;
}
