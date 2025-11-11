#include "include/PieceRenderer.h"
#include <GeometricTools.h>
#include <VertexArray.h>
#include <VertexBuffer.h>
#include <IndexBuffer.h>
#include <Shader.h>
#include <RenderCommands.h>
#include <TextureManager.h>
#include <glm/gtc/matrix_transform.hpp>
#include <PerspectiveCamera.h>
#include "shaders.h"

#ifndef TEXTURES_DIR
#define TEXTURES_DIR ""
#endif

void PieceRenderer::init(const PerspectiveCamera &cam, const BoardRenderer &board, const glm::vec3 &cameraPosition)
{
    m_board = &board;

    // texture (using cube sampler id 1 to stay consistent)
    TextureManager::GetInstance()->LoadCubeMapRGBA("cube",
                                                   std::string(TEXTURES_DIR) + "cube_texture.png", 1);

    GeometricTools::UnitCubeGeometry3D24WNormals g;
    GeometricTools::UnitCubeTopologyTriangles24 t;
    const auto &verts = g.GetVertices();
    const auto &idx = t.GetIndices();

    m_ibo = std::make_shared<IndexBuffer>(idx.data(), idx.size());
    auto layout = BufferLayout({{ShaderDataType::Float3, "position"},
                                {ShaderDataType::Float3, "normal"}});
    m_vbo = std::make_shared<VertexBuffer>(verts.data(), verts.size() * sizeof(verts[0]));
    m_vbo->SetLayout(layout);
    m_vao = std::make_shared<VertexArray>();
    m_vao->AddVertexBuffer(m_vbo);
    m_vao->SetIndexBuffer(m_ibo);

    m_shader = std::make_shared<Shader>(cubeVertexShaderSrc, cubeFragmentShaderSrc);
    m_shader->Bind();
    m_shader->UploadUniformMat4("u_projection", cam.GetProjectionMatrix());
    m_shader->UploadUniformMat4("u_view", cam.GetViewMatrix());
    m_shader->UploadUniformInt("u_cubeTextureSampler", 1);
    m_shader->UploadUniformFloat3("u_lightSourcePosition", cameraPosition);
    m_shader->UploadUniformFloat("u_diffuseStrength", 0.8f);
    m_shader->UploadUniformFloat3("u_cameraPosition", cameraPosition);
    m_shader->UploadUniformFloat("u_specularStrength", 0.1f);

    // build chess starting positions (red bottom, blue top)
    auto placePiece = [&](int col, int row, const glm::vec3 &color)
    {
        // ask the board for the real world center of this tile
        glm::vec3 worldCenter = board.tileCenterWorld(col, row);

        // keep a tiny levitation above the board plane (Y is up after the board’s 90° X-rot)
        constexpr glm::vec3 lift(0.0f, 0.08f, 0.0f);

        glm::mat4 M(1.0f);
        M = glm::translate(M, worldCenter + lift);
        M = glm::scale(M, glm::vec3(0.3f));
        m_pieces.push_back({M, color, col, row});
    };

    // use the board’s real grid size
    const int C = board.cols();
    const int R = board.rows();

    // pawns
    for (int c = 0; c < C; ++c)
    {
        placePiece(c, 1, glm::vec3(0.8f, 0.1f, 0.1f));     // red
        placePiece(c, R - 2, glm::vec3(0.1f, 0.1f, 0.8f)); // blue
    }

    // back ranks
    for (int c = 0; c < C; ++c)
    {
        placePiece(c, 0, glm::vec3(0.8f, 0.1f, 0.1f));     // red back
        placePiece(c, R - 1, glm::vec3(0.1f, 0.1f, 0.8f)); // blue back
    }

    m_vao->Unbind();
    m_vbo->Unbind();
    m_ibo->Unbind();
    m_shader->Unbind();
}

void PieceRenderer::draw(float timeSeconds, const glm::ivec2 &selectedTile)
{
    m_shader->Bind();
    m_vao->Bind();

    float baseHeight = 1.0f; // middle point
    float amplitude = 0.5f;  // total swing up/down around base
    float levitate = baseHeight + amplitude * std::sin(timeSeconds * 2.0f);

    for (const auto &p : m_pieces)
    {
        glm::mat4 M = p.model;
        glm::vec3 color = p.color;

        if (p.col == selectedTile.x && p.row == selectedTile.y)
        {
            color = glm::vec3(0.5f, 0.5f, 0.0f);
            M = glm::translate(M, glm::vec3(0.0f, levitate, 0.0f));
        }

        m_shader->UploadUniformMat4("u_model", M);
        m_shader->UploadUniformFloat3("u_color", color);
        RenderCommands::DrawIndex(m_vao, GL_TRIANGLES);
    }
}

bool PieceRenderer::tileOccupied(const glm::ivec2 &tile) const
{
    for (const auto &p : m_pieces)
        if (p.col == tile.x && p.row == tile.y)
            return true;
    return false;
}

void PieceRenderer::selectOrMove(const glm::ivec2 &selectedTile)
{
    // No piece selected yet, try selecting one
    if (!m_selectedPiece.has_value())
    {
        for (int i = 0; i < m_pieces.size(); ++i)
        {
            auto &p = m_pieces[i];
            if (p.col == selectedTile.x && p.row == selectedTile.y)
            {
                p.moving = true;
                p.originalColor = p.color;
                p.color = glm::vec3(0.5f, 0.5f, 0.0f);
                m_selectedPiece = i;
                return;
            }
        }
    }
    else // A piece is already selected, try to move it
    {
        int i = *m_selectedPiece;
        auto &movingPiece = m_pieces[i];

        // Target occupied? cancel move
        if (tileOccupied(selectedTile))
        {
            movingPiece.color = movingPiece.originalColor;
            movingPiece.moving = false;
            m_selectedPiece.reset();
            return;
        }

        // Move piece: update col/row and transform
        movingPiece.col = selectedTile.x;
        movingPiece.row = selectedTile.y;

        glm::vec3 worldCenter = m_board->tileCenterWorld(selectedTile.x, selectedTile.y);
        glm::vec3 lift(0.0f, 0.08f, 0.0f);
        movingPiece.model = glm::translate(glm::mat4(1.0f), worldCenter + lift);
        movingPiece.model = glm::scale(movingPiece.model, glm::vec3(0.3f));

        // Revert back to original color after successful move
        movingPiece.color = movingPiece.originalColor;

        movingPiece.moving = false;
        m_selectedPiece.reset();
    }
}
