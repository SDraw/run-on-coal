#pragma once
#include "Elements/Element.h"

namespace ROC
{

class Light final : public Element
{
    glm::vec3 m_position;
    glm::vec3 m_direction;
    glm::vec4 m_color;
    glm::vec2 m_cutoff;
    glm::vec3 m_falloff;
    unsigned char m_type;

    Light(const Light &that);
    Light& operator=(const Light &that);
public:
    enum LightType : unsigned char
    {
        LT_Directional = 0U,
        LT_Point,
        LT_Spotlight
    };

    inline void SetPosition(const glm::vec3 &f_vec) { std::memcpy(&m_position, &f_vec, sizeof(glm::vec3)); }
    inline const glm::vec3& GetPosition() const { return m_position; }

    inline void SetDirection(const glm::vec3 &f_vec) { std::memcpy(&m_direction, &f_vec, sizeof(glm::vec3)); }
    inline const glm::vec3& GetDirection() const { return m_direction; }

    inline void SetColor(const glm::vec4 &f_vec) { std::memcpy(&m_color, &f_vec, sizeof(glm::vec4)); }
    inline const glm::vec4& GetColor() const { return m_color; }

    inline void SetCutoff(const glm::vec2 &f_vec) { std::memcpy(&m_cutoff, &f_vec, sizeof(glm::vec2)); }
    inline const glm::vec2& GetCutoff() const { return m_cutoff; }

    inline void SetFalloff(const glm::vec3 &f_val) { std::memcpy(&m_falloff, &f_val, sizeof(glm::vec3)); }
    inline const glm::vec3& GetFalloff() const { return m_falloff; }

    inline unsigned char GetType() const { return m_type; }
    void SetType(unsigned char f_type);
protected:
    explicit Light(unsigned char f_type);
    ~Light();

    friend class ElementManager;
    friend class RenderManager;
};

}
