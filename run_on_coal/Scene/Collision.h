#pragma once

namespace ROC
{

class Collision
{
    btRigidBody *m_rigidBody;
public:
    void SetPosition(glm::vec3 &f_pos);
    void GetPosition(glm::vec3 &f_pos);
    void SetRotation(glm::quat &f_rot);
    void GetRotation(glm::quat &f_rot);
    enum RigidType { Sphere = 0U, Box, Cylinder, Capsule, Cone, Last };
protected:
    Collision();
    ~Collision();
    bool Create(unsigned char f_type, glm::vec3 &f_size);
    btRigidBody* GetRigidBody();
    friend class ElementManager;
    friend class PhysicsManager;
};

}
