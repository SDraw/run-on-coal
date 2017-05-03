#pragma once
#include "Elements/Drawable.h"
#define RENDERTARGET_TYPE_NONE -1
#define RENDERTARGET_TYPE_DEPTH 0
#define RENDERTARGET_TYPE_RGB 1
#define RENDERTARGET_TYPE_RGBA 2
#define RENDERTARGET_TYPE_RGBF 3
#define RENDERTARGET_TYPE_RGBAF 4
#define RENDERTARGET_FILTER_NONE -1
#define RENDERTARGET_FILTER_NEAREST 0
#define RENDERTARGET_FILTER_LINEAR 1

namespace ROC
{

class RenderTarget : public Drawable
{
    int m_type;
    int m_filtering;

    GLuint m_frameBuffer;
    GLuint m_renderBuffer;
    GLuint m_texture;

    glm::ivec2 m_size;

    std::string m_error;
    void Clear();
public:
    inline void GetSize(glm::ivec2 &f_size) { std::memcpy(&f_size, &m_size, sizeof(glm::ivec2)); }
    inline int GetFiltering() const { return m_filtering; }

    inline bool IsTransparent() const { return ((m_type == RENDERTARGET_TYPE_RGBA) || (m_type == RENDERTARGET_TYPE_RGBAF)); }
    inline bool IsDepthable() const { return (m_type == RENDERTARGET_TYPE_DEPTH); }
protected:
    RenderTarget();
    ~RenderTarget();
    bool Create(unsigned int f_num, const glm::ivec2 &f_size, int f_type, int f_filter);
    inline GLuint GetTextureID() const { return m_texture; }

    void Bind();
    void Enable();

    inline void GetError(std::string &f_str) { f_str.assign(m_error); }
    friend class ElementManager;
    friend class RenderManager;
    friend class Shader;
};

}