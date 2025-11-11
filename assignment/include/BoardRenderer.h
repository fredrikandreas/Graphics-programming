#ifndef BOARD_RENDERER_H
#define BOARD_RENDERER_H

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

    const glm::mat4 &getModel() const { return m_model; }
    int cols() const { return m_cols; }
    int rows() const { return m_rows; }
    glm::vec3 tileCenterWorld(int col, int row) const;
    glm::vec3 boardCenterWorld() const;              
    void setCamera(const PerspectiveCamera& cam);

private:
    int m_cols, m_rows;
    glm::ivec2 m_selectedTile{0, 0};

    std::shared_ptr<VertexArray> m_vao;
    std::shared_ptr<VertexBuffer> m_vbo;
    std::shared_ptr<IndexBuffer> m_ibo;
    std::shared_ptr<Shader> m_shader;
    glm::mat4 m_model{1.0f};
};

#endif // BOARD_RENDERER_H