#include "stdafx.h"

#include "Elements/Texture.h"

#include "Utils/GLBinder.h"

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

#define ROC_TEXTURE_CUBESIDES_COUNT 6

ROC::Texture::Texture()
{
    m_elementType = ET_Texture;

    m_type = TT_None;
    m_filtering = DFT_None;
    m_compressed = false;
    m_texture = 0U;
}
ROC::Texture::~Texture()
{
    if(m_texture != 0U)
    {
        IsCubic() ? GLBinder::ResetTextureCubemap(m_texture) : GLBinder::ResetTexture2D(m_texture);
        glDeleteTextures(1, &m_texture);
    }
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
            btClamp(m_type, static_cast<int>(TT_RGB), static_cast<int>(TT_RGBA));
            m_filtering = f_filter;
            btClamp(m_filtering, static_cast<int>(DFT_Nearest), static_cast<int>(DFT_Linear));
            m_compressed = f_compress;

            const GLuint l_lastTexture2D = GLBinder::GetBindedTexture2D();

            glGenTextures(1, &m_texture);
            GLBinder::BindTexture2D(m_texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST + m_filtering);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST + m_filtering);
            glTexImage2D(GL_TEXTURE_2D, 0, (f_type == TT_RGB) ? (m_compressed ? GL_COMPRESSED_RGB : GL_RGB) : (m_compressed ? GL_COMPRESSED_RGBA : GL_RGBA), m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, l_image.getPixelsPtr());

            GLBinder::BindTexture2D(l_lastTexture2D);
        }
    }
    return (m_texture != 0U);
}
bool ROC::Texture::LoadCubemap(const std::vector<std::string> &f_path, int f_filter, bool f_compress)
{
    if(m_texture == 0U)
    {
        m_type = TT_Cubemap;
        btClamp(m_filtering, static_cast<int>(DFT_Nearest), static_cast<int>(DFT_Linear));
        m_compressed = f_compress;

        const GLuint l_lastTextureCubemap = GLBinder::GetBindedTextureCubemap();

        glGenTextures(1, &m_texture);
        GLBinder::BindTextureCubemap(m_texture);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST + m_filtering);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST + m_filtering);

        for(size_t i = 0U, j = std::min(static_cast<size_t>(ROC_TEXTURE_CUBESIDES_COUNT), f_path.size()); i < j; i++)
        {
            sf::Image l_image;
            if(l_image.loadFromFile(f_path[i]))
            {
                sf::Vector2u l_imageSize = l_image.getSize();
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<int>(i), 0, m_compressed ? GL_COMPRESSED_RGB : GL_RGB, l_imageSize.x, l_imageSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, l_image.getPixelsPtr());
            }
        }

        GLBinder::BindTextureCubemap(l_lastTextureCubemap);
    }
    return (m_texture != 0U);
}
bool ROC::Texture::LoadDummy()
{
    if(m_texture == 0U)
    {
        std::memcpy(&m_size, &g_TextureDummySize, sizeof(glm::ivec2));
        m_filtering = DFT_Nearest;
        m_type = TT_RGB;
        m_compressed = false;

        const GLuint l_lastTexture2D = GLBinder::GetBindedTexture2D();

        glGenTextures(1, &m_texture);
        GLBinder::BindTexture2D(m_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB, g_TextureDummySize.x, g_TextureDummySize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, g_TextureDummyPattern);

        GLBinder::BindTexture2D(l_lastTexture2D);
    }
    return (m_texture != 0U);
}

bool ROC::Texture::IsTransparent() const 
{ 
    return (m_type == TT_RGBA); 
}
bool ROC::Texture::IsCubic() const 
{ 
    return (m_type == TT_Cubemap); 
}
bool ROC::Texture::IsCompressed() const
{
    return m_compressed;
}

void ROC::Texture::Bind()
{
    if(m_texture != 0U)
    {
        switch(m_type)
        {
            case TT_RGB: case TT_RGBA:
                GLBinder::BindTexture2D(m_texture);
                break;
            case TT_Cubemap:
                GLBinder::BindTextureCubemap(m_texture);
                break;
        }
    }
}
