#include "stdafx.h"

#include "Elements/RenderTarget.h"
#include "GL/GLTexture2D.h"
#include "GL/GLFramebuffer.h"
#include "GL/GLRenderbuffer.h"

#include "GL/GLSetting.h"
#include "GL/GLViewport.h"

namespace ROC
{

const glm::vec4 g_defaultClearColor(0.f, 0.f, 0.f, 0.f);
const glm::bvec2 g_defaultClearBuffer(true, true);

}

ROC::RenderTarget *ROC::RenderTarget::ms_fallbackRT = nullptr;
glm::ivec2 ROC::RenderTarget::ms_fallbackSize = glm::ivec2(0);

ROC::RenderTarget::RenderTarget()
{
    m_elementType = ET_RenderTarget;

    m_type = RTT_None;
    m_filtering = DFT_None;
    m_framebuffer = nullptr;
    m_renderbuffer = nullptr;
    m_texture = nullptr;

    m_clearColor = g_defaultClearColor;
    m_clearBuffer = g_defaultClearBuffer;
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
        std::memcpy(&m_size, &f_size, sizeof(glm::ivec2));
        btClamp<unsigned char>(m_type, RTT_Shadow, RTT_RGBAF);
        m_filtering = f_filter;
        btClamp<unsigned char>(m_filtering, DFT_Nearest, DFT_Linear);

        m_framebuffer = new GLFramebuffer();
        m_framebuffer->Create();
        m_framebuffer->Bind();

        m_texture = new GLTexture2D();
        switch(m_type)
        {
            case RTT_Shadow:
            {
                m_texture->Create(f_size.x, f_size.y, GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, nullptr, GL_NEAREST + m_filtering);
                m_texture->SetCompareFunc(GL_LESS);
                m_texture->SetCompareMode(GL_COMPARE_REF_TO_TEXTURE);
                m_texture->SetWrap(GL_CLAMP_TO_EDGE);
            } break;
            case RTT_RGB:
                m_texture->Create(f_size.x, f_size.y, GL_RGB, GL_RGB, nullptr, GL_NEAREST + m_filtering);
                break;
            case RTT_RGBA:
                m_texture->Create(f_size.x, f_size.y, GL_RGBA, GL_RGBA, nullptr, GL_NEAREST + m_filtering);
                break;
            case RTT_RGBF:
                m_texture->Create(f_size.x, f_size.y, GL_RGB32F, GL_RGB, nullptr, GL_NEAREST + m_filtering);
                break;
            case RTT_RGBAF:
                m_texture->Create(f_size.x, f_size.y, GL_RGBA32F, GL_RGBA, nullptr, GL_NEAREST + m_filtering);
                break;
        }

        if(m_type > RTT_Shadow)
        {
            m_renderbuffer = new GLRenderbuffer();
            m_renderbuffer->Create(GL_DEPTH_COMPONENT, f_size.x, f_size.y);
            m_renderbuffer->Bind();

            m_framebuffer->SetRenderbuffer(GL_DEPTH_ATTACHMENT, m_renderbuffer->GetName());
            m_framebuffer->SetTexture2D(GL_COLOR_ATTACHMENT0, m_texture->GetName());
            m_framebuffer->SetDrawBuffer(GL_COLOR_ATTACHMENT0);

            GLRenderbuffer::Reset();
        }
        else
        {
            m_framebuffer->SetTexture2D(GL_DEPTH_ATTACHMENT, m_texture->GetName());
            m_framebuffer->SetDrawBuffer(GL_NONE);
        }
        m_framebuffer->SetReadBuffer(GL_NONE);

        if(GLFramebuffer::CheckStatus() != GL_FRAMEBUFFER_COMPLETE) Clear();

        GLFramebuffer::Reset();
    }
    return (m_type != RTT_None);
}

GLTexture2D* ROC::RenderTarget::GetGLTexture() const
{
    return m_texture;
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

bool ROC::RenderTarget::SetProperty(RenderTargetProperty f_prop, const void *f_val)
{
    if(m_framebuffer)
    {
        switch(f_prop)
        {
            case RTP_ClearColor:
                std::memcpy(&m_clearColor, f_val, sizeof(glm::vec4));
                break;
            case RTP_Depth:
                m_clearBuffer[0] = *reinterpret_cast<const bool*>(f_val);
                break;
            case RTP_Color:
                m_clearBuffer[1] = *reinterpret_cast<const bool*>(f_val);
                break;
        }
    }
    return (m_framebuffer != nullptr);
}

void ROC::RenderTarget::Bind(size_t f_slot)
{
    if(m_texture) m_texture->Bind(static_cast<GLenum>(f_slot));
}

void ROC::RenderTarget::Enable(bool f_clear)
{
    if(m_framebuffer)
    {
        m_framebuffer->Bind();
        GLViewport::SetViewport(0, 0, m_size.x, m_size.y);
        if(f_clear)
        {
            if(m_clearBuffer[1] && (m_type > RTT_Shadow)) GLViewport::SetClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
            if(m_clearBuffer[0]) GLSetting::SetDepthMask(true);
            GLViewport::Clear(m_clearBuffer[0], (m_type > RTT_Shadow) ? m_clearBuffer[1] : false);
        }
    }
}

void ROC::RenderTarget::Disable()
{
    GLFramebuffer::Reset();
}

void ROC::RenderTarget::Clear()
{
    m_type = RTT_None;
    m_filtering = DFT_None;
    if(m_texture)
    {
        m_texture->Destroy();
        delete m_texture;
        m_texture = nullptr;
    }
    if(m_renderbuffer)
    {
        m_renderbuffer->Destroy();
        delete m_renderbuffer;
        m_renderbuffer = nullptr;
    }
    if(m_framebuffer)
    {
        m_framebuffer->Destroy();
        delete m_framebuffer;
        m_framebuffer = nullptr;
    }
}

void ROC::RenderTarget::SetFallbackRenderTarget(ROC::RenderTarget *f_rt)
{
    ms_fallbackRT = f_rt;
}

void ROC::RenderTarget::SetFallbackSize(const glm::ivec2 &f_size)
{
    std::memcpy(&ms_fallbackSize, &f_size, sizeof(glm::ivec2));
}

void ROC::RenderTarget::Fallback()
{
    if(ms_fallbackRT) ms_fallbackRT->Enable(false);
    else
    {
        GLFramebuffer::Reset();
        GLViewport::SetViewport(0, 0, ms_fallbackSize.x, ms_fallbackSize.y);
    }
}
