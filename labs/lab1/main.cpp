// #include area (SECTION 1)
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFWApplication.h>
#include <GeometricTools.h>

#include <iostream>

const char *APP_NAME = "Lab 1";
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int V_MAJOR = 4;
const int V_MINOR = 1;

struct GLFWwindow;

int main(int argc, char **argv)
{
    std::cout << "Hello World!" << std::endl;

    // GLFW initialization code and OpenGL initialization code (SECTION 2) + (SECTION 3)
    GLFWApplication app = GLFWApplication(APP_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, V_MAJOR, V_MINOR);

    // OpenGL data transfer code (SECTION 4)
    GLFWwindow *window = app.Init();

    // Drawing Geometric Objects in OpenGL
    auto triangle = GeometricTools::UnitTriangle2D;

    // Create a vertex array
    GLuint vertexArrayId;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    // Create a vertex buffer
    GLuint vertexBufferId;
    glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);

    // Populate the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle.data(), GL_STATIC_DRAW);

    // Set the layout of the bound buffer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);
    glEnableVertexAttribArray(0);

    auto square = GeometricTools::UnitSquare2D;

    // Create a vertex array
    GLuint squareVAO;
    glGenVertexArrays(1, &squareVAO);
    glBindVertexArray(squareVAO);

    // Create a vertex buffer
    GLuint squareVBO;
    glGenBuffers(1, &squareVBO);
    glBindBuffer(GL_ARRAY_BUFFER, squareVBO);

    // Populate the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square.data(), GL_STATIC_DRAW);

    // Set the layout of the bound buffer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);
    glEnableVertexAttribArray(0);

    const std::string vertexShaderSrc = R"(
        #version 410 core
        
        layout(location = 0) in vec2 position;
        uniform vec2 offset;   // Shift in X and Y position
        
        void main()
        {
            gl_Position = vec4(position + offset, 0.0, 1.0); // Homogeneous coordinates 3D+1
        }
    )";

    const std::string fragmentShaderSrc = R"(
        #version 410 core
        
        out vec4 color;
        uniform float time;

        void main()
        {
            float r = (sin(time) + 1.0) * 0.5;
            float g = (sin(time + 2.0) + 1.0) * 0.5;
            float b = (sin(time + 4.0) + 1.0) * 0.5;
            color = vec4(r, g, b, 1.0);
        }
    )";

    // Compile the vertex shader
    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar *vss = vertexShaderSrc.c_str();
    glShaderSource(vertexShader, 1, &vss, nullptr);
    glCompileShader(vertexShader);

    // Compile the fragment shader
    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar *fss = fragmentShaderSrc.c_str();
    glShaderSource(fragmentShader, 1, &fss, nullptr);
    glCompileShader(fragmentShader);

    // Create a shader program
    auto shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

    // Shader objects can be deleted once they
    // have been linked in a shader program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);

    GLint offsetLocation = glGetUniformLocation(shaderProgram, "offset");
    GLint timeLocation = glGetUniformLocation(shaderProgram, "time");

    glClearColor(0.5f, 0.0f, 0.0f, 1.0f);

    // Application loop code (SECTION 5)
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        float t = glfwGetTime(); // Time in seconds since program start
        glUniform1f(timeLocation, t);

        // Draw triangle
        glBindVertexArray(vertexArrayId);
        glUniform2f(offsetLocation, -0.5f, 0.0f); // shift to the left
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Draw square
        glBindVertexArray(squareVAO);
        glUniform2f(offsetLocation, 0.5f, 0.0f); // shift to the right
        glDrawArrays(GL_TRIANGLES, 0, 6);

        app.Swap(window);
        app.Poll();
    }

    // Termination code (SECTION 6)
    app.Destroy(window);

    return 0;
}
