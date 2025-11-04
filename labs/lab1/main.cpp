// #include area (SECTION 1)
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

struct GLFWwindow;

int main(int argc, char** argv)
{
    std::cout << "Hello World!" << std::endl;

    // GLFW initialization code (SECTION 2)
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // OpenGL initialization code (SECTION 3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // OpenGL data transfer code (SECTION 4)
    GLFWwindow* window = glfwCreateWindow(800, 800, "Lab 2", nullptr, nullptr);
    if (!window) 
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Load OpenGL function pointers using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Drawing Geometric Objects in OpenGL
    // Create a triangle
    float triangle[3*2] = { // 3 vertices x 2 coordinate components
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.0f,  0.5f
    };

    // Create a vertex array
    GLuint vertexArrayId;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    // Create a vertex buffer
    GLuint vertexBufferId;
    glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);

    // Populate the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

    // Set the layout of the bound buffer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, nullptr);
    glEnableVertexAttribArray(0);

    // Create a square (two traingles forming a square)
    float square[] = {
        // First triangle
        -0.5f,  0.5f,
         0.5f,  0.5f,
        -0.5f, -0.5f,
        // Second triangle
         0.5f,  0.5f,
         0.5f, -0.5f,
        -0.5f, -0.5f
    };

    // Create a vertex array
    GLuint squareVAO;
    glGenVertexArrays(1, &squareVAO);
    glBindVertexArray(squareVAO);

    // Create a vertex buffer
    GLuint squareVBO;
    glGenBuffers(1, &squareVBO);
    glBindBuffer(GL_ARRAY_BUFFER, squareVBO);

    // Populate the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);

    // Set the layout of the bound buffer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, nullptr);
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
    const GLchar* vss = vertexShaderSrc.c_str();
    glShaderSource(vertexShader, 1, &vss, nullptr);
    glCompileShader(vertexShader);

    // Compile the fragment shader
    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* fss = fragmentShaderSrc.c_str();
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

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Termination code (SECTION 6)
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
