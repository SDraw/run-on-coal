#include "stdafx.h"
#include "Scene/Texture.h"

ROC::Texture::Texture()
{
    m_type = TEXTURE_TYPE_NONE;
    m_texture = 0U;
}
ROC::Texture::~Texture()
{
    if(m_texture) glDeleteTextures(1,&m_texture);
}

bool ROC::Texture::Load(std::string &f_path, int f_type, bool f_compress)
{
    unsigned l_width, l_height;
    std::vector<unsigned char> l_texture;
    bool l_fail = false;
    if(lodepng::decode(l_texture,l_width,l_height,f_path,(f_type == TEXTURE_TYPE_RGB) ? LCT_RGB:LCT_RGBA))
    {
        //Let's use grey and orange tile for failed texture
        for(int i=0; i < 3; i++) l_texture.push_back(0x7F);
        for(int i=0; i < 2; i++)
        {
            l_texture.push_back(0xF7);
            l_texture.push_back(0x94);
            l_texture.push_back(0x1D);
        }
        for(int i=0; i < 3; i++) l_texture.push_back(0x7F);
        l_width = l_height = 2U;
        f_type = TEXTURE_TYPE_RGB;
        l_fail = true;
    }
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, l_fail ? GL_NEAREST:GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, l_fail ? GL_NEAREST:GL_LINEAR );
    glTexImage2D(GL_TEXTURE_2D,0,(f_type == TEXTURE_TYPE_RGB) ? (f_compress ? GL_COMPRESSED_RGB : GL_RGB) : (f_compress ? GL_COMPRESSED_RGBA : GL_RGBA),l_width,l_height,0,(f_type == TEXTURE_TYPE_RGB) ? GL_RGB:GL_RGBA,GL_UNSIGNED_BYTE,l_texture.data());
    l_texture.clear();

    m_type = f_type;
    return true;
}
bool ROC::Texture::LoadCubemap(std::vector<std::string> &f_path, bool f_compress)
{
    if(f_path.size() != 6) return false;
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    unsigned l_width, l_height;
    std::vector<unsigned char> l_texture;
    for(int i = 0; i < 6; i++)
    {
        if(lodepng::decode(l_texture,l_width,l_height,f_path[i],LCT_RGB)) return false;
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,f_compress ? GL_COMPRESSED_RGB : GL_RGB,l_width,l_height,0,GL_RGB,GL_UNSIGNED_BYTE,l_texture.data());
        l_texture.clear();
    }

    m_type = TEXTURE_TYPE_CUBEMAP;
    return true;
}

bool ROC::Texture::Bind(unsigned int f_bind)
{
    if(f_bind) glActiveTexture(GL_TEXTURE0+f_bind);
    switch(m_type)
    {
        case TEXTURE_TYPE_RGB: case TEXTURE_TYPE_RGBA:
            glBindTexture(GL_TEXTURE_2D,m_texture);
            break;
        case TEXTURE_TYPE_CUBEMAP:
            glBindTexture(GL_TEXTURE_CUBE_MAP,m_texture);
            break;
    }
    if(f_bind) glActiveTexture(GL_TEXTURE0);
    return true;
}

bool ROC::Texture::IsTransparent()
{
    return (m_type == TEXTURE_TYPE_RGBA);
}
bool ROC::Texture::IsCubic()
{
    return (m_type == TEXTURE_TYPE_CUBEMAP);
}

GLuint ROC::Texture::GetTexture()
{
    return m_texture;
}
