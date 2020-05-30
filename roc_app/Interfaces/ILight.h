#pragma once
#include "IElement.h"

namespace ROC
{

class ILight : public virtual IElement
{
public:
    enum LightType : unsigned char
    {
        LT_Directional = 0U,
        LT_Point,
        LT_Spotlight
    };

    virtual void SetPosition(const glm::vec3 &f_vec) = 0;
    virtual const glm::vec3& GetPosition() const = 0;
    virtual void SetDirection(const glm::vec3 &f_vec) = 0;
    virtual const glm::vec3& GetDirection() const = 0;
    virtual void SetColor(const glm::vec4 &f_vec) = 0;
    virtual const glm::vec4& GetColor() const = 0;
    virtual void SetCutoff(const glm::vec2 &f_vec) = 0;
    virtual const glm::vec2& GetCutoff() const = 0;
    virtual void SetFalloff(const glm::vec3 &f_val) = 0;
    virtual const glm::vec3& GetFalloff() const = 0;
    virtual unsigned char GetType() const = 0;
    virtual void SetType(unsigned char f_type) = 0;
};

}
