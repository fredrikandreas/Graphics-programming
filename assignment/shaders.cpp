#include "include/shaders.h"

const char *vertexShaderSrc = R"(
    #version 410 core
    layout(location=0) in vec2 position;
    layout(location=1) in vec2 texCoord;
    
    uniform mat4 u_projection; // Projection matrix
    uniform mat4 u_view;  // View matrix
    uniform mat4 u_model; // Model matrix
    
    out vec2 v_worldPos;
    out vec2 v_fragTextCoord;
    
    void main() {
        v_worldPos = position;
        v_fragTextCoord = texCoord;
        gl_Position = u_projection * u_view * u_model * vec4(position, 0.0, 1.0);
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
            baseColor = vec4(0.5, 0.5, 0.0, 1.0); // White
        }
        else
        {
            int checker = (col + row) % 2;
            baseColor = checker == 0 
                ? vec4(0.3, 0.3, 0.3, 1.0)  // Grey
                : vec4(0.0, 0.0, 0.0, 1.0); // Black
        }
        FragColor = mix(baseColor, texture(u_floorTextureSampler, v_worldPos), 0.4);
        FragColor = vec4(u_ambientStrength * FragColor.rgb, u_translucence);
    }
    )";
    
    const char *cubeVertexShaderSrc = R"(
        #version 410 core
        layout(location=0) in vec3 position;
        layout(location=3) in vec3 normal;
    
        uniform mat4 u_projection, u_view, u_model;
    
        out vec3 vs_position;
        out vec4 vs_normal;
        out vec4 vs_fragPosition;
    
        void main() {
            vs_position = position;
            gl_Position = u_projection * u_view * u_model * vec4(position, 1.0);
            vs_normal = normalize(u_model * vec4(normal, 1.0));
            vs_fragPosition = u_model * vec4(position, 1.0);
        }
    )";
    
    const char *cubeFragmentShaderSrc = R"(
        #version 410 core
        uniform samplerCube u_cubeTextureSampler;
    
        uniform float u_ambientStrength = 1.0;
        uniform float u_translucence = 1.0;
    
        uniform vec3 u_lightSourcePosition;
        uniform float u_diffuseStrength;
    
        uniform vec3 u_lightColor = vec3(1.0, 1.0, 0.85);
    
        uniform vec3 u_cameraPosition;
        uniform vec3 u_specularColor = vec3(1.0, 0.85, 0.85);
        uniform float u_specularStrength = 0.4f;
    
    
        in vec3 vs_position;
        in vec4 vs_normal;
        in vec4 vs_fragPosition;
        out vec4 color;
    
        void main() {
            vec3 lightDirection = normalize(vec3(u_lightSourcePosition - vs_fragPosition.xyz));
    
            float diffuseStrength = max(dot(lightDirection, vs_normal.xyz), 0.0f) * u_diffuseStrength;
    
            vec3 reflectedLight = normalize(reflect(-lightDirection, normalize(vs_normal.xyz)));
            vec3 observerDirection = normalize(u_cameraPosition - vs_fragPosition.xyz);
            float specFactor = pow(max(dot(observerDirection, reflectedLight), 0.0), 12.0);
            vec3 specular = specFactor * u_specularColor * u_specularStrength;
    
    
            vec4 texColor = texture(u_cubeTextureSampler, vs_position);
    
            vec3 lighting = (u_ambientStrength + diffuseStrength) * u_lightColor;
    
            vec3 litColor = texColor.rgb * lighting + specular;
    
            color = vec4(litColor, u_translucence);
    
        }
    )";