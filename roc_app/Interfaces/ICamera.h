#pragma once
#include "Interfaces/IElement.h"

namespace ROC
{

class ICamera : public virtual IElement
{
public:
    enum CameraProjectionType : unsigned char
    {
        CPT_Perspective = 0U,
        CPT_Orthogonal,
        CPT_Screen,
        CPT_VRLeft,
        CPT_VRRight
    };

    virtual void SetProjectionType(unsigned char f_type) = 0;
    virtual unsigned char GetProjectionType() const = 0;

    virtual void SetFOV(float f_fov) = 0;
    virtual float GetFOV() const = 0;

    virtual void SetAspectRatio(float f_ratio) = 0;
    virtual float GetAspectRatio() const = 0;

    virtual void SetOrthoParams(const glm::vec4 &f_size) = 0;
    virtual const glm::vec4& GetOrthoParams() const = 0;

    virtual void SetDepth(const glm::vec2 &f_depth) = 0;
    virtual const glm::vec2& GetDepth() const = 0;

    virtual void SetPosition(const glm::vec3 &f_pos) = 0;
    virtual const glm::vec3& GetPosition() const = 0;

    virtual void SetDirection(const glm::vec3 &f_dir) = 0;
    virtual void SetDirection(const glm::quat &f_dir) = 0;
    virtual const glm::vec3& GetDirection() const = 0;

    virtual void SetUpDirection(const glm::vec3 &f_dir) = 0;
    virtual const glm::vec3& GetUpDirection() const = 0;

    virtual const glm::mat4& GetViewMatrix() const = 0;
    virtual const glm::mat4& GetProjectionMatrix() const = 0;
    virtual const glm::mat4& GetViewProjectionMatrix() const = 0;
};

}
