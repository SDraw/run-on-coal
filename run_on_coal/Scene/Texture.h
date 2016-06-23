#pragma once

namespace ROC
{

class Texture
{
    GLuint m_texture;
    unsigned int m_type;
public:
    bool Bind(unsigned int f_bind);
    bool IsTransparent();
    bool IsCubic();
    enum TextureType
    {
        RGB = 1U, RGBA, Cubemap
    };
protected:
    Texture();
    ~Texture();
    bool Load(std::string &f_path, unsigned int f_type, bool f_compress);
    bool LoadCubemap(std::vector<std::string> &f_path, bool f_compress);
    GLuint GetTexture();
    friend class ElementManager;
    friend class Material;
    friend class RenderManager;
};

}
