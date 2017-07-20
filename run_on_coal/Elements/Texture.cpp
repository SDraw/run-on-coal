#include "stdafx.h"

#include "Elements/Texture.h"

namespace ROC
{

const std::vector<unsigned char> g_BrokenTextureData = {
    0x7FU, 0x7FU, 0x7FU,
    0xF7U, 0x94U, 0x1DU,
    0xF7U, 0x94U, 0x1DU,
    0x7FU, 0x7FU, 0x7FU
};
const glm::ivec2 g_BrokenTextureSize(2, 2);

}

ROC::Texture::Texture()
{
    m_elementType = ElementType::TextureElement;

    m_type = ROC_TEXTURE_TYPE_NONE;
    m_filtering = ROC_TEXTURE_FILTER_NONE;
    m_texture = 0U;
}
ROC::Texture::~Texture()
{
    if(m_texture) glDeleteTextures(1, &m_texture);
}

void ROC::Texture::Load(const std::string &f_path, int f_type, int f_filter, bool f_compress)
{
    if(m_texture == 0U)
    {
        sf::Image l_image;
        m_type = f_type;
        btClamp(m_type, ROC_TEXTURE_TYPE_RGB, ROC_TEXTURE_TYPE_CUBEMAP);
        m_filtering = f_filter;
        btClamp(m_filtering, ROC_TEXTURE_FILTER_NEAREST, ROC_TEXTURE_FILTER_LINEAR);

        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        if(l_image.loadFromFile(f_path))
        {
            sf::Vector2u l_imageSize = l_image.getSize();
            m_size.x = static_cast<int>(l_imageSize.x);
            m_size.y = static_cast<int>(l_imageSize.y);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST + f_filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST + f_filter);
            glTexImage2D(GL_TEXTURE_2D, 0, (f_type == ROC_TEXTURE_TYPE_RGB) ? (f_compress ? GL_COMPRESSED_RGB : GL_RGB) : (f_compress ? GL_COMPRESSED_RGBA : GL_RGBA), m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, l_image.getPixelsPtr());
        }
        else
        {
            std::memcpy(&m_size, &g_BrokenTextureSize, sizeof(glm::ivec2));

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB, g_BrokenTextureSize.x, g_BrokenTextureSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, g_BrokenTextureData.data());
        }
    }
}
void ROC::Texture::LoadCubemap(const std::vector<std::string> &f_path, int f_filter, bool f_compress)
{
    if(m_texture == 0U)
    {
        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST + f_filter);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST + f_filter);

        m_type = ROC_TEXTURE_TYPE_CUBEMAP;
        m_filtering = f_filter;
        btClamp(m_filtering, ROC_TEXTURE_FILTER_NEAREST, ROC_TEXTURE_FILTER_LINEAR);

        for(size_t i = 0, j = glm::min(6U, f_path.size()); i < j; i++)
        {
            sf::Image l_image;
            if(l_image.loadFromFile(f_path[i]))
            {
                sf::Vector2u l_imageSize = l_image.getSize();
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, f_compress ? GL_COMPRESSED_RGB : GL_RGB, l_imageSize.x, l_imageSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, l_image.getPixelsPtr());
            }
        }
    }
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
