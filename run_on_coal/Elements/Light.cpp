#include "stdafx.h"
#include "Elements/Light.h"

ROC::Light::Light()
{
    m_elementType = ElementType::LightElement;

    m_direction = glm::vec3(0.f, -1.f, 0.f);
    m_color = glm::vec4(1.f);
    m_params = glm::vec4(0.f, 1.f, 0.f, 0.f);
}
ROC::Light::~Light()
{
}
