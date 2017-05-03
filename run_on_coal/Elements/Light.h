#pragma once
#include "Elements/Element.h"

namespace ROC
{

class Light : public Element
{
    glm::vec3 m_direction;
    glm::vec4 m_color;
    glm::vec4 m_params;
public:
    inline void SetParams(const glm::vec4 &f_val) { std::memcpy(&m_params, &f_val, sizeof(glm::vec4)); }
    inline void GetParams(glm::vec4 &f_val) { std::memcpy(&f_val, &m_params, sizeof(glm::vec4)); }
    inline void SetColor(const glm::vec3 &f_vec) { std::memcpy(&m_color, &f_vec, sizeof(glm::vec3)); }
    inline void GetColor(glm::vec3 &f_vec) { std::memcpy(&f_vec, &m_color, sizeof(glm::vec3)); }
    inline void SetDirection(const glm::vec3 &f_vec) { std::memcpy(&m_direction, &f_vec, sizeof(glm::vec3)); }
    inline void GetDirection(glm::vec3 &f_vec) { std::memcpy(&f_vec, &m_direction, sizeof(glm::vec3)); }
protected:
    inline glm::vec4& GetParamsRef() { return m_params; }
    inline glm::vec4& GetColorRef() { return m_color; }
    inline glm::vec3& GetDirectionRef() { return m_direction; }
    Light();
    ~Light();
    friend class ElementManager;
    friend class RenderManager;
};

}