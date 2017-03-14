#pragma once
#include "Elements/Element.h"
#define COLLISION_TYPE_NONE -1
#define COLLISION_TYPE_SPHERE 0
#define COLLISION_TYPE_BOX 1
#define COLLISION_TYPE_CYLINDER 2
#define COLLISION_TYPE_CAPSULE 3
#define COLLISION_TYPE_CONE 4

namespace ROC
{

class Collision : public Element
{
    btRigidBody *m_rigidBody;
    int m_type;
public:
    inline int GetType() const { return m_type; }

    void SetPosition(const glm::vec3 &f_pos);
    void GetPosition(glm::vec3 &f_pos);
    void SetRotation(const glm::quat &f_rot);
    void GetRotation(glm::quat &f_rot);
protected:
    Collision();
    ~Collision();

    bool Create(int f_type, const glm::vec3 &f_size);
    inline btRigidBody* GetRigidBody() { return m_rigidBody; }

    friend class ElementManager;
    friend class PhysicsManager;
};

}
