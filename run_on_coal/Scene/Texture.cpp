#include "stdafx.h"
#include "Scene/Texture.h"

ROC::Texture::Texture()
{
    m_type = TEXTURE_TYPE_NONE;
    m_texture = 0U;
}
ROC::Texture::~Texture()
{
    if(m_texture) glDeleteTextures(1, &m_texture);
}

bool ROC::Texture::Load(std::string &f_path, int f_type, unsigned char f_filter, bool f_compress)
{
    if(m_type != TEXTURE_TYPE_NONE || f_type <= TEXTURE_TYPE_NONE || f_type >= TEXTURE_TYPE_CUBEMAP) return false;
    sf::Image l_image;
    m_type = f_type;
    if(l_image.loadFromFile(f_path))
    {
        sf::Vector2u l_imageSize = l_image.getSize();
        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST+f_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST+f_filter);
        glTexImage2D(GL_TEXTURE_2D, 0, (f_type == TEXTURE_TYPE_RGB) ? (f_compress ? GL_COMPRESSED_RGB : GL_RGB) : (f_compress ? GL_COMPRESSED_RGBA : GL_RGBA), l_imageSize.x, l_imageSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, l_image.getPixelsPtr());
    }
    else GenerateBrokenTexture();
    return true;
}
bool ROC::Texture::LoadCubemap(std::vector<std::string> &f_path, unsigned char f_filter, bool f_compress)
{
    if(m_type != TEXTURE_TYPE_NONE || f_path.size() != 6U) return false;
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST+f_filter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST+f_filter);

    m_type = TEXTURE_TYPE_CUBEMAP;
    for(int i = 0; i < 6; i++)
    {
        sf::Image l_image;
        if(!l_image.loadFromFile(f_path[i]))
        {
            glDeleteTextures(1, &m_texture);
            return false;
        }
        sf::Vector2u l_imageSize = l_image.getSize();
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, f_compress ? GL_COMPRESSED_RGB : GL_RGB, l_imageSize.x, l_imageSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, l_image.getPixelsPtr());
    }
    return true;
}
void ROC::Texture::GenerateBrokenTexture()
{
    std::vector<unsigned char> l_brokenImage;
    for(int i = 0; i < 3; i++) l_brokenImage.push_back(0x7FU);
    for(int i = 0; i < 2; i++)
    {
        l_brokenImage.push_back(0xF7U);
        l_brokenImage.push_back(0x94U);
        l_brokenImage.push_back(0x1DU);
    }
    for(int i = 0; i < 3; i++) l_brokenImage.push_back(0x7FU);
    m_type = TEXTURE_TYPE_RGB;
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2U, 2U, 0, GL_RGB, GL_UNSIGNED_BYTE, l_brokenImage.data());
}

void ROC::Texture::Bind(unsigned int f_bind)
{
    if(m_type == TEXTURE_TYPE_NONE) return;
    if(f_bind) glActiveTexture(GL_TEXTURE0 + f_bind);
    switch(m_type)
    {
        case TEXTURE_TYPE_RGB: case TEXTURE_TYPE_RGBA:
            glBindTexture(GL_TEXTURE_2D, m_texture);
            break;
        case TEXTURE_TYPE_CUBEMAP:
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
            break;
    }
    if(f_bind) glActiveTexture(GL_TEXTURE0);
}