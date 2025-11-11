#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>

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
    Unbind();
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

void Shader::UploadUniformInt(const std::string& name, const int number)
{
    GLint location = glGetUniformLocation(ShaderProgram, name.c_str());
    Bind();
    glUniform1i(location, number);
}

void Shader::UploadUniformFloat2(const std::string& name, const glm::vec2& vector)
{
    GLint location = glGetUniformLocation(ShaderProgram, name.c_str());
    Bind();
    glUniform2f(location, vector.x, vector.y);
}

void Shader::UploadUniformFloat3(const std::string& name, const glm::vec3& vector)
{
    GLint location = glGetUniformLocation(ShaderProgram, name.c_str());
    Bind();
    glUniform3f(location, vector.x, vector.y, vector.z);
}

void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
{
    GLint location = glGetUniformLocation(ShaderProgram, name.c_str());
    Bind();
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
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