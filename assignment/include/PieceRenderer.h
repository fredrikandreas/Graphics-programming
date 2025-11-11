#ifndef PIECE_RENDERER_H
#define PIECE_RENDERER_H

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
    void selectOrMove(const glm::ivec2& selectedTile);
    bool tileOccupied(const glm::ivec2& tile) const;

private:
    struct Piece
    {
        glm::mat4 model;
        glm::vec3 color;
        int col;
        int row;
        bool moving = false;
        glm::vec3 originalColor; 
    };
    std::shared_ptr<VertexArray> m_vao;
    std::shared_ptr<VertexBuffer> m_vbo;
    std::shared_ptr<IndexBuffer> m_ibo;
    std::shared_ptr<Shader> m_shader;

    const BoardRenderer* m_board = nullptr;

    std::vector<Piece> m_pieces;
    std::optional<int> m_selectedPiece; 
};

#endif // PIECE_RENDERER_H