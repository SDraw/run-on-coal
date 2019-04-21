#pragma once
#include "Elements/Drawable.h"

namespace ROC
{

class Texture final : public Drawable
{
    int m_type;
    glm::ivec2 m_size;
    GLuint m_texture;

    Texture(const Texture &that) = delete;
    Texture& operator=(const Texture &that) = delete;
public:
    enum TextureType
    {
        TT_None = -1,
        TT_RGB,
        TT_RGBA,
        TT_Cubemap
    };

    inline bool IsTransparent() const { return (m_type == TT_RGBA); }
    inline bool IsCubic() const { return (m_type == TT_Cubemap); }

    inline const glm::ivec2& GetSize() const { return m_size; }
protected:
    Texture();
    ~Texture();

    bool Load(const std::string &f_path, int f_type, int f_filter = DFT_Nearest, bool f_compress = false);
    bool LoadCubemap(const std::vector<std::string> &f_path, int f_filter = DFT_Nearest, bool f_compress = false);
    bool LoadDummy();

    inline GLuint GetTextureID() const { return m_texture; };

    void Bind();

    friend class ElementManager;
    friend class RenderManager;
    friend class AsyncTextureTask;
    friend class Material;
    friend class Shader;
};

}
