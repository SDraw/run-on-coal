#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/EventManager.h"
#include "Managers/LuaManager.h"
#include "Managers/RenderManager.h"
#include "Managers/SfmlManager.h"
#include "Model/Geometry.h"
#include "Model/Material.h"
#include "Model/Model.h"
#include "Model/Skeleton.h"
#include "Scene/Font.h"
#include "Scene/Quad.h"
#include "Scene/RenderTarget.h"
#include "Scene/Scene.h"
#include "Scene/Shader.h"
#include "Scene/Texture.h"
#include "Lua/LuaArguments.h"

ROC::RenderManager::RenderManager(Core *f_core)
{
    m_core = f_core;

    glEnable (GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    glEnable(GL_CULL_FACE); // [ default culling
    glCullFace(GL_BACK); // [

    glClearColor(0.223529f,0.223529f,0.223529f,0.f);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);

    m_activeScene = NULL;
    m_activeShader = NULL;
    m_activeTarget = NULL;
    m_quad = new Quad();
    m_lastVAO = 0U;
    m_lastTexture = 0U;
    m_depthEnabled = true;
    m_blendEnabled = false;
    m_cullEnabled = true;
    m_time = 0.0;
    m_locked = true;
    m_textureMatrix = glm::mat4(1.f);

    glm::ivec2 l_size;
    m_core->GetSfmlManager()->GetWindowSize(l_size);
    m_screenProjection = glm::ortho(0.f,static_cast<float>(l_size.x),0.f,static_cast<float>(l_size.y));

    m_argument = new LuaArguments();
}
ROC::RenderManager::~RenderManager()
{
    delete m_quad;
    delete m_argument;
}

void ROC::RenderManager::DoPulse()
{
    SfmlManager *l_sfmlManager = m_core->GetSfmlManager();
    m_time = l_sfmlManager->GetTime();

    m_locked = false;
    EventManager *l_eventManager = m_core->GetLuaManager()->GetEventManager();
    if(l_eventManager->IsEventExists(EventType::Render)) l_eventManager->CallEvent(EventType::Render,m_argument);
    m_locked = true;
    l_sfmlManager->SwapBuffers();
}

void ROC::RenderManager::ClearRenderArea(GLbitfield f_params)
{
    if(m_locked) return;
    if((f_params&GL_DEPTH_BUFFER_BIT) == GL_DEPTH_BUFFER_BIT) EnableDepth();
    glClear(f_params);
}

void ROC::RenderManager::SetActiveScene(Scene *f_scene)
{
    if(m_locked) return;
    m_activeScene = f_scene;
    if(m_activeScene)
    {
        if(m_activeShader)
        {
            Camera *l_camera = m_activeScene->GetCamera();
            if(l_camera)
            {
                l_camera->UpdateMatrices();
                m_activeShader->SetProjectionUniformValue(l_camera->m_projectionMatrix);
                m_activeShader->SetViewUniformValue(l_camera->m_viewMatrix);
                m_activeShader->SetCameraPositionUniformValue(l_camera->m_viewPosition);
                m_activeShader->SetCameraDirectionUniformValue(l_camera->m_viewDirection);
            }
            Light *l_light = m_activeScene->GetLight();
            if(l_light)
            {
                m_activeShader->SetLightingUniformValue(1U);
                m_activeShader->SetLightColorUniformValue(l_light->m_color);
                m_activeShader->SetLightDirectionUniformValue(l_light->m_direction);
                m_activeShader->SetLightParamUniformValue(l_light->m_params);
            }
            else m_activeShader->SetLightingUniformValue(0U);
        }
    }
}

void ROC::RenderManager::SetActiveShader(Shader *f_shader)
{
    if(m_locked) return;
    m_activeShader = f_shader;
    if(m_activeShader)
    {
        m_activeShader->Enable(true);
        m_activeShader->SetTimeUniformValue(m_time);
    }
}

void ROC::RenderManager::Render(Model *f_model, bool f_texturize, bool f_frustum, float f_radius)
{
    if(m_locked || !m_activeShader || !m_activeScene || !f_model->IsDrawable()) return;

    if(f_frustum)
    {
        Camera *l_camera = m_activeScene->GetCamera();
        if(!l_camera) return;
        f_model->GetPosition(m_modelPosition,true);
        if(!l_camera->IsInFrustum(m_modelPosition,f_radius)) return;
    }
    m_activeShader->SetModelUniformValue(f_model->m_matrix);

    //Skeletal animation
    if(f_model->HasSkeleton())
    {
		m_activeShader->SetBonesData(f_model->m_skeleton->m_boneMatrices);
        m_activeShader->SetAnimatedUniformValue(1U);
    }
    else m_activeShader->SetAnimatedUniformValue(0U);

    for(auto iter : f_model->m_geometry->m_materialVector)
    {
        unsigned char l_materialType = iter->m_type;
        if((l_materialType&MATERIAL_BIT_DEPTH) != MATERIAL_BIT_DEPTH)
        {
            if(m_activeTarget)
            {
                if(m_activeTarget->IsDepthable()) continue;
            }
            DisableDepth();
        }
        else EnableDepth();
        ((l_materialType&MATERIAL_BIT_TRANSPARENT) == MATERIAL_BIT_TRANSPARENT) ? EnableBlending() : DisableBlending();
        ((l_materialType&MATERIAL_BIT_DOUBLESIDE) == MATERIAL_BIT_DOUBLESIDE) ? DisableCulling() : EnableCulling();

        bool l_textureBind = CompareLastTexture(iter->m_texture->m_texture) && f_texturize;
        bool l_vaoBind = CompareLastVAO(iter->m_VAO);

        if(l_vaoBind)
        {
            m_activeShader->SetMaterialTypeUniformValue(static_cast<int>(l_materialType));
            m_activeShader->SetMaterialParamUniformValue(iter->m_params);
        }
        iter->Draw(l_textureBind,l_vaoBind);
    }
}
void ROC::RenderManager::Render(Font *f_font, glm::vec2 &f_pos, sf::String &f_text, glm::vec4 &f_color)
{
    if(m_locked || !m_activeShader) return;
    EnableBlending();
    DisableDepth();

    m_activeShader->SetProjectionUniformValue(m_screenProjection);
    m_activeShader->SetColorUniformValue(f_color);

    bool l_bind = CompareLastVAO(f_font->m_VAO);
    f_font->Draw(f_text,f_pos,l_bind);
}
void ROC::RenderManager::Render(Texture *f_texture, glm::vec2 &f_pos, glm::vec2 &f_size, float f_rot, glm::vec4 &f_color)
{
    if(m_locked || !m_activeShader || f_texture->IsCubic()) return;

    bool l_vaoBind = CompareLastVAO(m_quad->m_VAO);
    if(CompareLastTexture(f_texture->m_texture)) f_texture->Bind(0U);

    m_activeShader->SetProjectionUniformValue(m_screenProjection);
    m_activeShader->SetColorUniformValue(f_color);

    btTransform l_textureTransform;
    btVector3 l_textureTranslate(f_pos.x+f_size.x/2.f,f_pos.y+f_size.y/2.f,0.f);
    l_textureTransform.setIdentity();
    l_textureTransform.setOrigin(l_textureTranslate);
    if(f_rot != 0.f)
    {
        btVector3 l_textureZAxis(0.f,0.f,1.f);
        btQuaternion l_textureRotation;
        l_textureRotation.setRotation(l_textureZAxis,f_rot);
        l_textureTransform.setRotation(l_textureRotation);
    }
    l_textureTransform.getOpenGLMatrix((float*)&m_textureMatrix);
    m_activeShader->SetModelUniformValue(m_textureMatrix);

    m_quad->SetProportions(f_size,l_vaoBind);
    if(l_vaoBind) l_vaoBind = false;

    DisableCulling();
    DisableDepth();
    if(f_texture->IsTransparent()) EnableBlending();
    else DisableBlending();
    m_quad->Draw(l_vaoBind);
}
void ROC::RenderManager::Render(RenderTarget *f_rt, glm::vec2 &f_pos, glm::vec2 &f_size, float f_rot, glm::vec4 &f_color)
{
    if(m_locked || !m_activeShader || !f_rt->IsColored()) return;

    bool l_vaoBind = CompareLastVAO(m_quad->m_VAO);
    if(CompareLastTexture(f_rt->m_texture)) f_rt->BindTexture(0U);

    m_activeShader->SetProjectionUniformValue(m_screenProjection);
    m_activeShader->SetColorUniformValue(f_color);

    btTransform l_textureTransform;
    btVector3 l_textureTranslate(f_pos.x+f_size.x/2.f,f_pos.y+f_size.y/2.f,0.f);
    l_textureTransform.setIdentity();
    l_textureTransform.setOrigin(l_textureTranslate);
    if(f_rot != 0.f)
    {
        btVector3 l_textureZAxis(0.f,0.f,1.f);
        btQuaternion l_textureRotation;
        l_textureRotation.setRotation(l_textureZAxis,f_rot);
        l_textureTransform.setRotation(l_textureRotation);
    }
    l_textureTransform.getOpenGLMatrix((float*)&m_textureMatrix);
    m_activeShader->SetModelUniformValue(m_textureMatrix);

    m_quad->SetProportions(f_size,l_vaoBind);
    if(l_vaoBind) l_vaoBind = false;

    DisableCulling();
    DisableDepth();
    if(f_rt->IsTransparent()) EnableBlending();
    else DisableBlending();
    m_quad->Draw(l_vaoBind);
}

void ROC::RenderManager::SetRenderTarget(RenderTarget *f_rt)
{
    if(m_locked || m_activeTarget == f_rt) return;
    m_activeTarget = f_rt;
    if(!m_activeTarget)
    {
        glBindFramebuffer(GL_FRAMEBUFFER,NULL);
        m_core->GetSfmlManager()->GetWindowSize(m_renderTargetSize);
    }
    else
    {
        f_rt->Enable();
        f_rt->GetSize(m_renderTargetSize);
    }
    m_screenProjection = glm::ortho(0.f,static_cast<float>(m_renderTargetSize.x),0.f,static_cast<float>(m_renderTargetSize.y));
    glViewport(0,0,m_renderTargetSize.x,m_renderTargetSize.y);
}

void ROC::RenderManager::EnableNonActiveShader(Shader *f_shader)
{
    if(m_activeShader != f_shader) f_shader->Enable();
}
void ROC::RenderManager::RestoreActiveShader(Shader *f_shader)
{
    if(m_activeShader != f_shader && m_activeShader) m_activeShader->Enable();
}

void ROC::RenderManager::DisableDepth()
{
    if(m_depthEnabled)
    {
        glDepthMask(GL_FALSE);
        m_depthEnabled = false;
    }
}
void ROC::RenderManager::EnableDepth()
{
    if(!m_depthEnabled)
    {
        glDepthMask(GL_TRUE);
        m_depthEnabled = true;
    }
}
void ROC::RenderManager::DisableBlending()
{
    if(m_blendEnabled)
    {
        glDisable(GL_BLEND);
        m_blendEnabled = false;
    }
}
void ROC::RenderManager::EnableBlending()
{
    if(!m_blendEnabled)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        m_blendEnabled = true;
    }
}

bool ROC::RenderManager::CompareLastVAO(GLuint f_vao)
{
    if(f_vao == m_lastVAO) return false;
    m_lastVAO = f_vao;
    return true;
}
bool ROC::RenderManager::CompareLastTexture(GLuint f_texture)
{
    if(f_texture == m_lastTexture) return false;
    m_lastTexture = f_texture;
    return true;
}

void ROC::RenderManager::DisableCulling()
{
    if(m_cullEnabled)
    {
        glDisable(GL_CULL_FACE);
        m_cullEnabled = false;
    }
}
void ROC::RenderManager::EnableCulling()
{
    if(!m_cullEnabled)
    {
        glEnable(GL_CULL_FACE);
        m_cullEnabled = true;
    }
}

void ROC::RenderManager::ResetCallsReducing()
{
    m_lastTexture = 0U;
    m_lastVAO = 0U;
}
