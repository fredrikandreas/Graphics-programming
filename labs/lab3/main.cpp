#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFWApplication.h>
#include <GeometricTools.h>
#include <VertexBuffer.h>
#include <IndexBuffer.h>
#include <VertexArray.h>
#include <Shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include <memory>

#include "shaders.h"

const char *APP_NAME = "Lab 3";
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int V_MAJOR = 4;
const int V_MINOR = 1;

const int GRID_ROWS = 8;
const int GRID_COLS = 8;

glm::ivec2 selectedTile = {0, 0};

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
        case GLFW_KEY_UP:
            selectedTile.y = glm::clamp(selectedTile.y - 1, 0, GRID_ROWS - 1);
            break;
        case GLFW_KEY_DOWN:
            selectedTile.y = glm::clamp(selectedTile.y + 1, 0, GRID_ROWS - 1);
            break;
        case GLFW_KEY_LEFT:
            selectedTile.x = glm::clamp(selectedTile.x - 1, 0, GRID_COLS - 1);
            break;
        case GLFW_KEY_RIGHT:
            selectedTile.x = glm::clamp(selectedTile.x + 1, 0, GRID_COLS - 1);
            break;
        }
    }
}

int main(int argc, char **argv)
{
    GLFWApplication app(APP_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, V_MAJOR, V_MINOR);
    GLFWwindow *window = app.Init();

    app.SetKeyCallback(window, key_callback);

    GeometricTools::UnitGridGeometry2D<GRID_COLS, GRID_ROWS> geometry;
    GeometricTools::UnitGridTopologyTriangles<GRID_COLS, GRID_ROWS> topology;
    const auto &gridGeometry = geometry.GetGrid();
    const auto &gridTopology = topology.GetIndices();

    // Create buffers and arrays
    auto gridIndexBuffer = std::make_shared<IndexBuffer>(gridTopology.data(), gridTopology.size());
    auto gridBufferLayout = BufferLayout({{ShaderDataType::Float2, "position"}});
    auto gridVertexBuffer = std::make_shared<VertexBuffer>(gridGeometry.data(), gridGeometry.size() * sizeof(gridGeometry[0]));
    gridVertexBuffer->SetLayout(gridBufferLayout);
    auto vertexArray = std::make_shared<VertexArray>();
    vertexArray->AddVertexBuffer(gridVertexBuffer);
    vertexArray->SetIndexBuffer(gridIndexBuffer);

    glm::mat4 projectionMatrix = glm::perspective(45.0f, 1.0f, 1.0f, -10.0f);
    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f),
                                       glm::vec3(0.0f, 0.0f, 0.0f),
                                       glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(4.0f, 4.0f, 1.0f));
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 2.0f));

    glm::mat4 chessboardModelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

    auto chessboardShader = std::make_shared<Shader>(vertexShaderSrc, fragmentShaderSrc);
    chessboardShader->UploadUniformMat4("u_projection", projectionMatrix);
    chessboardShader->UploadUniformMat4("u_view", viewMatrix);
    chessboardShader->UploadUniformMat4("u_model", chessboardModelMatrix);

    auto program = chessboardShader->GetProgramID();
    glUniform1i(glGetUniformLocation(program, "uCols"), GRID_COLS);
    glUniform1i(glGetUniformLocation(program, "uRows"), GRID_ROWS);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        chessboardShader->Bind();
        chessboardShader->UploadUniformFloat2("uSelectedTile", glm::vec2(selectedTile));

        vertexArray->Bind();

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(gridTopology.size()), GL_UNSIGNED_INT, 0);

        app.Swap(window);
        app.Poll();
    }

    // Cleanup
    app.Destroy(window);

    return 0;
}
