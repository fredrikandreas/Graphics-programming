const char *vertexShaderSrc = R"(
#version 410 core
layout(location=0) in vec2 a_inPosition;
layout(location=1) in vec2 a_inTexCoord;

uniform mat4 u_projection; // Projection matrix
uniform mat4 u_view;  // View matrix
uniform mat4 u_model; // Model matrix

out vec2 v_worldPos;
out vec2 v_fragTextCoord;

void main() {
    v_worldPos = a_inPosition;
    v_fragTextCoord = a_inTexCoord;
    gl_Position = u_projection * u_view * u_model * vec4(a_inPosition, 0.0, 1.0);
}
)";

const char *fragmentShaderSrc = R"(
#version 410 core

in vec2 v_worldPos;
in vec2 v_fragTextCoord;

out vec4 FragColor;

uniform vec2 u_gridSize;
uniform sampler2D u_floorTextureSampler;
uniform vec2 u_SelectedTile;

uniform float u_ambientStrength = 1.0;
uniform float u_translucence = 1.0;

void main()
{
    // Calculate the current tile's grid position
    int col = int(floor(v_worldPos.x * float(u_gridSize.x)));
    int row = int(floor(v_worldPos.y * float(u_gridSize.y)));

    ivec2 tileCoords = ivec2(col, row);
    ivec2 selectedCoords = ivec2(u_SelectedTile);
    vec4 baseColor;

    vec2 localUV = fract(v_worldPos * u_gridSize);

    // Highlight the selected tile with green, otherwise use a checkerboard pattern
    if (tileCoords == selectedCoords)
    {
        baseColor = vec4(0.2, 1.0, 0.0, 1.0); // White
    }
    else
    {
        int checker = (col + row) % 2;
        baseColor = checker == 0 
            ? vec4(0.3, 0.3, 0.3, 1.0)  // Grey
            : vec4(0.0, 0.0, 0.0, 1.0); // Black
    }
    FragColor = mix(baseColor, texture(u_floorTextureSampler, localUV), 0.9);
    FragColor = vec4(u_ambientStrength * FragColor.rgb, u_translucence);
}
)";

const char *cubeVertexShaderSrc = R"(
    #version 410 core
    layout(location = 0) in vec3 i_position;

    uniform mat4 u_projection;
    uniform mat4 u_view;
    uniform mat4 u_model;

    out vec3 vs_position;

    void main() {
        gl_Position = u_projection * u_view * u_model * vec4(i_position, 1.0);
        vs_position = i_position;
    }
)";

const char *cubeFragmentShaderSrc = R"(
    #version 410 core
    uniform samplerCube u_cubeTextureSampler;

    uniform float u_ambientStrength = 1.0;
    uniform float u_translucence = 1.0;

    in vec3 vs_position;
    out vec4 color;

    void main() {
        color = texture(u_cubeTextureSampler, vs_position);
        color = vec4(u_ambientStrength * color.rgb, u_translucence);
    }
)";