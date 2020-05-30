#include "stdafx.h"

#include "Managers/RenderManager/RenderManager.h"
#include "Core/Core.h"
#include "Managers/RenderManager/Quad2D.h"
#include "Managers/RenderManager/Quad3D.h"
#include "Managers/RenderManager/PhysicsDrawer.h"
#include "Elements/Font.h"
#include "Elements/Scene/Scene.h"
#include "Elements/Texture.h"
#include "Utils/CustomArguments.h"

#include "Managers/ConfigManager.h"
#include "Managers/LogManager.h"
#include "Managers/PhysicsManager.h"
#include "Managers/SfmlManager.h"
#include "Managers/VRManager.h"
#include "Managers/ModuleManager.h"
#include "Elements/Camera.h"
#include "Elements/Geometry/Geometry.h"
#include "Elements/Geometry/Material.h"
#include "Elements/Light.h"
#include "Elements/Model/Model.h"
#include "Elements/Model/Skeleton.h"
#include "Elements/RenderTarget.h"
#include "Elements/Scene/SceneLayer.h"
#include "Elements/Shader/Shader.h"
#include "Interfaces/IModule.h"
#include "GL/GLViewport.h"
#include "GL/GLSetting.h"

namespace ROC
{

extern const glm::mat4 g_IdentityMatrix;
extern const glm::vec4 g_EmptyVec4;

const btVector3 g_TextureZAxis(0.f, 0.f, 1.f);

}

ROC::RenderManager::RenderManager(Core *f_core)
{
    m_core = f_core;
    m_vrManager = m_core->GetVRManager();

    GLSetting::Set(GL_DEPTH_TEST, true);
    GLSetting::Set(GL_TEXTURE_CUBE_MAP_SEAMLESS, true);
    GLSetting::Set(GL_CULL_FACE, true); // default culling
    GLSetting::SetDepthMask(true);
    GLSetting::SetPixelUnpackAlignment(1);
    if(m_core->GetConfigManager()->GetAntialiasing() > 0) GLSetting::Set(GL_LINE_SMOOTH, true);

    GLViewport::SetClearColor(0.223529f, 0.223529f, 0.223529f, 0.f);
    GLViewport::SetBlendFunctions(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Font::InitStaticResources();
    Shader::InitStaticResources();
    Material::InitStaticResources();

    m_renderStage = RS_None;
    m_clearFrame = false;
    m_activeScene = nullptr;
    m_quad2D = new Quad2D();
    m_quad3D = new Quad3D();
    m_physicsDrawer = new PhysicsDrawer();
    m_core->GetPhysicsManager()->SetDebugDrawer(m_physicsDrawer);

    m_time = 0.f;

    m_core->GetSfmlManager()->GetWindowSize(m_viewportSize);
    m_screenProjection = glm::ortho(0.f, static_cast<float>(m_viewportSize.x), 0.f, static_cast<float>(m_viewportSize.y));

    m_arguments = new CustomArguments();
}

ROC::RenderManager::~RenderManager()
{
    delete m_quad2D;
    delete m_quad3D;
    delete m_physicsDrawer;
    Font::ReleaseStaticResources();
    Material::ReleaseStaticResources();
    delete m_arguments;
}

bool ROC::RenderManager::SetActiveScene(Scene *f_scene)
{
    if(m_renderStage != RS_None)
    {
        if(m_activeScene) m_activeScene->Disable();
        m_activeScene = f_scene;

        if(m_activeScene)
        {
            m_activeScene->Enable();
            if(!m_activeScene->HasRenderTarget())
            {
                if(m_renderStage == RS_VR) m_vrManager->RestoreRenderTarget();
                else
                {
                    GLViewport::SetViewport(0, 0, m_viewportSize.x, m_viewportSize.y);
                    if(m_clearFrame)
                    {
                        ClearViewport();
                        m_clearFrame = false;
                    }
                }
            }
        }
    }
    return (m_renderStage != RS_None);
}

void ROC::RenderManager::RemoveAsActiveScene(Scene *f_scene)
{
    if(m_activeScene == f_scene)
    {
        m_activeScene->Disable();
        m_activeScene = nullptr;
    }
}

void ROC::RenderManager::UpdateViewportSize(const glm::ivec2 &f_size)
{
    std::memcpy(&m_viewportSize, &f_size, sizeof(glm::ivec2));
    m_screenProjection = glm::ortho(0.f, static_cast<float>(m_viewportSize.x), 0.f, static_cast<float>(m_viewportSize.y));
    RenderTarget::SetFallbackSize(m_viewportSize);
}

bool ROC::RenderManager::DrawScene(Scene *f_scene)
{
    bool l_result = false;
    if((m_renderStage != RS_None) && (m_activeScene == f_scene))
    {
        if(m_activeScene->HasCamera())
        {
            bool l_onShadowRT = (m_activeScene->HasRenderTarget() ? m_activeScene->GetRenderTarget()->IsShadowType() : false);

            for(const auto l_layer : m_activeScene->GetLayers())
            {
                if(l_layer->HasModels())
                {
                    Shader *l_shader = l_layer->GetShader();
                    if(l_shader)
                    {
                        // Update shader data
                        Camera *l_camera = m_activeScene->GetCamera();

                        l_shader->Enable();
                        l_shader->SetTime(m_time);
                        l_shader->SetProjectionMatrix(l_camera->GetProjectionMatrix());
                        l_shader->SetViewMatrix(l_camera->GetViewMatrix());
                        l_shader->SetViewProjectionMatrix(l_camera->GetViewProjectionMatrix());
                        l_shader->SetCameraPosition(l_camera->GetPosition());
                        l_shader->SetCameraDirection(l_camera->GetDirection());
                        l_shader->SetLightsData(m_activeScene->GetLights());

                        for(const auto &l_renderModel : l_layer->GetRenderModels())
                        {
                            if(l_renderModel->m_visible)
                            {
                                Model *l_model = l_renderModel->m_model;
                                if(l_model->HasGeometry())
                                {
                                    l_shader->SetModelMatrix(l_model->GetFullMatrix());

                                    if(l_model->HasSkeleton())
                                    {
                                        l_shader->SetBoneMatrices(l_model->GetSkeleton()->GetPoseMatrices());
                                        l_shader->SetAnimated(true);
                                    }
                                    else l_shader->SetAnimated(false);

                                    for(const auto l_material : l_model->GetGeometry()->GetMaterials())
                                    {
                                        if(!l_material->HasDepth() && l_onShadowRT) continue;

                                        l_shader->SetMaterialType(static_cast<int>(l_material->GetType()));
                                        l_shader->SetMaterialParam(l_material->GetParams());
                                        l_material->Draw(!l_onShadowRT);
                                    }
                                }
                            }
                        }

                        l_shader->Disable();
                    }
                }
            }
            l_result = true;
        }
    }
    return l_result;
}

bool ROC::RenderManager::Render(Font *f_font, const glm::vec2 &f_pos, const std::string &f_text, const glm::vec4 &f_color, const std::string &f_layer)
{
    bool l_result = false;
    if((m_renderStage != RS_None) && m_activeScene)
    {
        const ROC::SceneLayer *l_layer = m_activeScene->GetLayer(f_layer);
        if(l_layer)
        {
            Shader *l_shader = l_layer->m_shader;
            if(l_shader)
            {
                l_shader->Enable();
                l_shader->SetTime(m_time);

                l_shader->SetProjectionMatrix(m_screenProjection);
                l_shader->SetModelMatrix(g_IdentityMatrix);
                l_shader->SetColor(f_color);

                sf::String l_textUtf8 = sf::String::fromUtf8(f_text.begin(), f_text.end());
                f_font->Draw(l_textUtf8, f_pos);

                l_shader->Disable();

                l_result = true;
            }
        }
    }
    return l_result;
}

bool ROC::RenderManager::Render(Drawable *f_drawable, const glm::vec2 &f_pos, const glm::vec2 &f_size, float f_rot, const glm::vec4 &f_color, const std::string &f_layer)
{
    bool l_result = false;
    if((m_renderStage != RS_None) && m_activeScene)
    {
        const ROC::SceneLayer *l_layer = m_activeScene->GetLayer(f_layer);
        if(l_layer)
        {
            Shader *l_shader = l_layer->GetShader();
            if(l_shader)
            {
                l_shader->Enable();
                l_shader->SetTime(m_time);
                l_shader->SetProjectionMatrix(m_screenProjection);

                btTransform l_textureTransform;
                btVector3 l_textureTranslate(f_pos.x, f_pos.y, 0.f);
                l_textureTransform.setIdentity();
                l_textureTransform.setOrigin(l_textureTranslate);
                if(f_rot != 0.f)
                {
                    btQuaternion l_textureRotation;
                    l_textureRotation.setRotation(g_TextureZAxis, f_rot);
                    l_textureTransform.setRotation(l_textureRotation);
                }
                glm::mat4 l_textureMatrix;
                l_textureTransform.getOpenGLMatrix(glm::value_ptr(l_textureMatrix));
                l_shader->SetModelMatrix(l_textureMatrix);
                l_shader->SetColor(f_color);

                GLSetting::Set(GL_BLEND, f_drawable->IsTransparent());
                f_drawable->Bind();
                m_quad2D->SetTransformation(f_size);
                m_quad2D->Draw();

                l_shader->Disable();

                l_result = true;
            }
        }
    }
    return l_result;
}

bool ROC::RenderManager::Render(Drawable *f_drawable, const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec2 &f_size, const std::string &f_layer, const glm::bvec4 &f_params)
{
    bool l_result = false;
    if((m_renderStage != RS_None) && m_activeScene)
    {
        if(m_activeScene->HasCamera())
        {
            const ROC::SceneLayer *l_layer = m_activeScene->GetLayer(f_layer);
            if(l_layer)
            {
                Shader *l_shader = l_layer->GetShader();
                if(l_shader)
                {
                    glm::vec2 l_halfSize = f_size / 2.f;
                    float l_radius = glm::length(l_halfSize);
                    if(m_activeScene->GetCamera()->IsInFrustum(f_pos, l_radius))
                    {
                        l_shader->Enable();
                        l_shader->SetTime(m_time);

                        l_shader->SetAnimated(false);
                        l_shader->SetModelMatrix(m_quad3D->GetMatrix());
                        l_shader->SetMaterialParam(g_EmptyVec4);

                        int l_materialType = 0;
                        if(f_params.x) l_materialType |= Material::MPB_Shading;
                        if(f_params.y) l_materialType |= Material::MPB_Depth;
                        if(f_params.z) l_materialType |= Material::MPB_Transparency;
                        if(f_params.w) l_materialType |= Material::MPB_Doubleside;
                        l_shader->SetMaterialType(l_materialType);

                        GLSetting::Set(GL_CULL_FACE, !f_params.w);
                        GLSetting::SetDepthMask(f_params.y);
                        GLSetting::Set(GL_BLEND, (f_drawable->IsTransparent() && f_params.z));
                        f_drawable->Bind();
                        m_quad3D->SetTransformation(f_pos, f_rot, f_size);
                        m_quad3D->Draw();

                        l_shader->Disable();

                        l_result = true;
                    }
                }
            }
        }
    }
    return l_result;
}

bool ROC::RenderManager::DrawPhysics(float f_width, const std::string &f_layer)
{
    bool l_result = false;
    if((m_renderStage != RS_None) && m_activeScene)
    {
        if(m_activeScene->HasCamera())
        {
            const ROC::SceneLayer *l_layer = m_activeScene->GetLayer(f_layer);
            if(l_layer)
            {
                Shader *l_shader = l_layer->GetShader();
                if(l_shader)
                {
                    l_shader->Enable();

                    Camera *l_camera = m_activeScene->GetCamera();
                    l_shader->SetProjectionMatrix(l_camera->GetProjectionMatrix());
                    l_shader->SetViewMatrix(l_camera->GetViewMatrix());
                    l_shader->SetViewProjectionMatrix(l_camera->GetViewProjectionMatrix());
                    l_shader->SetCameraPosition(l_camera->GetPosition());
                    l_shader->SetCameraDirection(l_camera->GetDirection());
                    l_shader->SetModelMatrix(g_IdentityMatrix);

                    l_shader->SetAnimated(false);
                    l_shader->SetMaterialType((Material::MPB_Depth | Material::MPB_Doubleside));
                    l_shader->SetMaterialParam(g_EmptyVec4);

                    m_core->GetPhysicsManager()->DrawDebugWorld();
                    m_physicsDrawer->Draw(f_width);

                    l_shader->Disable();

                    l_result = true;
                }
            }
        }
    }
    return l_result;
}

bool ROC::RenderManager::SetViewport(const glm::ivec4 &f_area)
{
    if(m_renderStage != RS_None) GLViewport::SetViewport(f_area.x, f_area.y, f_area.z, f_area.w);
    return (m_renderStage != RS_None);
}

bool ROC::RenderManager::ClearViewport(bool f_depth, bool f_color)
{
    if(m_renderStage != RS_None)
    {
        if(f_depth) GLSetting::SetDepthMask(true);
        GLViewport::Clear(f_depth, f_color);
    }
    return (m_renderStage != RS_None);
}

bool ROC::RenderManager::SetClearColor(const glm::vec4 &f_color)
{
    if(m_renderStage != RS_None) GLViewport::SetClearColor(f_color.r, f_color.g, f_color.b, f_color.a);
    return (m_renderStage != RS_None);
}

bool ROC::RenderManager::SetPolygonMode(int f_mode)
{
    if(m_renderStage != RS_None) GLSetting::SetFillMode(GL_POINT + f_mode);
    return (m_renderStage != RS_None);
}

void ROC::RenderManager::DoPulse()
{
    m_time = m_core->GetSfmlManager()->GetTime();

    m_renderStage = RS_Main;
    m_clearFrame = true;
    m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnRender, m_arguments);
    m_clearFrame = false;

    if(m_vrManager->IsVREnabled())
    {
        m_renderStage = RS_VR;
        m_vrManager->Render();
    }

    m_core->GetSfmlManager()->SwapBuffers();
    m_renderStage = RS_None;
}

// ROC::IRenderManager
bool ROC::RenderManager::SetActiveScene(IScene *f_scene)
{
    return SetActiveScene(dynamic_cast<Scene*>(f_scene));
}

bool ROC::RenderManager::DrawScene(IScene *f_scene)
{
    return DrawScene(dynamic_cast<Scene*>(f_scene));
}

bool ROC::RenderManager::Render(IFont *f_font, const glm::vec2 &f_pos, const std::string &f_text, const glm::vec4 &f_color, const std::string &f_layer)
{
    return Render(dynamic_cast<Font*>(f_font), f_pos, f_text, f_color, f_layer);
}

bool ROC::RenderManager::Render(IDrawable *f_drawable, const glm::vec2 &f_pos, const glm::vec2 &f_size, float f_rot, const glm::vec4 &f_color, const std::string &f_layer)
{
    return Render(dynamic_cast<Drawable*>(f_drawable), f_pos, f_size, f_rot, f_color, f_layer);
}

bool ROC::RenderManager::Render(IDrawable *f_drawable, const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec2 &f_size, const std::string &f_layer, const glm::bvec4 &f_params)
{
    return Render(dynamic_cast<Drawable*>(f_drawable), f_pos, f_rot, f_size, f_layer, f_params);
}
