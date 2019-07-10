#pragma once
#include "Interfaces/ILight.h"
#include "Elements/Element.h"

namespace ROC
{

class Light final : public Element, public virtual ILight
{
    glm::vec3 m_position;
    glm::vec3 m_direction;
    glm::vec4 m_color;
    glm::vec2 m_cutoff;
    glm::vec3 m_falloff;
    unsigned char m_type;

    Light(const Light &that) = delete;
    Light& operator=(const Light &that) = delete;
public:
    void SetPosition(const glm::vec3 &f_vec);
    const glm::vec3& GetPosition() const;

    void SetDirection(const glm::vec3 &f_vec);
    const glm::vec3& GetDirection() const;

    void SetColor(const glm::vec4 &f_vec);
    const glm::vec4& GetColor() const;

    void SetCutoff(const glm::vec2 &f_vec);
    const glm::vec2& GetCutoff() const;

    void SetFalloff(const glm::vec3 &f_val);
    const glm::vec3& GetFalloff() const;

    unsigned char GetType() const;
    void SetType(unsigned char f_type);
protected:
    explicit Light(unsigned char f_type);
    ~Light();

    friend class ElementManager;
    friend class RenderManager;
    friend class Scene;
};

}
