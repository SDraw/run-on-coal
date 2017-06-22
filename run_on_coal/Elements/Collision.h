#pragma once
#include "Elements/Element.h"
#define ROC_COLLISION_TYPE_NONE -1
#define ROC_COLLISION_TYPE_SPHERE 0
#define ROC_COLLISION_TYPE_BOX 1
#define ROC_COLLISION_TYPE_CYLINDER 2
#define ROC_COLLISION_TYPE_CAPSULE 3
#define ROC_COLLISION_TYPE_CONE 4
#define ROC_COLLISION_MOTION_DEFAULT 0
#define ROC_COLLISION_MOTION_STATIC 1
#define ROC_COLLISION_MOTION_KINEMATIC 2

namespace ROC
{

class Model;
class Collision final: public Element
{
    btRigidBody *m_rigidBody;
    int m_motionType;
    glm::vec3 m_scale;
    Model *m_parentModel;
public:
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

    void SetMotionType(int f_state);
    inline int GetMotionType() { return m_motionType; }

    void GetTransform(glm::mat4 &f_mat, glm::vec3 &f_pos, glm::quat &f_rot);
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
