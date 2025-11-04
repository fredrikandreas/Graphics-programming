#include <glad/glad.h>

#include <array>
#include <vector>

namespace GeometricTools
{
    // Triangle
    constexpr std::array<float, 3 * 2> UnitTriangle2D = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.0f, 0.5f};

    // Square
    constexpr std::array<float, 6 * 2> UnitSquare2D = {
        // First triangle
        -0.5f, 0.5f,
        0.5f, 0.5f,
        -0.5f, -0.5f,
        // Second triangle
        0.5f, 0.5f,
        0.5f, -0.5f,
        -0.5f, -0.5f};

    // 2D grid geometry
    template <int DivX, int DivY>
    class UnitGridGeometry2D
    {
    private:
        void GenerateGrid()
        {
            float dx = 1.0f / DivX;
            float dy = 1.0f / DivY;
            int index = 0;

            for (int i = 0; i <= DivX; i++)
            {
                float x = -0.5f + i * dx;
                vertices[index++] = x;
                vertices[index++] = -0.5f;
                vertices[index++] = x;
                vertices[index++] = 0.5f;
            }
            for (int j = 0; j <= DivY; j++)
            {
                float y = -0.5f + j * dy;
                vertices[index++] = -0.5f;
                vertices[index++] = y;
                vertices[index++] = 0.5f;
                vertices[index++] = y;
            }
        }
    public:
        static constexpr int TotalFloats = 4 * (DivX + DivY + 2);
        static constexpr int TotalVertices = TotalFloats / 2;

        std::array<float, TotalFloats> vertices{};

        UnitGridGeometry2D()
        {
            GenerateGrid();
        }

        const std::array<float, TotalFloats> &GetGrid() const
        {
            return vertices;
        }
    }
}