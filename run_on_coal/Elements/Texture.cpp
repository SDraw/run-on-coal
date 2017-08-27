#include "stdafx.h"

#include "Elements/Texture.h"

namespace ROC
{

const unsigned char g_TextureDummyPattern[] = {
    0x7FU, 0x7FU, 0x7FU,
    0xF7U, 0x94U, 0x1DU,
    0xF7U, 0x94U, 0x1DU,
    0x7FU, 0x7FU, 0x7FU
};
const glm::ivec2 g_TextureDummySize(2, 2);

}

ROC::Texture::Texture()
{
    m_elementType = ET_Texture;
    m_elementTypeName.assign("Texture");

    m_type = ROC_TEXTURE_TYPE_NONE;
    m_filtering = ROC_TEXTURE_FILTER_NONE;
    m_texture = 0U;
}
ROC::Texture::~Texture()
{
    if(m_texture) glDeleteTextures(1, &m_texture);
}

bool ROC::Texture::Load(const std::string &f_path, int f_type, int f_filter, bool f_compress)
{
    if(m_texture == 0U)
    {
        sf::Image l_image;
        if(l_image.loadFromFile(f_path))
        {
            sf::Vector2u l_imageSize = l_image.getSize();
            m_size.x = static_cast<int>(l_imageSize.x);
            m_size.y = static_cast<int>(l_imageSize.y);

            m_type = f_type;
            btClamp(m_type, ROC_TEXTURE_TYPE_RGB, ROC_TEXTURE_TYPE_CUBEMAP);
            m_filtering = f_filter;
            btClamp(m_filtering, ROC_TEXTURE_FILTER_NEAREST, ROC_TEXTURE_FILTER_LINEAR);

            glGenTextures(1, &m_texture);
            glBindTexture(GL_TEXTURE_2D, m_texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST + m_filtering);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST + m_filtering);
            glTexImage2D(GL_TEXTURE_2D, 0, (f_type == ROC_TEXTURE_TYPE_RGB) ? (f_compress ? GL_COMPRESSED_RGB : GL_RGB) : (f_compress ? GL_COMPRESSED_RGBA : GL_RGBA), m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, l_image.getPixelsPtr());
        }
    }
    return (m_texture != 0U);
}
bool ROC::Texture::LoadCubemap(const std::vector<std::string> &f_path, int f_filter, bool f_compress)
{
    if(m_texture == 0U)
    {
        m_type = ROC_TEXTURE_TYPE_CUBEMAP;
        m_filtering = f_filter;
        btClamp(m_filtering, ROC_TEXTURE_FILTER_NEAREST, ROC_TEXTURE_FILTER_LINEAR);

        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST + m_filtering);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST + m_filtering);

        for(size_t i = 0, j = std::min(6U, f_path.size()); i < j; i++)
        {
            sf::Image l_image;
            if(l_image.loadFromFile(f_path[i]))
            {
                sf::Vector2u l_imageSize = l_image.getSize();
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, f_compress ? GL_COMPRESSED_RGB : GL_RGB, l_imageSize.x, l_imageSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, l_image.getPixelsPtr());
            }
            else
            {
                glDeleteTextures(1, &m_texture);
                m_type = ROC_TEXTURE_TYPE_NONE;
                m_filtering = ROC_TEXTURE_FILTER_NONE;
                m_texture = 0U;
                break;
            }
        }
    }
    return (m_texture != 0U);
}
bool ROC::Texture::LoadDummy()
{
    if(m_texture == 0U)
    {
        std::memcpy(&m_size, &g_TextureDummySize, sizeof(glm::ivec2));
        m_filtering = ROC_TEXTURE_FILTER_NEAREST;
        m_type = ROC_TEXTURE_TYPE_RGB;

        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB, g_TextureDummySize.x, g_TextureDummySize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, g_TextureDummyPattern);
    }
    return (m_texture != 0U);
}

void ROC::Texture::Bind()
{
    if(m_texture != 0U)
    {
        switch(m_type)
        {
            case ROC_TEXTURE_TYPE_RGB: case ROC_TEXTURE_TYPE_RGBA:
                glBindTexture(GL_TEXTURE_2D, m_texture);
                break;
            case ROC_TEXTURE_TYPE_CUBEMAP:
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
                break;
        }
    }
}
