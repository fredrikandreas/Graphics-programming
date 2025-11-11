#include <glm/glm.hpp>
#include <memory>

class VertexArray;
class VertexBuffer;
class IndexBuffer;
class Shader;
class PerspectiveCamera;

class PieceRenderer
{
public:
    void init(const PerspectiveCamera &cam, const glm::vec3 &cameraPosition);
    void draw(float angleXDeg, float angleYDeg);

private:
    std::shared_ptr<VertexArray> m_vao;
    std::shared_ptr<VertexBuffer> m_vbo;
    std::shared_ptr<IndexBuffer> m_ibo;
    std::shared_ptr<Shader> m_shader;
};
