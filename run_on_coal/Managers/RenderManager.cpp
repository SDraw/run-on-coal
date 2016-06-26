#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/EventManager.h"
#include "Managers/GlfwManager.h"
#include "Managers/LuaManager.h"
#include "Managers/RenderManager.h"
#include "Model/Material.h"
#include "Model/Model.h"
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

    m_currentScene = NULL;
    m_currentShader = NULL;
    m_currentRT = NULL;
    m_quad = new Quad();
    m_lastVAO = 0U;
    m_lastTexture = 0U;
    m_depthEnabled = true;
    m_blendEnabled = false;
    m_cullEnabled = true;
    m_time = 0.0;
    m_locked = true;

    glm::ivec2 l_size;
    m_core->GetGlfwManager()->GetFramebufferSize(l_size);
    m_screenProjection = glm::ortho(0.f,static_cast<float>(l_size.x),0.f,static_cast<float>(l_size.y));
}
ROC::RenderManager::~RenderManager()
{
    delete m_quad;
}

void ROC::RenderManager::DoPulse()
{
    GlfwManager *l_glfwManager = m_core->GetGlfwManager();
    m_time = float(l_glfwManager->GetTime());

    m_locked = false;
    EventManager *l_eventManager = m_core->GetLuaManager()->GetEventManager();
    if(l_eventManager->IsEventExists(EventType::Render))
    {
        LuaArguments l_args;
        l_eventManager->CallEvent(EventType::Render,l_args);
    }
    m_locked = true;
    l_glfwManager->SwapBuffers();
}

void ROC::RenderManager::ClearRenderArea(GLbitfield f_params)
{
    if(m_locked) return;
    std::bitset<sizeof(GLbitfield)*8> l_bit(f_params);
    if(l_bit.test(8)) EnableDepth();
    glClear(f_params);
}
void ROC::RenderManager::SetClearColour(glm::vec4 &f_color)
{
    glClearColor(f_color.r,f_color.g,f_color.b,f_color.a);
}

void ROC::RenderManager::SetViewport(glm::ivec4 &f_area)
{
    if(m_locked) return;
    glViewport(f_area.r,f_area.g,f_area.b,f_area.a);
}

void ROC::RenderManager::SetCurrentScene(Scene *f_scene)
{
    if(m_locked) return;
    m_currentScene = f_scene;
    if(m_currentScene)
    {
        if(m_currentShader)
        {
            Camera *l_camera = m_currentScene->GetCamera();
            if(l_camera)
            {
                glm::mat4 l_matrix;
                glm::vec3 l_vec;
                l_camera->GetProjectionMatrix(l_matrix);
                m_currentShader->SetProjectionUniformValue(l_matrix);
                l_camera->GetViewMatrix(l_matrix);
                m_currentShader->SetViewUniformValue(l_matrix);
                l_camera->GetPosition(l_vec);
                m_currentShader->SetCameraPositionUniformValue(l_vec);
                l_camera->GetDirection(l_vec);
                m_currentShader->SetCameraDirectionUniformValue(l_vec);
            }
            Light *l_light = m_currentScene->GetLight();
            if(l_light)
            {
                glm::vec3 l_vec;
                glm::vec4 l_params;
                m_currentShader->SetLightingUniformValue(1U);
                l_light->GetColor(l_vec);
                m_currentShader->SetLightColorUniformValue(l_vec);
                l_light->GetDirection(l_vec);
                m_currentShader->SetLightDirectionUniformValue(l_vec);
                l_light->GetParams(l_params);
                m_currentShader->SetLightParamUniformValue(l_params);
            }
            else m_currentShader->SetLightingUniformValue(0U);
        }
    }
}

void ROC::RenderManager::CheckAndRemoveSceneIfCurrent(Scene *f_scene)
{
    if(m_currentScene == f_scene) m_currentScene = NULL;
}

void ROC::RenderManager::SetCurrentShader(Shader *f_shader)
{
    if(m_locked) return;
    m_currentShader = f_shader;
    if(m_currentShader) m_currentShader->Enable();
}

void ROC::RenderManager::CheckShaderForCurrent(Shader *f_shader)
{
    if(m_currentShader == f_shader) m_currentShader = NULL;
}

void ROC::RenderManager::Render(Model *f_model, bool f_texturize)
{
    if(m_locked || !m_currentShader || !m_currentScene || !f_model->IsDrawable()) return;

    glm::mat4 l_matrix;
    f_model->GetMatrix(l_matrix);
    m_currentShader->SetModelUniformValue(l_matrix);

    m_currentShader->SetTimeUniformValue(m_time);

    //Skeletal animation
    if(f_model->HasSkeleton())
    {
        std::vector<glm::mat4> l_mats;
        f_model->GetBoneMatrices(l_mats);
        m_currentShader->SetBonesUniformValue(l_mats);
        m_currentShader->SetAnimatedUniformValue(1U);
    }
    else m_currentShader->SetAnimatedUniformValue(0U);

    for(unsigned int i=0, j=f_model->GetMaterialCount(); i < j; i++)
    {
        bool l_vaoBind = (j == 1U) ? CompareLastVAO(f_model->GetMaterialVAO(i)) : true;
        bool l_textureBind = (j == 1U) ? (CompareLastTexture(f_model->GetMaterialTexture(i)) && f_texturize) : f_texturize;

        std::bitset<8U> l_materialType;
        f_model->GetMaterialType(i,l_materialType);
        if(!l_materialType.test(1))
        {
            if(m_currentRT)
            {
                if(m_currentRT->IsDepthable()) continue;
            }
            DisableDepth();
        }
        else EnableDepth();
        l_materialType.test(2) ? EnableBlending() : DisableBlending();
        l_materialType.test(3) ? DisableCulling() : EnableCulling();

        if(l_vaoBind)
        {
            glm::vec4 l_mvec(1.f);
            m_currentShader->SetMaterialTypeUniformValue(static_cast<int>(l_materialType.to_ulong()));
            f_model->GetMaterialParam(i,l_mvec);
            m_currentShader->SetMaterialParamUniformValue(l_mvec);
        }

        f_model->DrawMaterial(i,l_textureBind,l_vaoBind);
    }
}
void ROC::RenderManager::Render(Font *f_font, glm::vec2 &f_pos, std::wstring &f_text, glm::vec4 &f_color)
{
    if(m_locked || !m_currentShader) return;
    EnableBlending();
    DisableDepth();

    m_currentShader->SetTimeUniformValue(m_time);
    m_currentShader->SetProjectionUniformValue(m_screenProjection);
    m_currentShader->SetColorUniformValue(f_color);

    bool l_bind = CompareLastVAO(f_font->GetVAO());
    f_font->Draw(f_text,f_pos,l_bind);
}
void ROC::RenderManager::Render(Texture *f_texture, glm::vec2 &f_pos, glm::vec2 &f_size, float f_rot)
{
    if(m_locked || !m_currentShader || f_texture->IsCubic()) return;

    bool l_vaoBind = CompareLastVAO(m_quad->GetVAO());
    if(CompareLastTexture(f_texture->GetTexture())) f_texture->Bind(0U);

    m_currentShader->SetProjectionUniformValue(m_screenProjection);
    m_currentShader->SetTimeUniformValue(m_time);

    glm::mat4 l_identity(1.f);
    glm::mat4 l_model;
    if(f_rot != 0.f)
    {
        l_model = glm::rotate(l_identity,f_rot,glm::vec3(0.f,0.f,1.f));
        l_model = glm::translate(l_identity,glm::vec3(f_pos+f_size/2.f,0.f))*l_model;
    }
    else l_model = glm::translate(l_identity,glm::vec3(f_pos+f_size/2.f,0.f));
    m_currentShader->SetModelUniformValue(l_model);

    m_quad->SetProportions(f_size,l_vaoBind);
    if(l_vaoBind) l_vaoBind = false;

    DisableCulling();
    DisableDepth();
    if(f_texture->IsTransparent()) EnableBlending();
    else DisableBlending();
    m_quad->Draw(l_vaoBind);
}
void ROC::RenderManager::Render(RenderTarget *f_rt, glm::vec2 &f_pos, glm::vec2 &f_size, float f_rot)
{
    if(m_locked || !m_currentShader || !f_rt->IsColored()) return;

    bool l_vaoBind = CompareLastVAO(m_quad->GetVAO());
    if(CompareLastTexture(f_rt->GetTexture())) f_rt->BindTexture(0U);

    m_currentShader->SetProjectionUniformValue(m_screenProjection);
    m_currentShader->SetTimeUniformValue(m_time);

    glm::mat4 l_identity(1.f);
    glm::mat4 l_model;
    if(f_rot != 0.f)
    {
        l_model = glm::rotate(l_identity,f_rot,glm::vec3(0.f,0.f,1.f));
        l_model = glm::translate(l_identity,glm::vec3(f_pos+f_size/2.f,0.f))*l_model;
    }
    else l_model = glm::translate(l_identity,glm::vec3(f_pos+f_size/2.f,0.f));
    m_currentShader->SetModelUniformValue(l_model);

    m_quad->SetProportions(f_size,l_vaoBind);
    if(l_vaoBind) l_vaoBind = false;

    DisableCulling();
    DisableDepth();
    if(f_rt->IsTransparent()) EnableBlending();
    else DisableBlending();
    m_quad->Draw(l_vaoBind);
}

void ROC::RenderManager::SetPolygonMode(unsigned int f_mode)
{
    if(f_mode > 2U) return;
    glPolygonMode(GL_FRONT_AND_BACK,GL_POINT+f_mode);
}

void ROC::RenderManager::SetRenderTarget(RenderTarget *f_rt)
{
    if(m_locked || m_currentRT == f_rt) return;
    m_currentRT = f_rt;
    glm::ivec2 l_size;
    if(!m_currentRT)
    {
        glBindFramebuffer(GL_FRAMEBUFFER,NULL);
        m_core->GetGlfwManager()->GetFramebufferSize(l_size);
    }
    else
    {
        f_rt->Enable();
        f_rt->GetSize(l_size);
    }
    m_screenProjection = glm::ortho(0.f,static_cast<float>(l_size.x),0.f,static_cast<float>(l_size.y));
    glViewport(0,0,l_size.x,l_size.y);
}

void ROC::RenderManager::CheckShader(Shader *f_shader)
{
    if(m_currentShader != f_shader) f_shader->Enable();
}
void ROC::RenderManager::RestoreShader(Shader *f_shader)
{
    if(m_currentShader)
    {
        if(m_currentShader != f_shader) m_currentShader->Enable();
    }
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
