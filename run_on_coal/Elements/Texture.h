#pragma once
#include "Elements/Drawable.h"
#define ROC_TEXTURE_TYPE_NONE -1
#define ROC_TEXTURE_TYPE_RGB 0
#define ROC_TEXTURE_TYPE_RGBA 1
#define ROC_TEXTURE_TYPE_CUBEMAP 2
#define ROC_TEXTURE_FILTER_NONE -1
#define ROC_TEXTURE_FILTER_NEAREST 0
#define ROC_TEXTURE_FILTER_LINEAR 1

namespace ROC
{

class Texture final : public Drawable
{
    int m_type;
    glm::ivec2 m_size;
    int m_filtering;
    GLuint m_texture;
public:
    inline bool IsTransparent() const { return (m_type == ROC_TEXTURE_TYPE_RGBA); }
    inline const glm::ivec2& GetSize() const { return m_size; }
    inline int GetFiltering() const { return m_filtering; }
protected:
    Texture();
    ~Texture();
    void Load(const std::string &f_path, int f_type, int f_filter, bool f_compress);
    void LoadCubemap(const std::vector<std::string> &f_path, int f_filter, bool f_compress);

    inline GLuint GetTextureID() const { return m_texture; }

    void Bind();

    friend class ElementManager;
    friend class Material;
    friend class RenderManager;
    friend class Shader;
};

}
