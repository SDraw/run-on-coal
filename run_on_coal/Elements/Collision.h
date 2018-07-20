#pragma once
#include "Elements/Element.h"

namespace ROC
{

class Model;
class Collision final : public Element
{
    btRigidBody *m_rigidBody;
    int m_motionType;
    glm::vec3 m_scale;
    Model *m_parentModel;
public:
    enum CollisionType
    {
        CT_None = -1,
        CT_Sphere,
        CT_Box,
        CT_Cylinder,
        CT_Capsule,
        CT_Cone
    };
    enum CollisionMotionType
    {
        CMT_Default,
        CMT_Static,
        CMT_Kinematic
    };

    void SetPosition(const glm::vec3 &f_pos);
    void GetPosition(glm::vec3 &f_pos);

    void SetRotation(const glm::quat &f_rot);
    void GetRotation(glm::quat &f_rot);

    void GetScale(glm::vec3 &f_val);

    void SetVelocity(const glm::vec3 &f_val);
    void GetVelocity(glm::vec3 &f_val);

    void SetAngularVelocity(const glm::vec3 &f_val);
    void GetAngularVelocity(glm::vec3 &f_val);

    void SetLinearFactor(const glm::vec3 &f_val);
    void GetLinearFactor(glm::vec3 &f_val);

    void SetAngularFactor(const glm::vec3 &f_val);
    void GetAngularFactor(glm::vec3 &f_val);

    float GetMass();

    void SetFriction(float f_val);
    inline float GetFriction() const { return (m_rigidBody ? m_rigidBody->getFriction() : -1.f); }

    void ApplyForce(const glm::vec3 &f_force, const glm::vec3 &f_rp);
    void ApplyCentralForce(const glm::vec3 &f_force);

    void ApplyImpulse(const glm::vec3 &f_impulse, const glm::vec3 &f_rp);
    void ApplyCentralImpulse(const glm::vec3 &f_impulse);

    void ApplyTorque(const glm::vec3 &f_torque, bool f_impulse);

    void SetMotionType(int f_type);
    inline int GetMotionType() const { return m_motionType; }
protected:
    Collision();
    ~Collision();
    bool Create(int f_type, const glm::vec3 &f_size, float f_mass);

    inline btRigidBody* GetRigidBody() { return m_rigidBody; }
    inline bool IsActive() { return m_rigidBody->isActive(); }

    void SetParentModel(Model *f_model);
    inline Model* GetParentModel() { return m_parentModel; }

    void SetScale(const glm::vec3 &f_val);

    friend class ElementManager;
    friend class InheritanceManager;
    friend class PhysicsManager;
    friend class Model;
};

}
