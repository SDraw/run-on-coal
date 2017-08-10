#include "stdafx.h"

#include "Managers/RenderManager/RenderManager.h"
#include "Core/Core.h"
#include "Managers/RenderManager/Quad.h"
#include "Managers/RenderManager/Quad3D.h"
#include "Elements/Font.h"
#include "Elements/Geometry/Geometry.h"
#include "Elements/Geometry/Material.h"
#include "Elements/Model/Model.h"
#include "Elements/Model/Skeleton.h"
#include "Elements/Movie.h"
#include "Elements/RenderTarget.h"
#include "Elements/Scene.h"
#include "Elements/Shader.h"
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

extern const glm::vec3 g_DefaultPosition;
extern const glm::mat4 g_IdentityMatrix;
extern const glm::vec4 g_EmptyVec4;
extern const glm::mat4 g_EmptyMat4;
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

    Font::CreateVAO();
    Font::CreateLibrary();
    Shader::CreateBonesUBO();

    m_activeScene = nullptr;
    m_activeShader = nullptr;
    m_activeTarget = nullptr;
    m_quad = new Quad();
    m_quad3D = new Quad3D();

    m_dummyTexture = new Texture();
    m_dummyTexture->LoadDummy();

    m_lastVAO = 0U;
    m_lastTexture = 0U;
    m_depthEnabled = true;
    m_blendEnabled = false;
    m_cullEnabled = true;
    m_skipNoDepthMaterials = false;
    m_time = 0.0;
    m_locked = true;
    m_textureMatrix = glm::mat4(1.f);
    m_materialBind = glm::bvec2(true);
    m_fontBind = glm::bvec2(true);

    m_movieVectorEnd = m_movieVector.end();

    glm::ivec2 l_size;
    m_core->GetSfmlManager()->GetWindowSize(l_size);
    m_screenProjection = glm::ortho(0.f, static_cast<float>(l_size.x), 0.f, static_cast<float>(l_size.y));

    m_argument = new LuaArguments();
    m_callback = nullptr;
}
ROC::RenderManager::~RenderManager()
{
    delete m_quad;
    delete m_quad3D;
    delete m_dummyTexture;
    delete m_argument;
    Font::DestroyVAO();
    Font::DestroyLibrary();
    Shader::DestroyBonesUBO();
    m_movieVector.clear();
}

void ROC::RenderManager::SetRenderTarget(RenderTarget *f_rt)
{
    if(!m_locked && (m_activeTarget != f_rt))
    {
        m_activeTarget = f_rt;
        if(!m_activeTarget)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, NULL);
            m_core->GetSfmlManager()->GetWindowSize(m_renderTargetSize);
            m_skipNoDepthMaterials = false;
        }
        else
        {
            m_activeTarget->Enable();
            m_renderTargetSize = m_activeTarget->GetSize();
            m_skipNoDepthMaterials = m_activeTarget->IsShadowType();
        }
        m_screenProjection = glm::ortho(0.f, static_cast<float>(m_renderTargetSize.x), 0.f, static_cast<float>(m_renderTargetSize.y));
        glViewport(0, 0, m_renderTargetSize.x, m_renderTargetSize.y);
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

void ROC::RenderManager::SetActiveScene(Scene *f_scene)
{
    if(!m_locked)
    {
        m_activeScene = f_scene;
        if(m_activeScene)
        {
            if(m_activeShader)
            {
                Camera *l_camera = m_activeScene->GetCamera();
                if(l_camera) l_camera->Update();
                m_activeShader->SetProjectionUniformValue(l_camera ? l_camera->GetProjectionMatrix() : g_EmptyMat4);
                m_activeShader->SetViewUniformValue(l_camera ? l_camera->GetViewMatrix() : g_EmptyMat4);
                m_activeShader->SetViewProjectionUniformValue(l_camera ? l_camera->GetViewProjectionMatrix() : g_EmptyMat4);
                m_activeShader->SetCameraPositionUniformValue(l_camera ? l_camera->GetPosition() : g_DefaultPosition);
                m_activeShader->SetCameraDirectionUniformValue(l_camera ? l_camera->GetDirection() : g_DefaultPosition);

                Light *l_light = m_activeScene->GetLight();
                m_activeShader->SetLightColorUniformValue(l_light ? l_light->GetColor() : g_EmptyVec4);
                m_activeShader->SetLightDirectionUniformValue(l_light ? l_light->GetDirection() : g_DefaultPosition);
                m_activeShader->SetLightParamUniformValue(l_light ? l_light->GetParams() : g_EmptyVec4);
            }
        }
    }
}
void ROC::RenderManager::RemoveAsActiveScene(Scene *f_scene)
{
    if(m_activeScene == f_scene) m_activeScene = nullptr;
}

void ROC::RenderManager::SetActiveShader(Shader *f_shader)
{
    if(!m_locked)
    {
        m_activeShader = f_shader;
        if(m_activeShader)
        {
            m_activeShader->Enable();
            m_activeShader->SetTimeUniformValue(m_time);
        }
    }
}
void ROC::RenderManager::RemoveAsActiveShader(Shader *f_shader)
{
    if(m_activeShader == f_shader) m_activeShader = nullptr;
}

void ROC::RenderManager::Render(Model *f_model, bool f_frustum, bool f_texturize)
{
    if(!m_locked && m_activeShader && m_activeScene && f_model->HasGeometry())
    {
        bool l_result = true;
        if(f_frustum)
        {
            Camera *l_camera = m_activeScene->GetCamera();
            if(l_camera)
            {
                if(f_model->GetParent() != nullptr)
                {
                    btTransform l_transform;
                    l_transform.setFromOpenGLMatrix(glm::value_ptr(f_model->GetMatrix()));
                    std::memcpy(&m_modelPosition, l_transform.getOrigin().m_floats, sizeof(glm::vec3));
                }
                else m_modelPosition = f_model->GetPosition();
                if(!l_camera->IsInFrustum(m_modelPosition, f_model->GetBoundSphereRadius())) l_result = false;
            }
        }
        if(l_result)
        {
            m_activeShader->SetModelUniformValue(f_model->GetMatrix());

            if(f_model->HasSkeleton())
            {
                Shader::SetBonesUniformValue(f_model->GetSkeleton()->GetBoneMatricesVector());
                m_activeShader->SetAnimatedUniformValue(1U);
            }
            else m_activeShader->SetAnimatedUniformValue(0U);

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

                m_materialBind.x = CompareLastTexture(l_texture->GetTextureID()) && f_texturize;
                if(m_materialBind.x) l_texture->Bind();
                m_materialBind.y = CompareLastVAO(iter->GetVAO());
                if(m_materialBind.y)
                {
                    m_activeShader->SetMaterialTypeUniformValue(static_cast<int>(iter->GetType()));
                    m_activeShader->SetMaterialParamUniformValue(iter->GetParams());
                }
                iter->Draw(m_materialBind.y);
            }
        }
    }
}
void ROC::RenderManager::Render(Font *f_font, const glm::vec2 &f_pos, const sf::String &f_text, const glm::vec4 &f_color)
{
    if(!m_locked && m_activeShader)
    {
        EnableBlending();
        DisableDepth();

        m_activeShader->SetModelUniformValue(g_IdentityMatrix);
        m_activeShader->SetProjectionUniformValue(m_screenProjection);
        m_activeShader->SetColorUniformValue(f_color);

        m_fontBind.x = CompareLastVAO(Font::GetVAO());
        m_fontBind.y = CompareLastTexture(f_font->GetAtlasTexture());
        f_font->Draw(f_text, f_pos, m_fontBind);
    }
}
void ROC::RenderManager::Render(Drawable *f_drawable, const glm::vec2 &f_pos, const glm::vec2 &f_size, float f_rot, const glm::vec4 &f_color)
{
    if(!m_locked && m_activeShader)
    {
        if(CompareLastVAO(m_quad->GetVAO())) m_quad->Bind();
        if(CompareLastTexture(f_drawable->GetTextureID())) f_drawable->Bind();

        m_quad->SetTransformation(f_size);

        m_activeShader->SetProjectionUniformValue(m_screenProjection);
        m_activeShader->SetColorUniformValue(f_color);

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
        m_activeShader->SetModelUniformValue(m_textureMatrix);

        DisableCulling();
        DisableDepth();
        f_drawable->IsTransparent() ? EnableBlending() : DisableBlending();
        Quad::Draw();
    }
}
void ROC::RenderManager::Render(Drawable *f_drawable, const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec2 &f_size, const glm::bvec4 &f_params)
{
    if(!m_locked && m_activeShader && m_activeScene)
    {
        Camera *l_camera = m_activeScene->GetCamera();
        if(l_camera)
        {
            glm::vec2 l_halfSize = f_size / 2.f;
            float l_radius = glm::length(l_halfSize);
            if(l_camera->IsInFrustum(f_pos, l_radius))
            {
                if(CompareLastVAO(m_quad3D->GetVAO())) m_quad3D->Bind();
                if(CompareLastTexture(f_drawable->GetTextureID())) f_drawable->Bind();

                m_quad3D->SetTransformation(f_pos, f_rot, f_size);

                m_activeShader->SetAnimatedUniformValue(0U);
                m_activeShader->SetModelUniformValue(m_quad3D->GetMatrixRef());
                m_activeShader->SetMaterialParamUniformValue(g_EmptyVec4);

                int l_materialType = 0;
                if(f_params.x) l_materialType |= ROC_MATERIAL_BIT_SHADING;
                if(f_params.y) l_materialType |= ROC_MATERIAL_BIT_DEPTH;
                if(f_params.z) l_materialType |= ROC_MATERIAL_BIT_TRANSPARENCY;
                if(f_params.w) l_materialType |= ROC_MATERIAL_BIT_DOUBLESIDE;
                m_activeShader->SetMaterialTypeUniformValue(l_materialType);

                f_params.w ? DisableCulling() : EnableCulling();
                f_params.y ? EnableDepth() : DisableDepth();
                (f_drawable->IsTransparent() && f_params.z) ? EnableBlending() : DisableBlending();
                Quad3D::Draw();
            }
        }
    }
}

bool ROC::RenderManager::AttachToShader(Shader *f_shader, Drawable *f_element, const std::string &f_uniform)
{
    EnableNonActiveShader(f_shader);
    bool l_result = f_shader->Attach(f_element, f_uniform);
    RestoreActiveShader(f_shader);
    return l_result;
}
void ROC::RenderManager::DetachFromShader(Shader *f_shader, Drawable *f_element)
{
    EnableNonActiveShader(f_shader);
    f_shader->Detach(f_element);
    RestoreActiveShader(f_shader);
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

void ROC::RenderManager::EnableNonActiveShader(Shader *f_shader) const
{
    if(m_activeShader != f_shader) f_shader->Enable(false);
}
void ROC::RenderManager::RestoreActiveShader(Shader *f_shader)
{
    if(m_activeShader != f_shader && m_activeShader) m_activeShader->Enable(false);
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
