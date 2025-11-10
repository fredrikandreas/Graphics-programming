#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>

class Shader
{
public:
    Shader(const std::string &vertexSrc, const std::string &fragmentSrc);
    ~Shader();

    void Bind() const;
    void Unbind() const;
    void UploadUniformFloat2(const std::string &name, const glm::vec2 &vector);
    void UploadUniformFloat3(const std::string &name, const glm::vec3 &vector);

    void UploadUniformMat4(const std::string &name, const glm::mat4 &matrix);

    inline GLuint GetProgramID() const { return ShaderProgram; }

private:
    GLuint VertexShader;
    GLuint FragmentShader;
    GLuint ShaderProgram;

    void CompileShader(GLenum shaderType, const std::string &shaderSrc);
};
