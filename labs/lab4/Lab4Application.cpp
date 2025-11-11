#include "Lab4Application.h"

GLuint Lab4Application::LoadTexture(const std::string &filepath, GLuint slot)
{
    int width, height, bpp;

    auto pixels = stbi_load(filepath.c_str(), &width, &height, &bpp, STBI_rgb_alpha);
    if (!pixels)
    {
        std::cerr << "Failed to load texture" << std::endl;
        return 0;
    }

    GLuint tex;
    glGenTextures(1, &tex); //[3]

    glActiveTexture(GL_TEXTURE0 + slot); // Specify texture unit. [4]
    glBindTexture(GL_TEXTURE_2D, tex);   // [5]

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels); // [6]

    // Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //[7]
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (pixels)
        stbi_image_free(pixels);

    return tex;
}

GLuint Lab4Application::LoadCubeMap(const std::array<std::string,3> &filepaths, GLuint slot)
{
    int width, height, bpp;

    auto pixelsTop = stbi_load(filepaths[0].c_str(), &width, &height, &bpp, STBI_rgb_alpha);
    auto pixelsBottom = stbi_load(filepaths[1].c_str(), &width, &height, &bpp, STBI_rgb_alpha);
    auto pixelsSides = stbi_load(filepaths[2].c_str(), &width, &height, &bpp, STBI_rgb_alpha);

    if (!pixelsTop || !pixelsBottom || !pixelsSides)
    {
        std::cerr << "Failed to load texture" << std::endl;
        return 0;
    }

    GLuint tex;
    glGenTextures(1, &tex); //[3]
    glActiveTexture(GL_TEXTURE1 + slot); // Specify texture unit. [4]
    glBindTexture(GL_TEXTURE_2D, tex);   // [5]

    for (unsigned int i = 0; i < 6; i++)
    {
        if (i == 2) // Top
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelsTop);
        else if (i == 3) // Bottom
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelsBottom);
        else // Sides
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelsSides);
    }

    // Wrapping
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
    // Filtering
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(pixelsTop);
    stbi_image_free(pixelsBottom);
    stbi_image_free(pixelsSides);

    return tex;
}