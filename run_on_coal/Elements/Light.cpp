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

void ROC::Light::SetType(unsigned char f_type)
{
    m_type = f_type;
    btClamp<unsigned char>(m_type, LT_Directional, LT_Spotlight);
}
