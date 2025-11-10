#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <GLFWApplication.h>
#include <GeometricTools.h>
#include <VertexBuffer.h>
#include <IndexBuffer.h>
#include <VertexArray.h>

#include <iostream>
#include <vector>
#include <memory>

#include "shaders.h"

const char *APP_NAME = "Lab 2";
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int V_MAJOR = 4;
const int V_MINOR = 1;

const int GRID_ROWS = 8;
const int GRID_COLS = 8;

struct GLFWwindow;

// Compile shader helper
GLuint CompileShader(GLenum type, const char *src)
{
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, nullptr);
    glCompileShader(s);
    return s;
}

int main(int argc, char **argv)
{
    GLFWApplication app(APP_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, V_MAJOR, V_MINOR);
    GLFWwindow *window = app.Init();

    // Create shader program
    GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShaderSrc);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);
    GLuint prog = glCreateProgram();

    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);

    glDeleteShader(vs);
    glDeleteShader(fs);

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

    glUseProgram(prog);
    glUniform1i(glGetUniformLocation(prog, "uCols"), GRID_COLS);
    glUniform1i(glGetUniformLocation(prog, "uRows"), GRID_ROWS);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(prog);

        vertexArray->Bind();

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(gridTopology.size()), GL_UNSIGNED_INT, 0);

        app.Swap(window);
        app.Poll();
    }

    // Cleanup
    glDeleteProgram(prog);
    app.Destroy(window);

    return 0;
}
