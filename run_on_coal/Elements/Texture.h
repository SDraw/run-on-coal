#pragma once
#include "Elements/Element.h"
#define TEXTURE_TYPE_NONE -1
#define TEXTURE_TYPE_RGB 0
#define TEXTURE_TYPE_RGBA 1
#define TEXTURE_TYPE_CUBEMAP 2
#define TEXTURE_FILTER_NONE -1
#define TEXTURE_FILTER_NEAREST 0
#define TEXTURE_FILTER_LINEAR 1

namespace ROC
{

class Texture : public Element
{
    int m_type;
    glm::ivec2 m_size;
    int m_filtering;
    GLuint m_texture;
    void GenerateBrokenTexture();
public:
    inline bool IsTransparent() const { return (m_type == TEXTURE_TYPE_RGBA); }
    inline bool IsCubic() const { return (m_type == TEXTURE_TYPE_CUBEMAP); }
    inline void GetSize(glm::ivec2 &f_size) { std::memcpy(&f_size, &m_size, sizeof(glm::ivec2)); }
    inline int GetFiltering() const { return m_filtering; }
protected:
    Texture();
    ~Texture();

    bool Load(std::string &f_path, int f_type, int f_filter, bool f_compress);
    bool LoadCubemap(std::vector<std::string> &f_path, int f_filter, bool f_compress);

    inline GLuint GetID() const { return m_texture; }
    void Bind(unsigned int f_bind);

    friend class ElementManager;
    friend class Material;
    friend class RenderManager;
    friend class Shader;
};

}
