#include "stdafx.h"

#include "Elements/RenderTarget.h"

#include "Utils/GLBinder.h"

ROC::RenderTarget *ROC::RenderTarget::ms_fallbackRT = nullptr;
glm::ivec2 ROC::RenderTarget::ms_fallbackSize = glm::ivec2(0);

ROC::RenderTarget::RenderTarget()
{
    m_elementType = ET_RenderTarget;

    m_type = RTT_None;
    m_filtering = DFT_None;
    m_frameBuffer = 0U;
    m_renderBuffer = 0U;
    m_texture = 0U;
}
ROC::RenderTarget::~RenderTarget()
{
    Clear();
}

bool ROC::RenderTarget::Create(unsigned char f_type, const glm::ivec2 &f_size, unsigned char f_filter)
{
    if(m_type == RTT_None)
    {
        m_type = f_type;
        btClamp<unsigned char>(m_type, RTT_Shadow, RTT_RGBAF);
        m_filtering = f_filter;
        btClamp<unsigned char>(m_filtering, DFT_Nearest, DFT_Linear);

        const GLuint l_lastFramebuffer = GLBinder::GetBindedFramebuffer();

        glGenFramebuffers(1, &m_frameBuffer);
        GLBinder::BindFramebuffer(m_frameBuffer);

        const GLuint l_lastTexture2D = GLBinder::GetBindedTexture2D();

        glGenTextures(1, &m_texture);
        GLBinder::BindTexture2D(m_texture);
        switch(m_type)
        {
            case RTT_Shadow:
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, f_size.x, f_size.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
                //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            } break;
            case RTT_RGB:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, f_size.x, f_size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
                break;
            case RTT_RGBA:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, f_size.x, f_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
                break;
            case RTT_RGBF:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, f_size.x, f_size.y, 0, GL_RGB, GL_FLOAT, NULL);
                break;
            case RTT_RGBAF:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, f_size.x, f_size.y, 0, GL_RGBA, GL_FLOAT, NULL);
                break;
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST + m_filtering);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST + m_filtering);

        if(m_type > RTT_Shadow)
        {
            GLuint l_lastRenderbuffer = GLBinder::GetBindedRenderbuffer();

            glGenRenderbuffers(1, &m_renderBuffer);
            GLBinder::BindRenderbuffer(m_renderBuffer);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, f_size.x, f_size.y);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);

            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);
            glDrawBuffer(GL_COLOR_ATTACHMENT0);

            GLBinder::BindRenderbuffer(l_lastRenderbuffer);
        }
        else
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_texture, 0);
            glDrawBuffer(GL_NONE);
        }
        glReadBuffer(GL_NONE);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            m_error.assign("Unable to create RT, framebuffer status ");
            m_error.append(std::to_string(glCheckFramebufferStatus(GL_FRAMEBUFFER)));
            Clear();
        }
        else std::memcpy(&m_size, &f_size, sizeof(glm::ivec2));

        GLBinder::BindFramebuffer(l_lastFramebuffer);
        GLBinder::BindTexture2D(l_lastTexture2D);
    }
    return (m_type != RTT_None);
}

const glm::ivec2& ROC::RenderTarget::GetSize() const
{
    return m_size;
}

bool ROC::RenderTarget::IsTransparent() const
{
    return ((m_type == RTT_RGBA) || (m_type == RTT_RGBAF));
}
bool ROC::RenderTarget::IsCubic() const
{
    return false;
}
bool ROC::RenderTarget::IsShadowType() const
{
    return (m_type == RTT_Shadow);
}

void ROC::RenderTarget::Bind()
{
    if(m_texture != 0U) GLBinder::BindTexture2D(m_texture);
}
void ROC::RenderTarget::Enable()
{
    if(m_frameBuffer != 0U)
    {
        GLBinder::BindFramebuffer(m_frameBuffer);
        GLBinder::SetViewport(0, 0, m_size.x, m_size.y);
    }
}
void ROC::RenderTarget::Disable()
{
    if(m_frameBuffer != 0U) GLBinder::BindFramebuffer(0U);
}

void ROC::RenderTarget::Clear()
{
    m_type = RTT_None;
    m_filtering = DFT_None;
    if(m_texture != 0U)
    {
        GLBinder::ResetTexture2D(m_texture);
        glDeleteTextures(1, &m_texture);
        m_texture = 0U;
    }
    if(m_renderBuffer != 0U)
    {
        GLBinder::ResetRenderbuffer(m_renderBuffer);
        glDeleteRenderbuffers(1, &m_renderBuffer);
        m_renderBuffer = 0U;
    }
    if(m_frameBuffer != 0U)
    {
        GLBinder::ResetFramebuffer(m_frameBuffer);
        glDeleteFramebuffers(1, &m_frameBuffer);
        m_frameBuffer = 0U;
    }
}

void ROC::RenderTarget::Fallback()
{
    if(ms_fallbackRT) ms_fallbackRT->Enable();
    else
    {
        GLBinder::BindFramebuffer(0U);
        GLBinder::SetViewport(0, 0, ms_fallbackSize.x, ms_fallbackSize.y);
    }
}
