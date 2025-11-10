#include <glad/glad.h>
#include <glm/glm.hpp>

#include <memory>

namespace RenderCommands
{
    inline void Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    inline void EnableDepthTesting()
    {
        glEnable(GL_DEPTH_TEST);
    }

    inline void SetPolygonMode(GLenum face, GLenum mode)
    {
        glPolygonMode(face, mode);
    }

    inline void SetViewport(int x, int y, int w, int h)
    {
        glViewport(x, y, w, h);
    }

    inline void DrawIndex(const std::shared_ptr<VertexArray> &vao, GLenum primitive)
    {
        glDrawElements(primitive, vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    inline void SetClearColor(glm::vec4 color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    inline void SetWireframeMode()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    inline void SetSolidMode()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
