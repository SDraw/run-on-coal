#include "stdafx.h"
#include "Scene/Light.h"

ROC::Light::Light()
{
    m_direction = glm::vec3(0.f,-1.f,0.f);
    m_color = glm::vec3(1.f,1.f,1.f);
    m_params = glm::vec4(0.f,1.f,0.f,0.f);
}

ROC::Light::~Light()
{
}

void ROC::Light::SetParams(glm::vec4 &f_val)
{
    std::memcpy(&m_params,&f_val,sizeof(glm::vec4));
}
void ROC::Light::GetParams(glm::vec4 &f_val)
{
    std::memcpy(&f_val,&m_params,sizeof(glm::vec4));
}

void ROC::Light::SetColor(glm::vec3 &f_vec)
{
    std::memcpy(&m_color,&f_vec,sizeof(glm::vec3));
}
void ROC::Light::GetColor(glm::vec3 &f_vec)
{
    std::memcpy(&f_vec,&m_color,sizeof(glm::vec3));
}

void ROC::Light::SetDirection(glm::vec3 &f_vec)
{
    std::memcpy(&m_direction,&f_vec,sizeof(glm::vec3));
}
void ROC::Light::GetDirection(glm::vec3 &f_vec)
{
    std::memcpy(&f_vec,&m_direction,sizeof(glm::vec3));
}
