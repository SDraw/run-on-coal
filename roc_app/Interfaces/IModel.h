#pragma once
#include "Interfaces/IElement.h"

namespace ROC
{

class IAnimation;
class ICollision;
class IGeometry;

class IModel : public virtual IElement
{
public:
    enum ModelAnimationProperty : unsigned char
    {
        MAP_Speed = 0U,
        MAP_Progress,
        Map_BlendTime
    };

    virtual IGeometry* GetGeometry() const = 0;
    virtual float GetBoundSphereRadius() const = 0;

    virtual void SetPosition(const glm::vec3 &f_pos) = 0;
    virtual const glm::vec3& GetPosition() const = 0;
    virtual void SetRotation(const glm::quat &f_rot) = 0;
    virtual const glm::quat& GetRotation() const = 0;
    virtual void SetScale(const glm::vec3 &f_scl) = 0;
    virtual const glm::vec3& GetScale() const = 0;
    virtual const glm::mat4& GetFullMatrix() const = 0;

    virtual bool AttachTo(IModel *f_model, int f_bone = -1) = 0;
    virtual bool Dettach() = 0;
    virtual IModel* GetParentModel() const = 0;

    virtual bool SetCollision(ICollision *f_col) = 0;
    virtual bool RemoveCollision() = 0;
    virtual ICollision* GetCollsion() const = 0;

    virtual bool SetAnimation(IAnimation *f_anim) = 0;
    virtual bool RemoveAnimation() = 0;
    virtual IAnimation* GetAnimation() const = 0;
    virtual bool PlayAnimation(bool f_loop) = 0;
    virtual bool PauseAnimation() = 0;
    virtual bool ResetAnimation() = 0;
    virtual bool GetAnimationProperty(ModelAnimationProperty f_prop, float &f_value) = 0;
    virtual bool SetAnimationProperty(ModelAnimationProperty f_prop, float f_value) = 0;
};

}
