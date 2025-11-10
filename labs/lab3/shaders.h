const char *vertexShaderSrc = R"(
#version 410 core
layout(location=0) in vec2 aPos;

uniform mat4 u_projection; // Projection matrix
uniform mat4 u_view;  // View matrix
uniform mat4 u_model; // Model matrix

out vec2 vUV;

void main() {
    vUV = aPos + 0.5;
    gl_Position = u_projection * u_view * u_model * vec4(aPos * 2.0, 0.0, 1.0);
}
)";

const char *fragmentShaderSrc = R"(
#version 410 core
in vec2 vUV;
out vec4 FragColor;

uniform int uCols;   // = GRID_COLS
uniform int uRows;   // = GRID_ROWS
uniform vec2 uSelectedTile;

void main()
{
    // Calculate the current tile's grid position
    int col = int(floor(vUV.x * float(uCols)));
    int row = int(floor(vUV.y * float(uRows)));
    ivec2 tilePos = ivec2(col, row);

    ivec2 selected = ivec2(uSelectedTile);

    // Highlight the selected tile with green, otherwise use a checkerboard pattern
    if (tilePos == selected)
    {
        FragColor = vec4(0.0, 1.0, 0.0, 1.0); // Green
    }
    else
    {
        int checker = (col + row) % 2;
        FragColor = checker == 0 ? vec4(1.0, 1.0, 1.0, 1.0) : vec4(0.0, 0.0, 0.0, 1.0); // Black and white
    }
}
)";
