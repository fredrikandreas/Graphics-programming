const char *vertexShaderSrc = R"(
#version 410 core
layout(location=0) in vec2 aPos;
layout(location=1) in vec2 aUV;

out vec2 vUV;

void main() {
    vUV = aUV;                  // 0..1 across the board
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

const char *fragmentShaderSrc = R"(
#version 410 core
in vec2 vUV;
out vec4 FragColor;

uniform int uCols;   // = GRID_COLS
uniform int uRows;   // = GRID_ROWS

void main() {
    // Which cell?
    int cx = int(floor(vUV.x * float(uCols)));
    int cy = int(floor(vUV.y * float(uRows)));
    bool white = ((cx + cy) % 2) == 0;
    vec3 col = white ? vec3(1.0) : vec3(0.0);
    FragColor = vec4(col, 1.0);
}
)";
