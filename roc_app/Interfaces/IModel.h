#pragma once
#include "ICollidable.h"

namespace ROC
{

class IAnimation;
class ICollision;
class IGeometry;

class IModel : public virtual ICollidable
{
public:
    enum ModelAnimationProperty : unsigned char
    {
        MAP_Speed = 0U,
        MAP_Progress,
        Map_BlendTime
    };

    virtual IGeometry* GetIGeometry() const = 0;
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
    virtual IModel* GetParentIModel() const = 0;
    virtual bool SetICollision(ICollision *f_col) = 0;
    virtual bool RemoveCollision() = 0;
    virtual ICollision* GetICollsion() const = 0;
    virtual bool SetIAnimation(IAnimation *f_anim) = 0;
    virtual bool RemoveAnimation() = 0;
    virtual IAnimation* GetIAnimation() const = 0;
    virtual bool PlayAnimation(bool f_loop) = 0;
    virtual bool PauseAnimation() = 0;
    virtual bool ResetAnimation() = 0;
    virtual bool GetAnimationProperty(ModelAnimationProperty f_prop, float &f_value) = 0;
    virtual bool SetAnimationProperty(ModelAnimationProperty f_prop, float f_value) = 0;
};

}
