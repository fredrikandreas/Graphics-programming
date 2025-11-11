#include <glm/glm.hpp>
#include <memory>

class VertexArray;
class VertexBuffer;
class IndexBuffer;
class Shader;
class PerspectiveCamera;

class BoardRenderer
{
public:
    BoardRenderer(int cols = 8, int rows = 8);

    void init(const PerspectiveCamera &cam);
    void draw();
    void setAmbient(float a);

    void setSelectedTile(glm::ivec2 tile) { m_selectedTile = tile; }

private:
    int m_cols, m_rows;
    glm::ivec2 m_selectedTile{0, 0};

    std::shared_ptr<VertexArray> m_vao;
    std::shared_ptr<VertexBuffer> m_vbo;
    std::shared_ptr<IndexBuffer> m_ibo;
    std::shared_ptr<Shader> m_shader;
    glm::mat4 m_model{1.0f};
};
