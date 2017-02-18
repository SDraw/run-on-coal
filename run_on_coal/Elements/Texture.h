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
    GLuint m_texture;
    void GenerateBrokenTexture();
public:
    inline bool IsTransparent() const { return (m_type == TEXTURE_TYPE_RGBA); }
    inline bool IsCubic() const { return (m_type == TEXTURE_TYPE_CUBEMAP); }
protected:
    Texture();
    ~Texture();

    bool Load(std::string &f_path, int f_type, unsigned char f_filter, bool f_compress);
    bool LoadCubemap(std::vector<std::string> &f_path, unsigned char f_filter, bool f_compress);

    inline GLuint GetID() const { return m_texture; }
    void Bind(unsigned int f_bind);

    friend class ElementManager;
    friend class Material;
    friend class RenderManager;
    friend class Shader;
};

}
