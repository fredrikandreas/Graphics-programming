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
}