#include "Shader.h"

Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc)
{
    CompileShader(GL_VERTEX_SHADER, vertexSrc);
    CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);

    ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
}

Shader::~Shader()
{
    glDeleteProgram(ShaderProgram);
}

void Shader::Bind() const
{
    glUseProgram(ShaderProgram);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::UploadUniformFloat2(const std::string& name, const glm::vec2& vector)
{
    GLint location = glGetUniformLocation(ShaderProgram, name.c_str());
    glUniform2f(location, vector.x, vector.y);
}

void Shader::CompileShader(GLenum shaderType, const std::string &shaderSrc)
{
    GLuint shader = glCreateShader(shaderType);
    const char *src = shaderSrc.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    if (shaderType == GL_VERTEX_SHADER)
        VertexShader = shader;
    else if (shaderType == GL_FRAGMENT_SHADER)
        FragmentShader = shader;
}