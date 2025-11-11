#include "include/BoardRenderer.h"
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

BoardRenderer::BoardRenderer(int cols, int rows)
    : m_cols(cols), m_rows(rows) {}

void BoardRenderer::init(const PerspectiveCamera &cam)
{
    // textures
    TextureManager::GetInstance()->LoadTexture2DRGBA("floor", std::string(TEXTURES_DIR) + "floor_texture.png", 0);

    // shader - create and bind shader early so attribute locations are known when VAO/VBO are set up
    m_shader = std::make_shared<Shader>(vertexShaderSrc, fragmentShaderSrc);
    m_shader->Bind();

    // grid geometry
    GeometricTools::UnitGridGeometry2DWTCoords<8, 8> grid_g;
    GeometricTools::UnitGridTopologyTriangles<8, 8> grid_t;
    const auto &geom = grid_g.GetGrid();
    const auto &topo = grid_t.GetIndices();

    m_ibo = std::make_shared<IndexBuffer>(topo.data(), topo.size());
    auto layout = BufferLayout({{ShaderDataType::Float2, "position"},
                                {ShaderDataType::Float2, "texCoord"}});
    m_vbo = std::make_shared<VertexBuffer>(geom.data(), geom.size() * sizeof(geom[0]));
    m_vbo->SetLayout(layout);
    m_vao = std::make_shared<VertexArray>();
    m_vao->AddVertexBuffer(m_vbo);
    m_vao->SetIndexBuffer(m_ibo);

    // model transform
    glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(4.0f, 4.0f, 2.0f));
    glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.75f, -0.5f));
    m_model = T * R * S;

    // Upload uniforms while shader is bound
    m_vbo->Bind();
    m_ibo->Bind();
    m_shader->UploadUniformMat4("u_projection", cam.GetProjectionMatrix());
    m_shader->UploadUniformMat4("u_view", cam.GetViewMatrix());
    m_shader->UploadUniformMat4("u_model", m_model);
    m_shader->UploadUniformFloat2("u_gridSize", glm::vec2(m_cols, m_rows));
    m_shader->UploadUniformInt("u_floorTextureSampler", 0);

    // Unbind to leave clean GL state (matches lab5)
    m_vao->Unbind();
    m_vbo->Unbind();
    m_ibo->Unbind();
    m_shader->Unbind();
}

void BoardRenderer::draw()
{
    m_shader->Bind();
    m_vao->Bind();
    m_shader->UploadUniformFloat2("u_SelectedTile", glm::vec2(m_selectedTile));
    RenderCommands::DrawIndex(m_vao, GL_TRIANGLES);
}

void BoardRenderer::setAmbient(float a)
{
    m_shader->Bind();
    m_shader->UploadUniformFloat("u_ambientStrength", a);
}
