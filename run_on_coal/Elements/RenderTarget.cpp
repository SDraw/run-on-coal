#include "stdafx.h"

#include "Elements/RenderTarget.h"

#include "Utils/GLBinder.h"

ROC::RenderTarget::RenderTarget()
{
    m_elementType = ET_RenderTarget;
    m_elementTypeName.assign("RenderTarget");

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

bool ROC::RenderTarget::Create(int f_type, const glm::ivec2 &f_size, int f_filter)
{
    if(m_type == RTT_None)
    {
        m_type = f_type;
        btClamp(m_type, static_cast<int>(RTT_Shadow), static_cast<int>(RTT_RGBAF));
        m_filtering = f_filter;
        btClamp(m_filtering, static_cast<int>(DFT_Nearest), static_cast<int>(DFT_Linear));

        GLint l_lastFramebuffer = 0;
        glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &l_lastFramebuffer);

        glGenFramebuffers(1, &m_frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

        GLint l_lastTexture = 0;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &l_lastTexture);

        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        switch(m_type)
        {
            case RTT_Shadow:
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, f_size.x, f_size.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
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
            glGenRenderbuffers(1, &m_renderBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, f_size.x, f_size.y);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);

            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);
            glDrawBuffer(GL_COLOR_ATTACHMENT0);
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
        else
        {
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            std::memcpy(&m_size, &f_size, sizeof(glm::ivec2));
        }

        glBindFramebuffer(GL_FRAMEBUFFER, l_lastFramebuffer);
        if(l_lastTexture) glBindTexture(GL_TEXTURE_2D, l_lastTexture);
    }
    return (m_type != RTT_None);
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
    if(m_frameBuffer != 0U) GLBinder::BindFramebuffer(NULL);
}

void ROC::RenderTarget::Clear()
{
    m_type = RTT_None;
    m_filtering = DFT_None;
    if(m_texture != 0U)
    {
        if(GLBinder::IsTextureBinded(m_texture)) GLBinder::ResetTexture();
        glDeleteTextures(1, &m_texture);
        m_texture = 0U;
    }
    if(m_renderBuffer != 0U)
    {
        glDeleteRenderbuffers(1, &m_renderBuffer);
        m_renderBuffer = 0U;
    }
    if(m_frameBuffer != 0U)
    {
        if(GLBinder::IsFramebufferBinded(m_frameBuffer)) GLBinder::ResetFramebuffer();
        glDeleteFramebuffers(1, &m_frameBuffer);
        m_frameBuffer = 0U;
    }
}
