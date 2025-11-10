const char *vertexShaderSrc = R"(
#version 410 core
layout(location=0) in vec2 aPos;
out vec2 vUV;

void main() {
    vUV = aPos + 0.5;
    gl_Position = vec4(aPos * 2.0, 0.0, 1.0);
}
)";

const char *fragmentShaderSrc = R"(
#version 410 core
in vec2 vUV;
out vec4 FragColor;

uniform int uCols;   // = GRID_COLS
uniform int uRows;   // = GRID_ROWS

void main() {
    // Compute which grid cell this pixel is in
    int col = int(floor(vUV.x * float(uCols)));
    int row = int(floor(vUV.y * float(uRows)));

    // Checker pattern
    if ((col + row) % 2 == 0)
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    else
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
)";
