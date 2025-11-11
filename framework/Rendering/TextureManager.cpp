// This is the TextureManager.cpp
#include "TextureManager.h"

#include <iostream>

bool TextureManager::LoadTexture2DRGBA(const std::string &name, const std::string &filePath, GLuint unit, bool mipMap)
{
    int width, height, bpp;
    auto data = this->LoadTextureImage(filePath, width, height, bpp, STBI_rgb_alpha);

    if (!data)
    {
        return false;
    }

    GLuint tex;
    glGenTextures(1, &tex);
    glActiveTexture(GL_TEXTURE0 + unit); // Texture Unit
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    if (mipMap)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    // Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Texture texture;
    texture.mipMap = mipMap;
    texture.width = width;
    texture.height = height;
    texture.name = name;
    texture.filePath = filePath;
    texture.unit = unit;
    texture.type = Texture2D;

    this->Textures.push_back(texture);

    this->FreeTextureImage(data);

    return true;
}

bool TextureManager::LoadCubeMapRGBA(const std::string &name, const std::string &filePath, GLuint unit, bool mipMap)
{
    int width, height, bpp;
    auto data = this->LoadTextureImage(filePath, width, height, bpp, STBI_rgb_alpha);

    if (!data)
    {
        return false;
    }

    /*Generate a texture object and upload the loaded image to it.*/
    GLuint tex;
    glGenTextures(1, &tex);
    glActiveTexture(GL_TEXTURE0 + unit); // Texture Unit
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

    for (unsigned int i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    if (mipMap)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    // Wrapping
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
    // Filtering
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Texture texture;
    texture.mipMap = mipMap;
    texture.width = width;
    texture.height = height;
    texture.name = name;
    texture.filePath = filePath;
    texture.unit = unit;
    texture.type = CubeMap;

    this->Textures.push_back(texture);
    this->FreeTextureImage(data);

    return true;
}

bool TextureManager::LoadCubeMapCustomRGBA(const std::string &name, const std::array<std::string,6> &filepaths, GLuint unit, bool mipMap)
{
    int width, height, bpp;

    auto left = this->LoadTextureImage(filepaths[0].c_str(), width, height, bpp, STBI_rgb_alpha);
    auto right = this->LoadTextureImage(filepaths[1].c_str(), width, height, bpp, STBI_rgb_alpha);
    auto top = this->LoadTextureImage(filepaths[2].c_str(), width, height, bpp, STBI_rgb_alpha);
    auto bottom = this->LoadTextureImage(filepaths[3].c_str(), width, height, bpp, STBI_rgb_alpha);
    auto back = this->LoadTextureImage(filepaths[4].c_str(), width, height, bpp, STBI_rgb_alpha);
    auto front = this->LoadTextureImage(filepaths[5].c_str(), width, height, bpp, STBI_rgb_alpha);

    if (!left || !right || !top || !bottom || !back || !front)
    {
        return false;
    }

    /*Generate a texture object and upload the loaded image to it.*/
    GLuint tex;
    glGenTextures(1, &tex);
    glActiveTexture(GL_TEXTURE0 + unit); // Texture Unit
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 0, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, left);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, right);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, top);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bottom);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, back);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, front);

    if (mipMap)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    // Wrapping
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
    // Filtering
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Texture texture;
    texture.mipMap = mipMap;
    texture.width = width;
    texture.height = height;
    texture.name = name;
    texture.filePath = "Custom Cube Map";
    texture.unit = unit;
    texture.type = CubeMap;

    this->Textures.push_back(texture);
    this->FreeTextureImage(left);
    this->FreeTextureImage(right);
    this->FreeTextureImage(top);
    this->FreeTextureImage(bottom);
    this->FreeTextureImage(back);
    this->FreeTextureImage(front);

    return true;
}

GLuint TextureManager::GetUnitByName(const std::string &name) const
{
    for (const auto &texture : this->Textures)
    {
        if (!texture.name.compare(name))
        {
            return texture.unit;
        }
    }
    return -1;
}

unsigned char *TextureManager::LoadTextureImage(const std::string &filepath, int &width, int &height, int &bpp, int format) const
{
    return stbi_load(filepath.c_str(), &width, &height, &bpp, format);
}

void TextureManager::FreeTextureImage(unsigned char *data) const
{
    if (data)
    {
        stbi_image_free(data);
    }
}
