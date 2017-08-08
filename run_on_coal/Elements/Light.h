#pragma once
#include "Elements/Element.h"

namespace ROC
{

class Light final : public Element
{
    glm::vec3 m_direction;
    glm::vec4 m_color;
    glm::vec4 m_params;
public:
    inline void SetParams(const glm::vec4 &f_val) { std::memcpy(&m_params, &f_val, sizeof(glm::vec4)); }
    inline const glm::vec4& GetParams() const { return m_params; }
    inline void SetColor(const glm::vec3 &f_vec) { std::memcpy(&m_color, &f_vec, sizeof(glm::vec3)); }
    inline const glm::vec4& GetColor() const { return m_color; }
    inline void SetDirection(const glm::vec3 &f_vec) { std::memcpy(&m_direction, &f_vec, sizeof(glm::vec3)); }
    inline const glm::vec3& GetDirection() const { return m_direction; }
protected:
    Light();
    ~Light();

    inline glm::vec4& GetParamsRef() { return m_params; }
    inline glm::vec4& GetColorRef() { return m_color; }
    inline glm::vec3& GetDirectionRef() { return m_direction; }

    friend class ElementManager;
    friend class RenderManager;
};

}
