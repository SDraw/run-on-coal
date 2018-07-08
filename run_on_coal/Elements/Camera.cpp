#include "stdafx.h"

#include "Elements/Camera.h"

#include "Utils/MathUtils.h"

vr::IVRSystem *ROC::Camera::ms_vrSystem = nullptr;

namespace ROC
{

extern const glm::vec3 g_DefaultPosition;
const glm::vec3 g_CameraDefaultViewDirection(0.f, 0.f, -1.f);
const glm::vec3 g_CameraDefaultUpDirection(0.f, 1.f, 0.f);

}

ROC::Camera::Camera(int f_type)
{
    m_elementType = ET_Camera;
    m_elementTypeName.assign("Camera");

    m_type = f_type;
    btClamp(m_type, static_cast<int>(CPT_Perspective), static_cast<int>(CPT_VRRight));

    m_viewPosition = g_DefaultPosition;
    m_viewDirection = g_CameraDefaultViewDirection;
    m_upDirection = g_CameraDefaultUpDirection;
    m_viewMatrix = glm::lookAtRH(m_viewPosition, m_viewPosition + m_viewDirection, g_CameraDefaultUpDirection);

    m_fov = glm::pi<float>() / 4.0f;
    m_aspectRatio = 640.f / 480.f;
    m_orthoParams = glm::vec4(-1.f, 1.f, -1.f, 1.f);
    m_depth = glm::vec2(1.f, 10.f);
    m_projectionMatrix = glm::perspective(m_fov, m_aspectRatio, m_depth.x, m_depth.y);
    m_viewProjectionMatrix = m_viewMatrix*m_projectionMatrix;

    m_rebuildView = false;
    m_rebuildProjection = false;
}
ROC::Camera::~Camera()
{
}

void ROC::Camera::SetProjectionType(int f_type)
{
    if(m_type != f_type)
    {
        m_type = f_type;
        btClamp(m_type, static_cast<int>(CPT_Perspective), static_cast<int>(CPT_VRRight));
        m_rebuildProjection = true;
    }
}

void ROC::Camera::SetPosition(const glm::vec3& f_pos)
{
    if(m_viewPosition != f_pos)
    {
        std::memcpy(&m_viewPosition, &f_pos, sizeof(glm::vec3));
        m_rebuildView = true;
    }
}
void ROC::Camera::SetDirection(const glm::vec3& f_dir)
{
    if(m_viewDirection != f_dir)
    {
        std::memcpy(&m_viewDirection, &f_dir, sizeof(glm::vec3));
        m_rebuildView = true;
    }
}
void ROC::Camera::SetDirection(const glm::quat &f_dir)
{
    glm::vec3 l_dir = f_dir*g_CameraDefaultViewDirection;
    if(m_viewDirection != l_dir)
    {
        std::memcpy(&m_viewDirection, &l_dir, sizeof(glm::vec3));
        m_rebuildView = true;
    }

    glm::vec3 l_upDir = f_dir*g_CameraDefaultUpDirection;
    if(m_upDirection != l_upDir)
    {
        std::memcpy(&m_upDirection, &l_upDir, sizeof(glm::vec3));
        m_rebuildView = true;
    }
}
void ROC::Camera::SetUpDirection(const glm::vec3 &f_dir)
{
    if(m_upDirection != f_dir)
    {
        std::memcpy(&m_upDirection, &f_dir, sizeof(glm::vec3));
        m_rebuildView = true;
    }
}

void ROC::Camera::SetFOV(float f_fov)
{
    if(m_fov != f_fov)
    {
        m_fov = f_fov;
        m_rebuildProjection = true;
    }
}

void ROC::Camera::SetAspectRatio(float f_ratio)
{
    if(m_aspectRatio != f_ratio)
    {
        m_aspectRatio = f_ratio;
        m_rebuildProjection = true;
    }
}
void ROC::Camera::SetOrthoParams(const glm::vec4 &f_size)
{
    if(m_orthoParams != f_size)
    {
        std::memcpy(&m_orthoParams, &f_size, sizeof(glm::vec4));
        m_rebuildProjection = true;
    }
}
void ROC::Camera::SetDepth(const glm::vec2 &f_depth)
{
    if(m_depth != f_depth)
    {
        std::memcpy(&m_depth, &f_depth, sizeof(glm::vec2));
        m_rebuildProjection = true;
    }
}

void ROC::Camera::Update()
{
    if(m_rebuildView)
    {
        glm::vec3 l_viewPoint = m_viewPosition + m_viewDirection;
        m_viewMatrix = glm::lookAtRH(m_viewPosition, l_viewPoint, m_upDirection);
    }
    if(m_rebuildProjection)
    {
        switch(m_type)
        {
            case CPT_Perspective:
                m_projectionMatrix = glm::perspective(m_fov, m_aspectRatio, m_depth.x, m_depth.y);
                break;
            case CPT_Orthogonal:
                m_projectionMatrix = glm::ortho(m_orthoParams.x, m_orthoParams.y, m_orthoParams.z, m_orthoParams.w, m_depth.x, m_depth.y);
                break;
            case CPT_Screen:
                m_projectionMatrix = glm::ortho(m_orthoParams.x, m_orthoParams.y, m_orthoParams.z, m_orthoParams.w);
                break;
            case CPT_VRLeft:
            {
                vr::HmdMatrix44_t l_projection = ms_vrSystem->GetProjectionMatrix(vr::Eye_Left, m_depth.x, m_depth.y);
                MathUtils::ExtractMatrix(l_projection, m_projectionMatrix);
            } break;
            case CPT_VRRight:
            {
                vr::HmdMatrix44_t l_projection = ms_vrSystem->GetProjectionMatrix(vr::Eye_Right, m_depth.x, m_depth.y);
                MathUtils::ExtractMatrix(l_projection, m_projectionMatrix);
            } break;
        }
    }
    if(m_rebuildView || m_rebuildProjection)
    {
        m_viewProjectionMatrix = m_projectionMatrix*m_viewMatrix;
        m_planes[0] = glm::row(m_viewProjectionMatrix, 3) + glm::row(m_viewProjectionMatrix, 0);
        m_planes[1] = glm::row(m_viewProjectionMatrix, 3) - glm::row(m_viewProjectionMatrix, 0);
        m_planes[2] = glm::row(m_viewProjectionMatrix, 3) + glm::row(m_viewProjectionMatrix, 1);
        m_planes[3] = glm::row(m_viewProjectionMatrix, 3) - glm::row(m_viewProjectionMatrix, 1);
        m_planes[4] = glm::row(m_viewProjectionMatrix, 3) + glm::row(m_viewProjectionMatrix, 2);
        m_planes[5] = glm::row(m_viewProjectionMatrix, 3) - glm::row(m_viewProjectionMatrix, 2);
        for(auto &iter : m_planes) iter /= sqrtf(iter.x*iter.x + iter.y*iter.y + iter.z*iter.z);

        m_rebuildView = false;
        m_rebuildProjection = false;
    }
}

bool ROC::Camera::IsInFrustum(const glm::vec3 &f_pos, float f_radius)
{
    bool l_result = true;
    for(auto &iter : m_planes)
    {
        if(iter.x*f_pos.x + iter.y*f_pos.y + iter.z*f_pos.z + iter.w < -f_radius)
        {
            l_result = false;
            break;
        }
    }
    return l_result;
}
bool ROC::Camera::IsInFrustum(const glm::mat4 &f_mat, float f_radius)
{
    bool l_result = true;
    btTransform l_transform = btTransform::getIdentity();
    l_transform.setFromOpenGLMatrix(glm::value_ptr(f_mat));
    const btVector3 &l_position = l_transform.getOrigin();
    for(auto &iter : m_planes)
    {
        if(iter.x*l_position.x() + iter.y*l_position.y() + iter.z*l_position.z() + iter.w < -f_radius)
        {
            l_result = false;
            break;
        }
    }
    return l_result;
}

void ROC::Camera::SetVRSystem(vr::IVRSystem *f_system)
{
    ms_vrSystem = f_system;
}
