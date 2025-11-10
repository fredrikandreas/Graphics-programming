#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <GLFWApplication.h>
#include <GeometricTools.h>
#include <VertexBuffer.h>

#include <iostream>
#include <vector>

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

    // Generate positions + UVs for a (DivX+1) x (DivY+1) grid
    // layout: [pos.x, pos.y, uv.x, uv.y]
    std::vector<float> verts;
    verts.reserve((GRID_COLS + 1) * (GRID_ROWS + 1) * 4);

    for (int y = 0; y <= GRID_ROWS; ++y)
    {
        float v = float(y) / float(GRID_ROWS); // 0..1
        float py = -1.0f + 2.0f * v;           // -1..1
        for (int x = 0; x <= GRID_COLS; ++x)
        {
            float u = float(x) / float(GRID_COLS); // 0..1
            float px = -1.0f + 2.0f * u;           // -1..1
            verts.push_back(px);
            verts.push_back(py);
            verts.push_back(u);
            verts.push_back(v);
        }
    }

    using Topo = GeometricTools::UnitGridTopologyTriangles<GRID_COLS, GRID_ROWS>;
    Topo topology;

    const auto& idx = topology.GetIndices();
    const GLsizei indexCount = static_cast<GLsizei>(idx.size());

    // Upload to GPU
    GLuint vao = 0, ebo = 0;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);

    VertexBuffer vbo(verts.data(), static_cast<GLsizei>(verts.size()));

    vbo.Bind();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

    // aPos (location=0) -> 2 floats, stride 4 floats
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // aUV (location=1) -> 2 floats, after pos
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Tell the shader how many rows/cols (uniforms)
    glUseProgram(prog);
    GLint uCols = glGetUniformLocation(prog, "uCols");
    GLint uRows = glGetUniformLocation(prog, "uRows");
    glUniform1i(uCols, GRID_COLS);
    glUniform1i(uRows, GRID_ROWS);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(prog);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

        app.Swap(window);
        app.Poll();
    }

    // Cleanup
    glDeleteBuffers(1,&ebo);
    glDeleteVertexArrays(1,&vao);
    glDeleteProgram(prog);
    app.Destroy(window);

    return 0;
}
