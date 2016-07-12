#include "stdafx.h"
#include "Scene/Collision.h"

ROC::Collision::Collision()
{
    m_rigidBody = NULL;
}
ROC::Collision::~Collision()
{
    if(m_rigidBody)
    {
        delete m_rigidBody->getMotionState();
        delete m_rigidBody;
    }
}

btRigidBody* ROC::Collision::GetRigidBody()
{
    return m_rigidBody;
}
bool ROC::Collision::Create(unsigned int f_type, glm::vec3 &f_size)
{
    if(m_rigidBody || f_type >= 5U) return false;
    btVector3 l_inertia;
    btCollisionShape *l_shape = NULL;
    switch(f_type)
    {
        case COLLISION_TYPE_SPHERE:
            l_shape = new btSphereShape(f_size.x);
            break;
        case COLLISION_TYPE_BOX:
            l_shape = new btBoxShape((btVector3&)f_size);
            break;
        case COLLISION_TYPE_CYLINDER:
            l_shape = new btCylinderShape((btVector3&)f_size);
            break;
        case COLLISION_TYPE_CAPSULE:
            l_shape = new btCapsuleShape(f_size.x,f_size.y);
            break;
        case COLLISION_TYPE_CONE:
            l_shape = new btConeShape(f_size.x,f_size.y);
            break;
    }
    l_shape->calculateLocalInertia(0.f,l_inertia);
    btTransform l_transform;
    l_transform.setIdentity();
    btDefaultMotionState *l_fallMotionState = new btDefaultMotionState(l_transform);
    if(!l_fallMotionState) return false;
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(0.f,l_fallMotionState,l_shape,l_inertia);
    m_rigidBody = new btRigidBody(fallRigidBodyCI);
    if(!m_rigidBody) return false;
    return true;
}

void ROC::Collision::GetPosition(glm::vec3 &f_pos)
{
    btTransform l_transform = m_rigidBody->getCenterOfMassTransform();
    btVector3 l_position = l_transform.getOrigin();
    std::memcpy(&f_pos,l_position,sizeof(glm::vec3));
}
void ROC::Collision::SetPosition(glm::vec3 &f_pos)
{
    btTransform l_transform = m_rigidBody->getCenterOfMassTransform();
    l_transform.setOrigin((btVector3&)f_pos);
    m_rigidBody->setCenterOfMassTransform(l_transform);
    m_rigidBody->activate(true);
}
void ROC::Collision::GetRotation(glm::quat &f_rot)
{
    btTransform l_transform = m_rigidBody->getCenterOfMassTransform();
    btQuaternion l_rotation = l_transform.getRotation();
    std::memcpy(&f_rot,l_rotation,sizeof(glm::quat));
}
void ROC::Collision::SetRotation(glm::quat &f_rot)
{
    btTransform l_transform = m_rigidBody->getCenterOfMassTransform();
    l_transform.setRotation((btQuaternion&)f_rot);
    m_rigidBody->setCenterOfMassTransform(l_transform);
    m_rigidBody->activate(true);
}
