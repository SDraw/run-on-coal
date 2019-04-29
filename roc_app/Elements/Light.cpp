#include "stdafx.h"

#include "Elements/Light.h"

namespace ROC
{

extern const glm::vec2 g_EmptyVec2;
extern const glm::vec3 g_EmptyVec3;

}

ROC::Light::Light(unsigned char f_type)
{
    m_elementType = ET_Light;
    m_elementTypeName.assign("Light");

    m_position = g_EmptyVec3;
    m_direction = glm::vec3(0.f, -1.f, 0.f);
    m_color = glm::vec4(1.f);
    m_cutoff = g_EmptyVec2;
    m_falloff = glm::vec3(1.f, 0.f, 0.f);

    m_type = f_type;
    btClamp<unsigned char>(m_type, LT_Directional, LT_Spotlight);
}
ROC::Light::~Light()
{
}

void ROC::Light::SetPosition(const glm::vec3 &f_vec)
{
    std::memcpy(&m_position, &f_vec, sizeof(glm::vec3));
}
const glm::vec3& ROC::Light::GetPosition() const
{
    return m_position;
}

void ROC::Light::SetDirection(const glm::vec3 &f_vec)
{
    std::memcpy(&m_direction, &f_vec, sizeof(glm::vec3));
}
const glm::vec3& ROC::Light::GetDirection() const
{
    return m_direction;
}

void ROC::Light::SetColor(const glm::vec4 &f_vec)
{
    std::memcpy(&m_color, &f_vec, sizeof(glm::vec4));
}
const glm::vec4& ROC::Light::GetColor() const
{
    return m_color;
}

void ROC::Light::SetCutoff(const glm::vec2 &f_vec)
{
    std::memcpy(&m_cutoff, &f_vec, sizeof(glm::vec2));
}
const glm::vec2& ROC::Light::GetCutoff() const
{
    return m_cutoff;
}

void ROC::Light::SetFalloff(const glm::vec3 &f_val)
{
    std::memcpy(&m_falloff, &f_val, sizeof(glm::vec3));
}
const glm::vec3& ROC::Light::GetFalloff() const
{
    return m_falloff;
}

unsigned char ROC::Light::GetType() const
{
    return m_type;
}
void ROC::Light::SetType(unsigned char f_type)
{
    m_type = f_type;
    btClamp<unsigned char>(m_type, LT_Directional, LT_Spotlight);
}
