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

void PieceRenderer::init(const PerspectiveCamera &cam, const glm::vec3 &cameraPosition)
{
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
    m_shader->Unbind();
}

void PieceRenderer::draw(float angleXDeg, float angleYDeg)
{
    m_shader->Bind();
    m_vao->Bind();

    glm::mat4 Rx = glm::rotate(glm::mat4(1.0f), glm::radians(angleXDeg), glm::vec3(0, 1, 0));
    glm::mat4 Ry = glm::rotate(glm::mat4(1.0f), glm::radians(angleYDeg), glm::vec3(1, 0, 0));
    m_shader->UploadUniformMat4("u_model", Rx * Ry);

    RenderCommands::DrawIndex(m_vao, GL_TRIANGLES);
}
