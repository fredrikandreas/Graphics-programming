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

#include <cmath>
#include <iostream>
#include <vector>
#include <memory>
#include <type_traits>

#include "shaders.h"

const char *APP_NAME = "Lab 4";
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int V_MAJOR = 4;
const int V_MINOR = 1;

const int GRID_ROWS = 8;
const int GRID_COLS = 8;

glm::ivec2 selectedTile = {0, 0};
float angle_x = 0.0f;
float angle_y = 0.0f;
float rotationSpeed = 90.0f;
float angVelX = 0.0f;
float angVelY = 0.0f;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    /*
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
        case GLFW_KEY_UP:
            // selectedTile.y = glm::clamp(selectedTile.y - 1, 0, GRID_ROWS - 1);
            angle_y -= 20.0f;
            if (angle_y < 0.0f)
            {
                angle_y = 360.0f;
            }
            break;
        case GLFW_KEY_DOWN:
            // selectedTile.y = glm::clamp(selectedTile.y + 1, 0, GRID_ROWS - 1);
            angle_y += 20.0f;
            if (angle_y > 360.0f)
            {
                angle_y = 0.0f;
            }
            break;
        case GLFW_KEY_LEFT:
            // selectedTile.x = glm::clamp(selectedTile.x - 1, 0, GRID_COLS - 1);
            angle_x -= 20.0f;
            if (angle_x < 0.0f)
            {
                angle_x = 360.0f;
            }
            break;
        case GLFW_KEY_RIGHT:
            // selectedTile.x = glm::clamp(selectedTile.x + 1, 0, GRID_COLS - 1);
            angle_x += 20.0f;
            if (angle_x > 360.0f)
            {
                angle_x = 0.0f;
            }
            break;
        }
    }
    */
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_UP:
            angVelY -= rotationSpeed;
            break; // rotate around X-ax in your model
        case GLFW_KEY_DOWN:
            angVelY += rotationSpeed;
            break;
        case GLFW_KEY_LEFT:
            angVelX -= rotationSpeed;
            break; // rotate around Y-ax in your model
        case GLFW_KEY_RIGHT:
            angVelX += rotationSpeed;
            break;
        default:
            break;
        }
    }
    else if (action == GLFW_RELEASE)
    {
        // Revert the velocity change for the released key
        switch (key)
        {
        case GLFW_KEY_UP:
            angVelY += rotationSpeed;
            break;
        case GLFW_KEY_DOWN:
            angVelY -= rotationSpeed;
            break;
        case GLFW_KEY_LEFT:
            angVelX += rotationSpeed;
            break;
        case GLFW_KEY_RIGHT:
            angVelX -= rotationSpeed;
            break;
        default:
            break;
        }
    }
}

int main(int argc, char **argv)
{
    GLFWApplication app(APP_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, V_MAJOR, V_MINOR);
    GLFWwindow *window = app.Init();
    glfwSwapInterval(1);
    RenderCommands::EnableDepthTesting();
    RenderCommands::SetClearColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
    int fbw, fbh;
    glfwGetFramebufferSize(window, &fbw, &fbh);
    RenderCommands::SetViewport(0, 0, fbw, fbh);
    app.SetKeyCallback(window, key_callback);

    // Loading textures
    TextureManager *textureManager = TextureManager::GetInstance();
    textureManager->LoadTexture2DRGBA("floor", TEXTURES_DIR + std::string("cube_top.png"), 0);
    textureManager->LoadCubeMapCustomRGBA("cubeMapCustom",
                                          std::array<std::string, 6>{
                                              TEXTURES_DIR + std::string("cube_sides.png"),  // Right
                                              TEXTURES_DIR + std::string("cube_sides.png"),  // Left
                                              TEXTURES_DIR + std::string("cube_top.png"),    // Top
                                              TEXTURES_DIR + std::string("cube_bottom.png"), // Bottom
                                              TEXTURES_DIR + std::string("cube_sides.png"),  // Back
                                              TEXTURES_DIR + std::string("cube_sides.png")}, // Front
                                          1);

    // Grid / Chessboard
    GeometricTools::UnitGridGeometry2DWTCoords<GRID_COLS, GRID_ROWS> grid_g;
    GeometricTools::UnitGridTopologyTriangles<GRID_COLS, GRID_ROWS> grid_t;
    const auto &gridGeometry = grid_g.GetGrid();
    const auto &gridTopology = grid_t.GetIndices();

    auto gridIndexBuffer = std::make_shared<IndexBuffer>(gridTopology.data(), gridTopology.size());
    auto gridBufferLayout = BufferLayout({{ShaderDataType::Float2, "a_inPosition"},
                                          {ShaderDataType::Float2, "a_inTexCoord"}});

    auto gridVertexBuffer = std::make_shared<VertexBuffer>(gridGeometry.data(), gridGeometry.size() * sizeof(gridGeometry[0]));
    gridVertexBuffer->SetLayout(gridBufferLayout);
    auto gridVertexArray = std::make_shared<VertexArray>();
    gridVertexArray->AddVertexBuffer(gridVertexBuffer);
    gridVertexArray->SetIndexBuffer(gridIndexBuffer);

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 1.0f, 1.0f, 10.0f);
    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 1.0f, 5.0f),
                                       glm::vec3(0.0f, 0.0f, 0.0f),
                                       glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 gridScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(4.0f, 4.0f, 1.0f));
    glm::mat4 gridRotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 gridTranslationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, -0.5f, -1.25f));

    glm::mat4 chessboardModelMatrix = gridTranslationMatrix * gridRotationMatrix * gridScaleMatrix;

    auto chessboardShader = std::make_shared<Shader>(vertexShaderSrc, fragmentShaderSrc);
    chessboardShader->Bind();
    gridVertexBuffer->Bind();
    gridIndexBuffer->Bind();
    chessboardShader->UploadUniformMat4("u_projection", projectionMatrix);
    chessboardShader->UploadUniformMat4("u_view", viewMatrix);
    chessboardShader->UploadUniformMat4("u_model", chessboardModelMatrix);
    chessboardShader->UploadUniformFloat2("u_gridSize", glm::vec2(GRID_COLS, GRID_ROWS));
    chessboardShader->UploadUniformInt("u_floorTextureSampler", 0);

    gridVertexArray->Unbind();
    chessboardShader->Unbind();

    // Cube
    GeometricTools::UnitCubeGeometry3D cube_g;
    GeometricTools::UnitCubeTopologyTriangles cube_t;
    const auto &cubeGeometry = cube_g.GetVertices();
    const auto &cubeTopology = cube_t.GetIndices();

    auto cubeIndexBuffer = std::make_shared<IndexBuffer>(cubeTopology.data(), cubeTopology.size());
    auto cubeBufferLayout = BufferLayout({{ShaderDataType::Float3, "i_position"}});
    auto cubeVertexBuffer = std::make_shared<VertexBuffer>(cubeGeometry.data(), cubeGeometry.size() * sizeof(cubeGeometry[0]));
    cubeVertexBuffer->SetLayout(cubeBufferLayout);
    auto cubeVertexArray = std::make_shared<VertexArray>();
    cubeVertexArray->AddVertexBuffer(cubeVertexBuffer);
    cubeVertexArray->SetIndexBuffer(cubeIndexBuffer);

    auto cubeShader = std::make_shared<Shader>(cubeVertexShaderSrc, cubeFragmentShaderSrc);
    cubeShader->UploadUniformMat4("u_projection", projectionMatrix);
    cubeShader->UploadUniformMat4("u_view", viewMatrix);
    cubeShader->UploadUniformInt("u_cubeTextureSampler", 1);

    cubeVertexArray->Unbind();
    cubeShader->Unbind();

    double lastTime = glfwGetTime();

    // Enable blending
    glEnable(GL_BLEND);
    // Set the blending function: s*alpha + d(1-alpha)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        double now = glfwGetTime();
        float dt = static_cast<float>(now - lastTime);
        lastTime = now;

        angle_x += angVelX * dt;
        angle_y += angVelY * dt;

        auto wrap360 = [](float a)
        {
            a = std::fmod(a, 360.0f);
            return (a < 0.0f) ? a + 360.0f : a;
        };
        angle_x = wrap360(angle_x);
        angle_y = wrap360(angle_y);

        RenderCommands::Clear();
        RenderCommands::SetSolidMode();

        // Draw grid/ chessboard
        chessboardShader->Bind();
        gridVertexArray->Bind();
        chessboardShader->UploadUniformFloat2("u_SelectedTile", glm::vec2(selectedTile));
        RenderCommands::DrawIndex(gridVertexArray, GL_TRIANGLES);

        // Draw cube
        cubeShader->Bind();
        cubeVertexArray->Bind();
        glm::mat4 cubeRotationX = glm::rotate(glm::mat4(1.0f), glm::radians(angle_x), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 cubeRotationY = glm::rotate(glm::mat4(1.0f), glm::radians(angle_y), glm::vec3(1.0f, 0.0f, 0.0f));
        cubeShader->UploadUniformMat4("u_model", cubeRotationX * cubeRotationY);
        RenderCommands::DrawIndex(cubeVertexArray, GL_TRIANGLES);

        app.Swap(window);
        app.Poll();
    }

    // Cleanup
    app.Destroy(window);

    return 0;
}
