#pragma once
#include "Interfaces/ITexture.h"
#include "Elements/Drawable.h"

namespace ROC
{

class Texture final : public Drawable, public virtual ITexture
{
    unsigned char m_type;
    glm::ivec2 m_size;
    bool m_compressed;
    GLuint m_texture;

    Texture(const Texture &that) = delete;
    Texture& operator=(const Texture &that) = delete;
public:
    bool IsTransparent() const;
    bool IsCubic() const;
    bool IsCompressed() const;

    inline const glm::ivec2& GetSize() const { return m_size; }
protected:
    Texture();
    ~Texture();

    bool Load(const std::string &f_path, unsigned char f_type, unsigned char f_filter, bool f_compress);
    bool LoadCubemap(const std::vector<std::string> &f_path, unsigned char f_filter, bool f_compress);
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
