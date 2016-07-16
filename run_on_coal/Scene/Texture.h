#pragma once
#define TEXTURE_TYPE_NONE -1
#define TEXTURE_TYPE_RGB 0
#define TEXTURE_TYPE_RGBA 1
#define TEXTURE_TYPE_CUBEMAP 2

namespace ROC
{

class Texture
{
    GLuint m_texture;
    int m_type;
public:
    void Bind(unsigned int f_bind);
    bool IsTransparent();
    bool IsCubic();
protected:
    Texture();
    ~Texture();
    bool Load(std::string &f_path, int f_type, bool f_compress);
    bool LoadCubemap(std::vector<std::string> &f_path, bool f_compress);
    GLuint GetTexture();
    friend class ElementManager;
    friend class Material;
    friend class RenderManager;
};

}
