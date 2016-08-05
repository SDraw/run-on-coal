#pragma once
#define TEXTURE_TYPE_NONE -1
#define TEXTURE_TYPE_RGB 0
#define TEXTURE_TYPE_RGBA 1
#define TEXTURE_TYPE_CUBEMAP 2

namespace ROC
{

class Texture
{
    int m_type;
public:
    inline bool IsTransparent() { return (m_type == TEXTURE_TYPE_RGBA); }
    inline bool IsCubic() { return (m_type == TEXTURE_TYPE_CUBEMAP); }
protected:
    GLuint m_texture;
    Texture();
    ~Texture();
    bool Load(std::string &f_path, int f_type, bool f_compress);
    bool LoadCubemap(std::vector<std::string> &f_path, bool f_compress);
    void Bind(unsigned int f_bind);
    friend class ElementManager;
    friend class Material;
    friend class RenderManager;
    friend class Shader;
};

}
