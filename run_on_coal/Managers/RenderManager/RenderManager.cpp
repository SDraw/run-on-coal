#include "stdafx.h"

#include "Managers/RenderManager/RenderManager.h"
#include "Core/Core.h"
#include "Managers/RenderManager/Quad2D.h"
#include "Managers/RenderManager/Quad3D.h"
#include "Elements/Font.h"
#include "Elements/Geometry/Geometry.h"
#include "Elements/Geometry/Material.h"
#include "Elements/Model/Model.h"
#include "Elements/Model/Skeleton.h"
#include "Elements/Movie.h"
#include "Elements/RenderTarget.h"
#include "Elements/Scene.h"
#include "Elements/Shader/Shader.h"
#include "Elements/Texture.h"
#include "Lua/LuaArguments.h"
#include "Utils/Pool.h"

#include "Managers/EventManager.h"
#include "Managers/LuaManager.h"
#include "Managers/SfmlManager.h"
#include "Elements/Camera.h"
#include "Elements/Light.h"

namespace ROC
{

extern const glm::mat4 g_IdentityMatrix;
extern const glm::vec4 g_EmptyVec4;
const btVector3 g_TextureZAxis(0.f, 0.f, 1.f);

}

ROC::RenderManager::RenderManager(Core *f_core)
{
    m_core = f_core;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    glEnable(GL_CULL_FACE); // default culling

    glClearColor(0.223529f, 0.223529f, 0.223529f, 0.f);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Font::CreateLibrary();
    Font::CreateVAO();
    Shader::CreateBonesUBO();

    m_activeScene = nullptr;
    m_quad2D = new Quad2D();
    m_quad3D = new Quad3D();

    m_dummyTexture = new Texture();
    m_dummyTexture->LoadDummy();

    m_lastVAO = 0U;
    m_lastTexture = 0U;
    m_depthEnabled = true;
    m_blendEnabled = false;
    m_cullEnabled = true;
    m_skipNoDepthMaterials = false;
    m_time = 0.f;
    m_locked = true;
    m_textureMatrix = g_IdentityMatrix;
    m_materialBind = glm::bvec2(true);
    m_fontBind = glm::bvec2(true);

    m_movieVectorEnd = m_movieVector.end();

    glm::ivec2 l_size;
    m_core->GetSfmlManager()->GetWindowSize(l_size);

    m_argument = new LuaArguments();
    m_callback = nullptr;
}
ROC::RenderManager::~RenderManager()
{
    m_movieVector.clear();
    delete m_quad2D;
    delete m_quad3D;
    delete m_dummyTexture;
    delete m_argument;
    Font::DestroyVAO();
    Font::DestroyLibrary();
    Shader::DestroyBonesUBO();
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
            if(m_activeScene->HasRenderTarget())
            {
                RenderTarget *l_rt = m_activeScene->GetRenderTarget();
                const glm::ivec2 &l_renderSize = l_rt->GetSize();
                m_skipNoDepthMaterials = l_rt->IsShadowType();
                glViewport(0, 0, l_renderSize.x, l_renderSize.y);
            }
            else
            {
                m_skipNoDepthMaterials = false;
                m_core->GetSfmlManager()->GetWindowSize(m_windowSize);
                glViewport(0, 0, m_windowSize.x, m_windowSize.y);
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

void ROC::RenderManager::AddMovie(Movie *f_movie)
{
    if(std::find(m_movieVector.begin(), m_movieVectorEnd, f_movie) == m_movieVectorEnd)
    {
        m_movieVector.push_back(f_movie);
        m_movieVectorEnd = m_movieVector.end();
    }
}
void ROC::RenderManager::RemoveMovie(Movie *f_movie)
{
    auto iter = std::find(m_movieVector.begin(), m_movieVectorEnd, f_movie);
    if(iter != m_movieVectorEnd)
    {
        m_movieVector.erase(iter);
        m_movieVectorEnd = m_movieVector.end();
    }
}

void ROC::RenderManager::Render(Model *f_model)
{
    if(!m_locked && m_activeScene && f_model->HasGeometry())
    {
        if(m_activeScene->IsValidForRender())
        {
            if( m_activeScene->GetCamera()->IsInFrustum(f_model->GetGlobalMatrix(), f_model->GetBoundSphereRadius()))
            {
                Shader *l_shader = m_activeScene->GetShader();
                l_shader->SetModelMatrix(f_model->GetGlobalMatrix());

                if(f_model->HasSkeleton())
                {
                    Shader::SetBoneMatrices(f_model->GetSkeleton()->GetPoseMatrices());
                    l_shader->SetAnimated(1U);
                }
                else l_shader->SetAnimated(0U);

                for(auto iter : f_model->GetGeometry()->GetMaterialVector())
                {
                    if(iter->HasDepth()) EnableDepth();
                    else
                    {
                        if(m_skipNoDepthMaterials) continue;
                        else DisableDepth();
                    }
                    iter->IsTransparent() ? EnableBlending() : DisableBlending();
                    iter->IsDoubleSided() ? DisableCulling() : EnableCulling();

                    Texture *l_texture = iter->HasTexture() ? iter->GetTexture() : m_dummyTexture;

                    m_materialBind.x = CompareLastTexture(l_texture->GetTextureID()) && !m_skipNoDepthMaterials;
                    if(m_materialBind.x) l_texture->Bind();
                    m_materialBind.y = CompareLastVAO(iter->GetVAO());
                    if(m_materialBind.y)
                    {
                        l_shader->SetMaterialType(static_cast<int>(iter->GetType()));
                        l_shader->SetMaterialParam(iter->GetParams());
                    }
                    iter->Draw(m_materialBind.y);
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

            m_fontBind.x = CompareLastVAO(Font::GetVAO());
            m_fontBind.y = CompareLastTexture(f_font->GetAtlasTexture());
            f_font->Draw(f_text, f_pos, m_fontBind);
        }
    }
}
void ROC::RenderManager::Render(Drawable *f_drawable, const glm::vec2 &f_pos, const glm::vec2 &f_size, float f_rot, const glm::vec4 &f_color)
{
    if(!m_locked && m_activeScene)
    {
        if(m_activeScene->IsValidForRender())
        {
            if(CompareLastVAO(m_quad2D->GetVAO())) m_quad2D->Bind();
            if(CompareLastTexture(f_drawable->GetTextureID())) f_drawable->Bind();

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
            Quad2D::Draw();
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
                if(CompareLastVAO(m_quad3D->GetVAO())) m_quad3D->Bind();
                if(CompareLastTexture(f_drawable->GetTextureID())) f_drawable->Bind();

                m_quad3D->SetTransformation(f_pos, f_rot, f_size);

                Shader *l_shader = m_activeScene->GetShader();
                l_shader->SetAnimated(0U);
                l_shader->SetModelMatrix(m_quad3D->GetMatrixRef());
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
                Quad3D::Draw();
            }
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
bool ROC::RenderManager::CompareLastVAO(GLuint f_vao)
{
    return ((f_vao == m_lastVAO) ? false : ((m_lastVAO = f_vao) == f_vao));
}
bool ROC::RenderManager::CompareLastTexture(GLuint f_texture)
{
    return ((f_texture == m_lastTexture) ? false : ((m_lastTexture = f_texture) == f_texture));
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

void ROC::RenderManager::DoPulse()
{
    m_time = m_core->GetSfmlManager()->GetTime();

    ResetCallsReducing();
    for(auto iter : m_movieVector) iter->Update();

    m_locked = false;
    if(m_callback) (*m_callback)();

    m_core->GetLuaManager()->GetEventManager()->CallEvent("onRender", m_argument);
    m_locked = true;
    m_core->GetSfmlManager()->SwapBuffers();
}
