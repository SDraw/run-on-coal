#include "stdafx.h"
#include "Elements/Animation/Animation.h"
#include "Elements/Geometry/BoneData.h"
#include "Elements/Geometry/BoneJointData.h"
#include "Elements/Geometry/Geometry.h"
#include "Elements/Model/Bone.h"
#include "Elements/Model/Model.h"
#include "Elements/Model/Skeleton.h"
#include "Utils/SystemTick.h"
#include "Utils/Utils.h"

ROC::Model::Model(Geometry *f_geometry)
{
    m_elementType = ElementType::ModelElement;

    m_position = glm::vec3(0.f);
    m_rotation = glm::quat(1.f, 0.f, 0.f, 0.f);
    m_scale = glm::vec3(1.f);
    m_localMatrix = glm::mat4(1.f);
    m_matrix = glm::mat4(1.f);
    m_rebuildMatrix = false;
    m_rebuilded = false;

    m_geometry = f_geometry;

    m_parent = NULL;
    m_parentBone = -1;

    m_animation = NULL;
    m_animLastTick = 0U;
    m_animCurrentTick = 0U;
    m_animState = AnimationState::None;
    m_animationSpeed = 1.f;

    m_skeleton = NULL;
    if(m_geometry)
    {
        if(m_geometry->HasBonesData())
        {
            m_skeleton = new Skeleton(m_geometry->GetBonesDataRef());
            if(m_geometry->HasBonesCollisionData()) m_skeleton->InitStaticBoneCollision(m_geometry->GetBonesCollisionDataRef());
            if(m_geometry->HasJointsData()) m_skeleton->InitDynamicBoneCollision(m_geometry->GetJointsDataRef());
        }
    }

    m_rigidBody = NULL;
    m_rigidType = MODEL_RIGIDITY_TYPE_NONE;
}
ROC::Model::~Model()
{
    if(m_skeleton) delete m_skeleton;
    RemoveCollision();
}

void ROC::Model::SetPosition(glm::vec3 &f_pos)
{
    if(std::memcmp(&m_position, &f_pos, sizeof(glm::vec3)) != 0)
    {
        std::memcpy(&m_position, &f_pos, sizeof(glm::vec3));
        if(m_rigidBody)
        {
            btTransform l_transform = m_rigidBody->getCenterOfMassTransform();
            l_transform.setOrigin(btVector3(m_position.x, m_position.y, m_position.z));
            m_rigidBody->setCenterOfMassTransform(l_transform);
            m_rigidBody->activate(true);
        }
        else m_rebuildMatrix = true;
    }
}
void ROC::Model::GetPosition(glm::vec3 &f_pos, bool f_global)
{
    if(m_rigidBody) std::memcpy(&f_pos, m_rigidBody->getCenterOfMassTransform().getOrigin().m_floats, sizeof(glm::vec3));
    else
    {
        if(f_global && m_parent)
        {
            btTransform l_transform;
            l_transform.setFromOpenGLMatrix(reinterpret_cast<float*>(&m_matrix));
            std::memcpy(&f_pos, l_transform.getOrigin().m_floats, sizeof(glm::vec3));
        }
        else std::memcpy(&f_pos, &m_position, sizeof(glm::vec3));
    }
}

void ROC::Model::SetRotation(glm::quat &f_rot)
{
    if(std::memcmp(&m_rotation, &f_rot, sizeof(glm::quat)) != 0)
    {
        std::memcpy(&m_rotation, &f_rot, sizeof(glm::quat));
        if(m_rigidBody)
        {
            btTransform l_transform = m_rigidBody->getCenterOfMassTransform();
            l_transform.setRotation(btQuaternion(m_rotation.x, m_rotation.y, m_rotation.z, m_rotation.w));
            m_rigidBody->setCenterOfMassTransform(l_transform);
            m_rigidBody->activate(true);
        }
        else m_rebuildMatrix = true;
    }
}
void ROC::Model::GetRotation(glm::quat &f_rot, bool f_global)
{
    if(m_rigidBody)
    {
        btQuaternion l_rotation = m_rigidBody->getCenterOfMassTransform().getRotation();
        f_rot.x = l_rotation.x();
        f_rot.y = l_rotation.y();
        f_rot.z = l_rotation.z();
        f_rot.w = l_rotation.w();
    }
    else
    {
        if(f_global && m_parent)
        {
            btTransform l_transform;
            l_transform.setFromOpenGLMatrix(reinterpret_cast<float*>(&m_matrix));
            btQuaternion l_rot = l_transform.getRotation();
            f_rot.x = l_rot.x();
            f_rot.y = l_rot.y();
            f_rot.z = l_rot.z();
            f_rot.w = l_rot.w();
        }
        else std::memcpy(&f_rot, &m_rotation, sizeof(glm::quat));
    }
}

void ROC::Model::SetScale(glm::vec3 &f_scl)
{
    if(!m_rigidBody && (std::memcmp(&m_scale, &f_scl, sizeof(glm::vec3)) != 0))
    {
        std::memcpy(&m_scale, &f_scl, sizeof(glm::vec3));
        m_rebuildMatrix = true;
    }
}
void ROC::Model::GetScale(glm::vec3 &f_scl, bool f_global)
{
    if(f_global && m_parent)
    {
        glm::vec3 l_scale, l_skew, l_translation;
        glm::quat l_rotation;
        glm::vec4 perspective;
        glm::decompose(m_matrix, l_scale, l_rotation, l_translation, l_skew, perspective);
        std::memcpy(&f_scl, &l_scale, sizeof(glm::vec3));
    }
    else std::memcpy(&f_scl, &m_scale, sizeof(glm::vec3));
}
void ROC::Model::UpdateMatrix()
{
    m_rebuilded = false;
    if(m_rebuildMatrix)
    {
        m_localMatrix = glm::translate(Bone::m_identity, m_position)*glm::toMat4(m_rotation)*glm::scale(Bone::m_identity, m_scale);
        m_rebuildMatrix = false;
        m_rebuilded = true;
    }
    if(m_parent)
    {
        if(m_parentBone != -1)
        {
            if(m_parent->m_skeleton->GetBonesVectorRef()[m_parentBone]->IsRebuilded() || m_parent->m_rebuilded)
            {
                glm::mat4 l_parentMatrix;
                std::memcpy(&l_parentMatrix, &m_parent->m_matrix, sizeof(glm::mat4));
                m_matrix = (l_parentMatrix*m_parent->m_skeleton->GetBonesVectorRef()[m_parentBone]->GetMatrixRef())*m_localMatrix;
                m_rebuilded = true;
            }
        }
        else
        {
            if(m_parent->m_rebuilded)
            {
                glm::mat4 l_parentMatrix;
                std::memcpy(&l_parentMatrix, &m_parent->m_matrix, sizeof(glm::mat4));
                m_matrix = l_parentMatrix*m_localMatrix;
                m_rebuilded = true;
            }
        }
    }
    else
    {
        if(m_rebuilded) std::memcpy(&m_matrix, &m_localMatrix, sizeof(glm::mat4));
    }
}

void ROC::Model::SetParent(Model *f_model, int f_bone)
{
    m_parent = f_model;
    m_parentBone = f_bone;
    m_rebuildMatrix = true;
}

void ROC::Model::SetAnimation(Animation *f_anim)
{
    m_animation = f_anim;
    if(m_animation)
    {
        m_animState = AnimationState::Paused;
        m_animCurrentTick = 0U;
        m_skeleton->ResetBonesInterpolation();
        UpdateSkeleton();
    }
    else
    {
        m_animationSpeed = 1.f;
        m_animState = AnimationState::None;
    }
}
void ROC::Model::UpdateAnimation()
{
    if(m_skeleton && m_animation && (m_animState != AnimationState::Paused))
    {
        unsigned int l_sysTick = SystemTick::GetTick();
        unsigned int l_difTick = static_cast<unsigned int>(static_cast<float>(l_sysTick - m_animLastTick)*m_animationSpeed);
        m_animLastTick = l_sysTick;
        m_animCurrentTick += l_difTick;
        m_animCurrentTick %= m_animation->GetTotalDuration();
        UpdateSkeleton();
    }
}
bool ROC::Model::PlayAnimation()
{
    if(m_animation)
    {
        if(m_animState != AnimationState::Playing)
        {
            if(m_animState == AnimationState::Paused || m_animLastTick == 0) m_animLastTick = SystemTick::GetTick();
            m_animState = AnimationState::Playing;
        }
    }
    return (m_animation != NULL);
}
bool ROC::Model::PauseAnimation()
{
    if(m_animation) m_animState = AnimationState::Paused;
    return (m_animation != NULL);
}
bool ROC::Model::ResetAnimation()
{
    if(m_animation)
    {
        m_animCurrentTick = 0U;
        m_animLastTick = SystemTick::GetTick();
        UpdateSkeleton();
    }
    return (m_animation != NULL);
}
bool ROC::Model::SetAnimationSpeed(float f_val)
{
    if(m_animation)
    {
        m_animationSpeed = f_val;
        if(m_animationSpeed < 0.f) m_animationSpeed = 0.f;
    }
    return (m_animation != NULL);
}
bool ROC::Model::SetAnimationProgress(float f_val)
{
    if(m_animation)
    {
        btClamp(f_val, 0.f, 1.f);
        m_animCurrentTick = static_cast<unsigned int>(float(m_animation->GetTotalDuration())*f_val);
    }
    return (m_animation != NULL);
}
float ROC::Model::GetAnimationProgress() const
{
    return (m_animation ? (static_cast<float>(m_animCurrentTick) / static_cast<float>(m_animation->GetTotalDuration())) : -1.f);
}

void ROC::Model::UpdateSkeleton()
{
    if(m_animation)
    {
        if(m_animation->CacheData(m_animCurrentTick)) m_skeleton->Update(m_animation->GetCachedDataRef());
    }
    else m_skeleton->Update();
}
bool ROC::Model::HasSkeletonStaticBoneCollision() const
{
    return (m_skeleton ? m_skeleton->HasStaticBoneCollision() : false);
}
bool ROC::Model::HasSkeletonDynamicBoneCollision() const
{
    return (m_skeleton ? m_skeleton->HasDynamicBoneCollision() : false);
}

bool ROC::Model::SetCollision(int f_type, float f_mass, glm::vec3 &f_dim)
{
    if(!m_rigidBody && !m_parent && (f_mass >= 0.f))
    {

        btVector3 l_inertia;
        btCollisionShape *l_shape = NULL;
        m_rigidType = f_type;
        btClamp(m_rigidType, MODEL_RIGIDITY_TYPE_SPHERE, MODEL_RIGIDITY_TYPE_CONE);
        switch(m_rigidType)
        {
            case MODEL_RIGIDITY_TYPE_SPHERE:
                l_shape = new btSphereShape(f_dim.x);
                break;
            case MODEL_RIGIDITY_TYPE_BOX:
                l_shape = new btBoxShape(btVector3(f_dim.x, f_dim.y, f_dim.z));
                break;
            case MODEL_RIGIDITY_TYPE_CYLINDER:
                l_shape = new btCylinderShape(btVector3(f_dim.x, f_dim.y, f_dim.z));
                break;
            case MODEL_RIGIDITY_TYPE_CAPSULE:
                l_shape = new btCapsuleShape(f_dim.x, f_dim.y);
                break;
            case MODEL_RIGIDITY_TYPE_CONE:
                l_shape = new btConeShape(f_dim.x, f_dim.y);
                break;
        }
        l_shape->calculateLocalInertia(f_mass, l_inertia);
        btDefaultMotionState *l_fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(m_rotation.x, m_rotation.y, m_rotation.z, m_rotation.w), btVector3(m_position.x, m_position.y, m_position.z)));
        btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(f_mass, l_fallMotionState, l_shape, l_inertia);
        m_rigidBody = new btRigidBody(fallRigidBodyCI);
    }
    return (m_rigidBody != NULL);
}
bool ROC::Model::RemoveCollision()
{
    if(m_rigidBody)
    {
        delete m_rigidBody->getMotionState();
        delete m_rigidBody;
        m_rigidBody = NULL;
        m_rigidType = MODEL_RIGIDITY_TYPE_NONE;
    }
    return (m_rigidBody == NULL);
}
bool ROC::Model::SetVelocity(glm::vec3 &f_val)
{
    if(m_rigidBody)
    {
        btVector3 l_velocity(f_val.x, f_val.y, f_val.z);
        m_rigidBody->setLinearVelocity(l_velocity);
        m_rigidBody->activate(true);
    }
    return (m_rigidBody != NULL);
}
bool ROC::Model::GetVelocity(glm::vec3 &f_val)
{
    if(m_rigidBody) std::memcpy(&f_val, m_rigidBody->getLinearVelocity().m_floats, sizeof(glm::vec3));
    return (m_rigidBody != NULL);
}
bool ROC::Model::SetAngularVelocity(glm::vec3 &f_val)
{
    if(m_rigidBody)
    {
        btVector3 l_velocity(f_val.x, f_val.y, f_val.z);
        m_rigidBody->setAngularVelocity(l_velocity);
        m_rigidBody->activate(true);
    }
    return (m_rigidBody != NULL);
}
bool ROC::Model::GetAngularVelocity(glm::vec3 &f_val)
{
    if(m_rigidBody) std::memcpy(&f_val, m_rigidBody->getAngularVelocity().m_floats, sizeof(glm::vec3));
    return (m_rigidBody != NULL);
}
float ROC::Model::GetMass()
{
    float l_mass = -1.f;
    if(m_rigidBody)
    {
        float l_invMass = m_rigidBody->getInvMass();
        l_mass = ((l_invMass == 0.f) ? 0.f : (1.f / l_invMass));
    }
    return l_mass;
}
bool ROC::Model::SetFriction(float f_val)
{
    if(m_rigidBody)
    {
        if(f_val < 0.f) f_val = 0.f;
        m_rigidBody->setFriction(f_val);
        m_rigidBody->activate(true);
    }
    return (m_rigidBody != NULL);
}
void ROC::Model::UpdateCollision()
{
    if(m_rigidBody)
    {
        m_rebuilded = m_rigidBody->isActive();
        if(m_rebuilded)
        {
            const btTransform &l_transform = m_rigidBody->getCenterOfMassTransform();
            btQuaternion l_rotation = l_transform.getRotation();
            std::memcpy(&m_position, l_transform.getOrigin().m_floats, sizeof(glm::vec3));
            m_rotation.x = l_rotation.x();
            m_rotation.y = l_rotation.y();
            m_rotation.z = l_rotation.z();
            m_rotation.w = l_rotation.w();
            l_transform.getOpenGLMatrix(reinterpret_cast<float*>(&m_localMatrix));
            std::memcpy(&m_matrix, &m_localMatrix, sizeof(glm::mat4));
        }
    }
}
