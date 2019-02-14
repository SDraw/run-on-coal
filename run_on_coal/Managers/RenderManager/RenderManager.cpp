#include "stdafx.h"

#include "Managers/RenderManager/RenderManager.h"
#include "Core/Core.h"
#include "Managers/LogManager.h"
#include "Managers/RenderManager/Quad2D.h"
#include "Managers/RenderManager/Quad3D.h"
#include "Managers/RenderManager/PhysicsDrawer.h"
#include "Elements/Font.h"
#include "Elements/Geometry/Geometry.h"
#include "Elements/Geometry/Material.h"
#include "Elements/Model/Model.h"
#include "Elements/Model/Skeleton.h"
#include "Elements/RenderTarget.h"
#include "Elements/Scene.h"
#include "Elements/Shader/Shader.h"
#include "Elements/Texture.h"
#include "Lua/LuaArguments.h"
#include "Utils/Pool.h"

#include "Managers/ConfigManager.h"
#include "Managers/EventManager.h"
#include "Managers/LuaManager.h"
#include "Managers/PhysicsManager.h"
#include "Managers/SfmlManager.h"
#include "Managers/VRManager.h"
#include "Elements/Camera.h"
#include "Elements/Light.h"
#include "Utils/GLBinder.h"

namespace ROC
{

extern const glm::mat4 g_IdentityMatrix;
extern const glm::vec4 g_EmptyVec4;

const btVector3 g_TextureZAxis(0.f, 0.f, 1.f);
const glm::vec4 g_DefaultClearColor(0.223529f, 0.223529f, 0.223529f, 0.f);
const std::vector<std::string> g_VRRenderSide
{
    "left", "right"
};

}

#define ROC_VRRENDER_SIDE_LEFT 0U
#define ROC_VRRENDER_SIDE_RIGHT 1U

ROC::RenderManager::RenderManager(Core *f_core)
{
    m_core = f_core;
    m_vrManager = m_core->GetVRManager();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    glEnable(GL_CULL_FACE); // default culling

    glClearColor(g_DefaultClearColor.r, g_DefaultClearColor.g, g_DefaultClearColor.b, g_DefaultClearColor.a);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Font::CreateLibrary();
    Font::CreateVAO();
    Shader::UpdateDrawableMaxCount();

    m_activeScene = nullptr;
    m_quad2D = new Quad2D();
    m_quad3D = new Quad3D();
    m_physicsDrawer = new PhysicsDrawer();
    m_core->GetPhysicsManager()->SetDebugDrawer(m_physicsDrawer);

    m_dummyTexture = new Texture();
    m_dummyTexture->LoadDummy();

    m_depthEnabled = true;
    m_blendEnabled = false;
    m_cullEnabled = true;
    m_skipNoDepthMaterials = false;
    m_time = 0.f;
    m_locked = true;
    m_textureMatrix = g_IdentityMatrix;

    m_clearColor = g_DefaultClearColor;
    m_core->GetSfmlManager()->GetWindowSize(m_viewportSize);

    m_luaArguments = new LuaArguments();
    m_callback = nullptr;
    m_vrCallback = nullptr;
}
ROC::RenderManager::~RenderManager()
{
    delete m_quad2D;
    delete m_quad3D;
    delete m_dummyTexture;
    delete m_physicsDrawer;
    delete m_luaArguments;
    Font::DestroyVAO();
    Font::DestroyLibrary();
}

void ROC::RenderManager::SetActiveScene(Scene *f_scene)
{
    if(!m_locked)
    {
        if(m_activeScene) m_activeScene->Disable();
        m_activeScene = f_scene;

        if(m_activeScene)
        {
            m_activeScene->Enable();
            if(m_activeScene->HasRenderTarget()) m_skipNoDepthMaterials = m_activeScene->GetRenderTarget()->IsShadowType();
            else
            {
                m_skipNoDepthMaterials = false;

                if(m_vrManager->IsVREnabled() && m_vrLock) m_vrManager->EnableRenderTarget();
                else GLBinder::SetViewport(0, 0, m_viewportSize.x, m_viewportSize.y);
            }

            if(m_activeScene->HasShader())
            {
                Shader *l_shader = m_activeScene->GetShader();
                l_shader->SetTime(m_core->GetSfmlManager()->GetTime());
            }
        }
    }
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
}

void ROC::RenderManager::DrawScene(Scene *f_scene)
{
    if(!m_locked && (m_activeScene == f_scene))
    {
        if(m_activeScene->IsValidForRender())
        {
            Shader *l_shader = m_activeScene->GetShader();
            RenderTarget *l_renderTarget = m_activeScene->GetRenderTarget();
            bool l_skipTextures = (l_renderTarget ? l_renderTarget->IsShadowType() : false);

            auto l_distantModelVector = m_activeScene->GetDistantModels();
            for(const auto l_distantModel : l_distantModelVector)
            {
                if(l_distantModel->m_visible)
                {
                    Model *l_model = l_distantModel->m_model;
                    if(l_model->HasGeometry())
                    {
                        l_shader->SetModelMatrix(l_model->GetFullMatrix());

                        if(l_model->HasSkeleton())
                        {
                            l_shader->SetBoneMatrices(l_model->GetSkeleton()->GetPoseMatrices());
                            l_shader->SetAnimated(true);
                        }
                        else l_shader->SetAnimated(false);

                        for(const auto l_material : l_model->GetGeometry()->GetMaterialVector())
                        {
                            if(l_material->HasDepth()) EnableDepth();
                            else
                            {
                                if(m_skipNoDepthMaterials) continue;
                                else DisableDepth();
                            }
                            l_material->IsTransparent() ? EnableBlending() : DisableBlending();
                            l_material->IsDoubleSided() ? DisableCulling() : EnableCulling();

                            if(!l_skipTextures)
                            {
                                Texture *l_texture = l_material->HasTexture() ? l_material->GetTexture() : m_dummyTexture;
                                l_texture->Bind();
                            }
                            l_shader->SetMaterialType(static_cast<int>(l_material->GetType()));
                            l_shader->SetMaterialParam(l_material->GetParams());
                            l_material->Draw();
                        }
                    }
                }
            }
        }
    }
}
void ROC::RenderManager::Render(Font *f_font, const glm::vec2 &f_pos, const sf::String &f_text, const glm::vec4 &f_color)
{
    if(!m_locked && m_activeScene)
    {
        if(m_activeScene->IsValidForRender())
        {
            EnableBlending();
            DisableDepth();

            Shader *l_shader = m_activeScene->GetShader();
            l_shader->SetModelMatrix(g_IdentityMatrix);
            l_shader->SetColor(f_color);
            f_font->Draw(f_text, f_pos);
        }
    }
}
void ROC::RenderManager::Render(Drawable *f_drawable, const glm::vec2 &f_pos, const glm::vec2 &f_size, float f_rot, const glm::vec4 &f_color)
{
    if(!m_locked && m_activeScene)
    {
        if(m_activeScene->IsValidForRender())
        {
            f_drawable->Bind();
            m_quad2D->SetTransformation(f_size);

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
            l_textureTransform.getOpenGLMatrix(glm::value_ptr(m_textureMatrix));

            Shader *l_shader = m_activeScene->GetShader();
            l_shader->SetModelMatrix(m_textureMatrix);
            l_shader->SetColor(f_color);

            DisableCulling();
            DisableDepth();
            f_drawable->IsTransparent() ? EnableBlending() : DisableBlending();
            m_quad2D->Draw();
        }
    }
}
void ROC::RenderManager::Render(Drawable *f_drawable, const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec2 &f_size, const glm::bvec4 &f_params)
{
    if(!m_locked && m_activeScene)
    {
        if(m_activeScene->IsValidForRender())
        {
            glm::vec2 l_halfSize = f_size / 2.f;
            float l_radius = glm::length(l_halfSize);
            if(m_activeScene->GetCamera()->IsInFrustum(f_pos, l_radius))
            {
                f_drawable->Bind();
                m_quad3D->SetTransformation(f_pos, f_rot, f_size);

                Shader *l_shader = m_activeScene->GetShader();
                l_shader->SetAnimated(false);
                l_shader->SetModelMatrix(m_quad3D->GetMatrix());
                l_shader->SetMaterialParam(g_EmptyVec4);

                int l_materialType = 0;
                if(f_params.x) l_materialType |= Material::MPB_Shading;
                if(f_params.y) l_materialType |= Material::MPB_Depth;
                if(f_params.z) l_materialType |= Material::MPB_Transparency;
                if(f_params.w) l_materialType |= Material::MPB_Doubleside;
                l_shader->SetMaterialType(l_materialType);

                f_params.w ? DisableCulling() : EnableCulling();
                f_params.y ? EnableDepth() : DisableDepth();
                (f_drawable->IsTransparent() && f_params.z) ? EnableBlending() : DisableBlending();
                m_quad3D->Draw();
            }
        }
    }
}
void ROC::RenderManager::DrawPhysics(float f_width)
{
    if(!m_locked && m_activeScene)
    {
        if(m_activeScene->IsValidForRender())
        {
            Shader *l_shader = m_activeScene->GetShader();
            l_shader->SetModelMatrix(g_IdentityMatrix);
            l_shader->SetAnimated(false);
            l_shader->SetMaterialType((Material::MPB_Depth | Material::MPB_Doubleside));
            l_shader->SetMaterialParam(g_EmptyVec4);

            DisableCulling();
            EnableDepth();
            DisableBlending();

            m_dummyTexture->Bind();

            m_core->GetPhysicsManager()->DrawDebugWorld();
            m_physicsDrawer->Draw(f_width);
        }
    }
}

void ROC::RenderManager::ClearRenderArea(bool f_depth, bool f_color)
{
    if(!m_locked)
    {
        int l_params = 0;
        if(f_depth)
        {
            l_params |= GL_DEPTH_BUFFER_BIT;
            EnableDepth();
        }
        if(f_color) l_params |= GL_COLOR_BUFFER_BIT;
        glClear(l_params);
    }
}
void ROC::RenderManager::SetClearColour(const glm::vec4 &f_color)
{
    if(!m_locked)
    {
        if(m_clearColor != f_color)
        {
            std::memcpy(&m_clearColor, &f_color, sizeof(glm::vec4));
            glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
        }
    }
}
void ROC::RenderManager::SetViewport(const glm::ivec4 &f_area)
{
    if(!m_locked) GLBinder::SetViewport(f_area.x, f_area.y, f_area.z, f_area.w);
}
void ROC::RenderManager::SetPolygonMode(int f_mode)
{
    if(!m_locked) glPolygonMode(GL_FRONT_AND_BACK, GL_POINT + f_mode);
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
        m_blendEnabled = true;
    }
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

void ROC::RenderManager::DoPulse()
{
    m_locked = false;
    m_time = m_core->GetSfmlManager()->GetTime();

    if(m_callback) (*m_callback)();
    EventManager *l_eventManager = m_core->GetLuaManager()->GetEventManager();
    l_eventManager->CallEvent(EventManager::EME_onRender, m_luaArguments);

    if(m_vrManager->IsVREnabled())
    {
        m_vrLock = true;

        const glm::uvec2 &l_rtSize = m_vrManager->GetTargetsSize();

        m_vrManager->SetVRStage(VRManager::VRS_Left);
        m_vrManager->EnableRenderTarget();
        if(m_vrCallback) (*m_vrCallback)(g_VRRenderSide[ROC_VRRENDER_SIDE_LEFT]);
        m_luaArguments->PushArgument(g_VRRenderSide[ROC_VRRENDER_SIDE_LEFT]);
        l_eventManager->CallEvent(EventManager::EME_onVRRender, m_luaArguments);
        m_luaArguments->Clear();

        m_vrManager->SetVRStage(VRManager::VRS_Right);
        m_vrManager->EnableRenderTarget();
        if(m_vrCallback) (*m_vrCallback)(g_VRRenderSide[ROC_VRRENDER_SIDE_RIGHT]);
        m_luaArguments->PushArgument(g_VRRenderSide[ROC_VRRENDER_SIDE_RIGHT]);
        l_eventManager->CallEvent(EventManager::EME_onVRRender, m_luaArguments);
        m_luaArguments->Clear();

        m_vrManager->SubmitRender();
        m_vrManager->SetVRStage(VRManager::VRS_None);
        m_vrLock = false;
    }

    m_core->GetSfmlManager()->SwapBuffers();
    m_locked = true;
}
