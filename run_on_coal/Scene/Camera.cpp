#include "stdafx.h"
#include "Scene/Camera.h"

ROC::Camera::Camera()
{
    m_viewPosition = glm::vec3(0.f);
    m_viewDirection = glm::vec3(0.f,0.f,1.f);
    m_viewMatrix = glm::lookAt(m_viewPosition,m_viewPosition+m_viewDirection,glm::vec3(0.0f,1.0f,0.0f));
    m_projectionMatrix = glm::perspective(glm::pi<float>()/4.0f,640.f/480.f,0.2f,100.f);
    m_rebuildViewMatrix = false;
}
ROC::Camera::~Camera()
{
}

void ROC::Camera::UpdateViewMatrix()
{
    m_viewMatrix = glm::lookAt(m_viewPosition,m_viewPosition+m_viewDirection,glm::vec3(0.0f,1.0f,0.0f));
}

void ROC::Camera::SetPerspective(float f_fov, float f_width, float f_height, float f_near, float f_far)
{
    m_projectionMatrix = glm::perspectiveFov(f_fov,f_width,f_height,f_near,f_far);
}
void ROC::Camera::SetOrtho(float l_left,float l_right,float f_bottom,float f_top,float l_near,float l_far)
{
    m_projectionMatrix = glm::ortho(l_left,l_right,f_bottom,f_top,l_near,l_far);
}

void ROC::Camera::SetPosition(glm::vec3& f_pos)
{
    if(!std::memcmp(&m_viewPosition,&f_pos,sizeof(glm::vec3))) return;
    std::memcpy(&m_viewPosition,&f_pos,sizeof(glm::vec3));
    m_rebuildViewMatrix = true;
}
void ROC::Camera::GetPosition(glm::vec3& f_pos)
{
    std::memcpy(&f_pos,&m_viewPosition,sizeof(glm::vec3));
}

void ROC::Camera::SetDirection(glm::vec3& f_pos)
{
    if(!std::memcmp(&m_viewDirection,&f_pos,sizeof(glm::vec3))) return;
    std::memcpy(&m_viewDirection,&f_pos,sizeof(glm::vec3));
    m_rebuildViewMatrix = true;
}
void ROC::Camera::GetDirection(glm::vec3& f_pos)
{
    std::memcpy(&f_pos,&m_viewDirection,sizeof(glm::vec3));
}

void ROC::Camera::GetViewMatrix(glm::mat4 &f_mat)
{
    if(m_rebuildViewMatrix)
    {
        UpdateViewMatrix();
        m_rebuildViewMatrix = false;
    }
    std::memcpy(&f_mat,&m_viewMatrix,sizeof(glm::mat4));
}
void ROC::Camera::GetProjectionMatrix(glm::mat4 &f_mat)
{
    std::memcpy(&f_mat,&m_projectionMatrix,sizeof(glm::mat4));
}
