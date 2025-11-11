#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "BoardRenderer.h"

class VertexArray;
class VertexBuffer;
class IndexBuffer;
class Shader;
class PerspectiveCamera;

class PieceRenderer
{
public:
    PieceRenderer() = default;
    ~PieceRenderer() = default;

    void init(const PerspectiveCamera &cam, const BoardRenderer &board, const glm::vec3 &cameraPosition);
    void draw(float timeSeconds, const glm::ivec2 &selectedTile);

private:
    struct Piece
    {
        glm::mat4 model;
        glm::vec3 color;
        int col;
        int row;
        float phase;
        float speed;
        float amp;
    };
    std::shared_ptr<VertexArray> m_vao;
    std::shared_ptr<VertexBuffer> m_vbo;
    std::shared_ptr<IndexBuffer> m_ibo;
    std::shared_ptr<Shader> m_shader;

    std::vector<Piece> m_pieces;
};
