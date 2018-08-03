#include "stdafx.h"

#include "Elements/Collision.h"
#include "Elements/Model/Model.h"

namespace ROC
{

extern const glm::vec3 g_DefaultScale;

}

ROC::Collision::Collision()
{
    m_elementType = ET_Collision;
    m_elementTypeName.assign("Collision");

    m_rigidBody = nullptr;
    m_motionType = CMT_Default;
    m_scale = g_DefaultScale;
}
ROC::Collision::~Collision()
{
    if(m_rigidBody)
    {
        delete m_rigidBody->getMotionState();
        delete m_rigidBody;
    }
}

bool ROC::Collision::Create(int f_type, const glm::vec3 &f_size, float f_mass)
{
    if(!m_rigidBody)
    {
        btClamp(f_type, static_cast<int>(CT_Sphere), static_cast<int>(CT_Cone));
        btVector3 l_inertia;
        btCollisionShape *l_shape = nullptr;
        switch(f_type)
        {
            case CT_Sphere:
                l_shape = new btSphereShape(f_size.x);
                break;
            case CT_Box:
                l_shape = new btBoxShape(btVector3(f_size.x, f_size.y, f_size.z));
                break;
            case CT_Cylinder:
                l_shape = new btCylinderShape(btVector3(f_size.x, f_size.y, f_size.z));
                break;
            case CT_Capsule:
                l_shape = new btCapsuleShape(f_size.x, f_size.y);
                break;
            case CT_Cone:
                l_shape = new btConeShape(f_size.x, f_size.y);
                break;
        }
        if(l_shape)
        {
            l_shape->calculateLocalInertia(f_mass, l_inertia);
            btTransform l_transform;
            l_transform.setIdentity();
            btDefaultMotionState *l_fallMotionState = new btDefaultMotionState(l_transform);
            btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(f_mass, l_fallMotionState, l_shape, l_inertia);
            m_rigidBody = new btRigidBody(fallRigidBodyCI);
            m_rigidBody->setUserPointer(this);
        }
    }
    return (m_rigidBody != nullptr);
}

void ROC::Collision::GetPosition(glm::vec3 &f_pos) const
{
    if(m_rigidBody)
    {
        btTransform l_transform;
        switch(m_motionType)
        {
            case CMT_Default: case CMT_Static:
                l_transform = m_rigidBody->getCenterOfMassTransform();
                break;
            case CMT_Kinematic:
                m_rigidBody->getMotionState()->getWorldTransform(l_transform);
                break;
        }
        std::memcpy(&f_pos, l_transform.getOrigin().m_floats, sizeof(glm::vec3));
    }
}
void ROC::Collision::SetPosition(const glm::vec3 &f_pos)
{
    if(m_rigidBody)
    {
        btTransform l_transform;
        switch(m_motionType)
        {
            case CMT_Default: case CMT_Static:
                l_transform = m_rigidBody->getCenterOfMassTransform();
                break;
            case CMT_Kinematic:
                m_rigidBody->getMotionState()->getWorldTransform(l_transform);
                break;
        }
        btVector3 l_pos(f_pos.x, f_pos.y, f_pos.z);
        l_transform.setOrigin(l_pos);
        switch(m_motionType)
        {
            case CMT_Default: case CMT_Static:
                m_rigidBody->setCenterOfMassTransform(l_transform);
                break;
            case CMT_Kinematic:
                m_rigidBody->getMotionState()->setWorldTransform(l_transform);
                break;
        }
        m_rigidBody->activate(true);
    }
}
void ROC::Collision::GetRotation(glm::quat &f_rot) const
{
    if(m_rigidBody)
    {
        btTransform l_transform;
        switch(m_motionType)
        {
            case CMT_Default: case CMT_Static:
                l_transform = m_rigidBody->getCenterOfMassTransform();
                break;
            case CMT_Kinematic:
                m_rigidBody->getMotionState()->getWorldTransform(l_transform);
                break;
        }
        btQuaternion l_rotation = l_transform.getRotation();
        for(int i = 0; i < 4; i++) f_rot[i] = l_rotation[i];
    }
}
void ROC::Collision::SetRotation(const glm::quat &f_rot)
{
    if(m_rigidBody)
    {
        btTransform l_transform;
        switch(m_motionType)
        {
            case CMT_Default: case CMT_Static:
                l_transform = m_rigidBody->getCenterOfMassTransform();
                break;
            case CMT_Kinematic:
                m_rigidBody->getMotionState()->getWorldTransform(l_transform);
                break;
        }
        btQuaternion l_rotation(f_rot.x, f_rot.y, f_rot.z, f_rot.w);
        l_transform.setRotation(l_rotation);
        switch(m_motionType)
        {
            case CMT_Default: case CMT_Static:
                m_rigidBody->setCenterOfMassTransform(l_transform);
                break;
            case CMT_Kinematic:
                m_rigidBody->getMotionState()->setWorldTransform(l_transform);
                break;
        }
        m_rigidBody->activate(true);
    }
}

void ROC::Collision::SetScale(const glm::vec3 &f_val)
{
    if(m_rigidBody)
    {
        std::memcpy(&m_scale, &f_val, sizeof(glm::vec3));
        m_rigidBody->getCollisionShape()->setLocalScaling(btVector3(m_scale.x, m_scale.y, m_scale.z));
    }
}
void ROC::Collision::GetScale(glm::vec3 &f_val) const
{
    std::memcpy(&f_val, &m_scale, sizeof(glm::vec3));
}

void ROC::Collision::SetVelocity(const glm::vec3 &f_val)
{
    if(m_rigidBody)
    {
        m_rigidBody->setLinearVelocity(btVector3(f_val.x, f_val.y, f_val.z));
        m_rigidBody->activate(true);
    }
}
void ROC::Collision::GetVelocity(glm::vec3 &f_val) const
{
    if(m_rigidBody) std::memcpy(&f_val, m_rigidBody->getLinearVelocity().m_floats, sizeof(glm::vec3));
}

void ROC::Collision::SetAngularVelocity(const glm::vec3 &f_val)
{
    if(m_rigidBody)
    {
        m_rigidBody->setAngularVelocity(btVector3(f_val.x, f_val.y, f_val.z));
        m_rigidBody->activate(true);
    }
}
void ROC::Collision::GetAngularVelocity(glm::vec3 &f_val) const
{
    if(m_rigidBody) std::memcpy(&f_val, m_rigidBody->getAngularVelocity().m_floats, sizeof(glm::vec3));
}

void ROC::Collision::SetLinearFactor(const glm::vec3 &f_val)
{
    if(m_rigidBody) m_rigidBody->setLinearFactor(btVector3(f_val.x, f_val.y, f_val.z));
}
void ROC::Collision::GetLinearFactor(glm::vec3 &f_val) const
{
    if(m_rigidBody) std::memcpy(&f_val, m_rigidBody->getLinearFactor().m_floats, sizeof(glm::vec3));
}

void ROC::Collision::SetAngularFactor(const glm::vec3 &f_val)
{
    if(m_rigidBody) m_rigidBody->setAngularFactor(btVector3(f_val.x, f_val.y, f_val.z));
}
void ROC::Collision::GetAngularFactor(glm::vec3 &f_val) const
{
    if(m_rigidBody) std::memcpy(&f_val, m_rigidBody->getAngularFactor().m_floats, sizeof(glm::vec3));
}

float ROC::Collision::GetMass() const
{
    float l_invMass = m_rigidBody ? m_rigidBody->getInvMass() : -1.f;
    float l_mass = ((l_invMass == 0.f) ? 0.f : (1.f / l_invMass));
    return l_mass;
}

void ROC::Collision::SetFriction(float f_val)
{
    if(m_rigidBody)
    {
        btClamp(f_val, 0.f, 1.f);
        m_rigidBody->setFriction(f_val);
        m_rigidBody->activate(true);
    }
}

void ROC::Collision::ApplyForce(const glm::vec3 &f_force, const glm::vec3 &f_rp)
{
    if(m_rigidBody)
    {
        btVector3 l_force(f_force.x, f_force.y, f_force.z);
        btVector3 l_relPos(f_rp.x, f_rp.y, f_rp.z);
        m_rigidBody->applyForce(l_force, l_relPos);
    }
}
void ROC::Collision::ApplyCentralForce(const glm::vec3 &f_force)
{
    if(m_rigidBody) m_rigidBody->applyCentralForce(btVector3(f_force.x, f_force.y, f_force.z));
}

void ROC::Collision::ApplyImpulse(const glm::vec3 &f_impulse, const glm::vec3 &f_rp)
{
    if(m_rigidBody)
    {
        btVector3 l_impulse(f_impulse.x, f_impulse.y, f_impulse.z);
        btVector3 l_relPos(f_rp.x, f_rp.y, f_rp.z);
        m_rigidBody->applyImpulse(l_impulse, l_relPos);
    }
}
void ROC::Collision::ApplyCentralImpulse(const glm::vec3 &f_impulse)
{
    if(m_rigidBody) m_rigidBody->applyCentralImpulse(btVector3(f_impulse.x, f_impulse.y, f_impulse.z));
}

void ROC::Collision::ApplyTorque(const glm::vec3 &f_torque, bool f_impulse)
{
    if(m_rigidBody)
    {
        btVector3 l_torque(f_torque.x, f_torque.y, f_torque.z);
        f_impulse ? m_rigidBody->applyTorqueImpulse(l_torque) : m_rigidBody->applyTorque(l_torque);
    }
}

void ROC::Collision::GetMatrix(glm::mat4 &f_mat) const
{
    if(m_rigidBody)
    {
        btTransform l_transform;
        switch(m_motionType)
        {
            case CMT_Default: case CMT_Static:
                l_transform = m_rigidBody->getCenterOfMassTransform();
                break;
            case CMT_Kinematic:
                m_rigidBody->getMotionState()->getWorldTransform(l_transform);
                break;
        }
        l_transform.getOpenGLMatrix(glm::value_ptr(f_mat));
    }
}

void ROC::Collision::SetMotionType(int f_type)
{
    if(m_rigidBody)
    {
        m_motionType = f_type;
        btClamp(m_motionType, static_cast<int>(CMT_Default), static_cast<int>(CMT_Kinematic));
        switch(m_motionType)
        {
            case CMT_Default:
            {
                m_rigidBody->setCollisionFlags(0);
                m_rigidBody->setActivationState(ACTIVE_TAG);
            } break;
            case CMT_Static:
            {
                m_rigidBody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
                m_rigidBody->setActivationState(ACTIVE_TAG);
            } break;
            case CMT_Kinematic:
            {
                m_rigidBody->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
                m_rigidBody->setActivationState(DISABLE_DEACTIVATION);
            } break;
        }
    }
}
