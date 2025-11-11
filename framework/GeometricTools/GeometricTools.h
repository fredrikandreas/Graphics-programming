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

            for (int j = 0; j <= DivY; ++j)
            {
                float y = -0.5f + j * dy;
                for (int i = 0; i <= DivX; ++i)
                {
                    float x = -0.5f + i * dx;
                    vertices[index++] = x;
                    vertices[index++] = y;
                }
            }
        }

    public:
        static constexpr int NumVertices = (DivX + 1) * (DivY + 1);
        static constexpr int NumFloats = NumVertices * 2;

        std::array<float, NumFloats> vertices{};

        UnitGridGeometry2D()
        {
            GenerateGrid();
        }

        const std::array<float, NumFloats> &GetGrid() const
        {
            return vertices;
        }

        int GetVertexCount() const
        {
            return NumVertices;
        }
    };

    template <int DivX, int DivY>
    class UnitGridTopologyTriangles
    {
    private:
        void GenerateIndices()
        {
            int index = 0;

            for (int y = 0; y < DivY; ++y)
            {
                for (int x = 0; x < DivX; ++x)
                {
                    int tL = y * VerticesPerRow + x;
                    int tR = tL + 1;
                    int bL = tL + VerticesPerRow;
                    int bR = bL + 1;

                    // First triangle
                    indices[index++] = tL;
                    indices[index++] = bL;
                    indices[index++] = bR;

                    // Second triangle
                    indices[index++] = tL;
                    indices[index++] = bR;
                    indices[index++] = tR;
                }
            }
        }

    public:
        static constexpr int VerticesPerRow = DivX + 1;
        static constexpr int NumIndices = 6 * DivX * DivY;

        std::array<GLuint, NumIndices> indices{};

        UnitGridTopologyTriangles()
        {
            GenerateIndices();
        }

        const std::array<GLuint, NumIndices> &GetIndices() const
        {
            return indices;
        }
    };

    // Unit cube centered at origin.
    class UnitCubeGeometry3D
    {
    public:
        static constexpr int NumVertices = 8;
        static constexpr int NumFloats = NumVertices * 3;

        std::array<float, NumFloats> vertices{};

        UnitCubeGeometry3D()
        {
            // 8 corner vertices (x,y,z)
            vertices = {
                -0.5f, -0.5f, -0.5f, // 0
                0.5f, -0.5f, -0.5f,  // 1
                0.5f, 0.5f, -0.5f,   // 2
                -0.5f, 0.5f, -0.5f,  // 3
                -0.5f, -0.5f, 0.5f,  // 4
                0.5f, -0.5f, 0.5f,   // 5
                0.5f, 0.5f, 0.5f,    // 6
                -0.5f, 0.5f, 0.5f    // 7
            };
        }

        const std::array<float, NumFloats> &GetVertices() const { return vertices; }
    };

    class UnitCubeTopologyTriangles
    {
    public:
        static constexpr int NumIndices = 36;
        std::array<GLuint, NumIndices> indices{};

        UnitCubeTopologyTriangles()
        {
            indices = {
                // Back face (-Z)
                0, 1, 2, 0, 2, 3,
                // Front face (+Z)
                4, 6, 5, 4, 7, 6,
                // Bottom face (-Y)
                0, 4, 5, 0, 5, 1,
                // Top face (+Y)
                3, 2, 6, 3, 6, 7,
                // Right face (+X)
                1, 5, 6, 1, 6, 2,
                // Left face (-X)
                0, 3, 7, 0, 7, 4};
        }

        const std::array<GLuint, NumIndices> &GetIndices() const
        {
            return indices;
        }
    };
    // Constexpr version that generates the geometry of the grid, including texture coordinates
    // The shape of the generated data is PPTTPPTTPPTT..., meaning two components for position
    // and two components for texture coordinates.
    template <unsigned int X, unsigned int Y>
    class UnitGridGeometry2DWTCoords
    {
    public:
        static constexpr GLsizei total = (X + 1) * (Y + 1) * (2 + 2);
        std::array<float, total> verticies;
        UnitGridGeometry2DWTCoords()
        {
            generate();
        }
        const std::array<float, total> &GetGrid() const
        {
            return verticies;
        }

    private:
        void generate()
        {
            float stepX = 1.0f / X;
            float stepY = 1.0f / Y;

            float startX = -0.5f;
            float startY = -0.5f;

            float startTX = 0.0f;
            float startTY = 0.0f;

            std::size_t index = 0;

            for (size_t y = 0; y <= Y; ++y)
            {
                for (size_t x = 0; x <= X; ++x)
                {
                    index = (y * (X + 1) + x) * 4;
                    verticies[index] = startX + (x * stepX);
                    verticies[index + 1] = startY + (y * stepY);
                    verticies[index + 2] = startTX + (x * stepX);
                    verticies[index + 3] = startTY + (y * stepY);
                }
            }
        }
    };

    // Unit cube with per-face normals (24 unique vertices)
    class UnitCubeGeometry3D24WNormals
    {
    public:
        static constexpr int NumVertices = 24;
        static constexpr int NumFloats = NumVertices * 6; // position (3) + normal (3)

        std::array<float, NumFloats> vertices{};

        UnitCubeGeometry3D24WNormals()
        {
            vertices = {
                // Front (+Z)
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
                0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

                // Back (-Z)
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
                -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
                0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

                // Left (-X)
                -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
                -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,

                // Right (+X)
                0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

                // Top (+Y)
                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,

                // Bottom (-Y)
                -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
                0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f};
        }

        const std::array<float, NumFloats> &GetVertices() const { return vertices; }
    };

    // Unit cube topology for 24-vertex cube
    class UnitCubeTopologyTriangles24
    {
    public:
        static constexpr int NumIndices = 36;
        std::array<GLuint, NumIndices> indices{};

        UnitCubeTopologyTriangles24()
        {
            indices = {
                // Front
                0, 1, 2, 0, 2, 3,
                // Back
                4, 5, 6, 4, 6, 7,
                // Left
                8, 9, 10, 8, 10, 11,
                // Right
                12, 13, 14, 12, 14, 15,
                // Top
                16, 17, 18, 16, 18, 19,
                // Bottom
                20, 21, 22, 20, 22, 23};
        }

        const std::array<GLuint, NumIndices> &GetIndices() const { return indices; }
    };

}