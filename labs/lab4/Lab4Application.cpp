#include "Lab4Application.h"

GLuint Lab4Application::LoadTexture(const std::string &filepath, GLuint slot)
{
    // Floor texture
    int width, height, bpp;

    auto pixels = stbi_load(filepath.c_str(), &width, &height, &bpp, STBI_rgb_alpha);
    if (!pixels)
    {
        std::cerr << "Failed to load texture" << std::endl;
        return 0;
    }

    GLuint tex;
    glGenTextures(1, &tex); //[3]

    glActiveTexture(GL_TEXTURE0 + slot);                                                           // Specify texture unit. [4]
    glBindTexture(GL_TEXTURE_2D, tex);                                                             // [5]

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