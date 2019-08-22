#pragma once
#include "Interfaces/IElement.h"

namespace ROC
{

class IModel;

class ICollision : public virtual IElement
{
public:
    enum CollisionType : unsigned char
    {
        CT_Sphere = 0U,
        CT_Box,
        CT_Cylinder,
        CT_Capsule,
        CT_Cone
    };
    enum CollisionMotionType : unsigned char
    {
        CMT_Default = 0U,
        CMT_Static,
        CMT_Kinematic
    };

    virtual void SetPosition(const glm::vec3 &f_pos) = 0;
    virtual void GetPosition(glm::vec3 &f_pos) const = 0;

    virtual void SetRotation(const glm::quat &f_rot) = 0;
    virtual void GetRotation(glm::quat &f_rot) const = 0;

    virtual void GetScale(glm::vec3 &f_val) const = 0;

    virtual void SetVelocity(const glm::vec3 &f_val) = 0;
    virtual void GetVelocity(glm::vec3 &f_val) const = 0;

    virtual void SetAngularVelocity(const glm::vec3 &f_val) = 0;
    virtual void GetAngularVelocity(glm::vec3 &f_val) const = 0;

    virtual void SetLinearFactor(const glm::vec3 &f_val) = 0;
    virtual void GetLinearFactor(glm::vec3 &f_val) const = 0;

    virtual void SetAngularFactor(const glm::vec3 &f_val) = 0;
    virtual void GetAngularFactor(glm::vec3 &f_val) const = 0;

    virtual float GetMass() const = 0;

    virtual void SetFriction(float f_val) = 0;
    virtual float GetFriction() const = 0;

    virtual void ApplyForce(const glm::vec3 &f_force, const glm::vec3 &f_rp) = 0;
    virtual void ApplyCentralForce(const glm::vec3 &f_force) = 0;

    virtual void ApplyImpulse(const glm::vec3 &f_impulse, const glm::vec3 &f_rp) = 0;
    virtual void ApplyCentralImpulse(const glm::vec3 &f_impulse) = 0;

    virtual void ApplyTorque(const glm::vec3 &f_torque, bool f_impulse) = 0;

    virtual void SetMotionType(unsigned char f_type) = 0;
    virtual unsigned char GetMotionType() const = 0;

    virtual void GetMatrix(glm::mat4 &f_mat) const = 0;
};

}