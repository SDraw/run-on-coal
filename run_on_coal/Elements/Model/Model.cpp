#include "stdafx.h"

#include "Elements/Model/Model.h"
#include "Elements/Animation/Animation.h"
#include "Elements/Model/Bone.h"
#include "Elements/Collision.h"
#include "Elements/Geometry/Geometry.h"
#include "Elements/Model/Skeleton.h"

#include "Utils/SystemTick.h"

namespace ROC
{

extern const glm::mat4 g_IdentityMatrix;
extern const glm::vec3 g_DefaultPosition;
extern const glm::quat g_DefaultRotation;
extern const glm::vec3 g_DefaultScale;
extern const float g_Epsilon;

}

ROC::Model::Model(Geometry *f_geometry)
{
    m_elementType = ElementType::ModelElement;
    m_elementTypeName.assign("Model");

    m_position = g_DefaultPosition;
    m_rotation = g_DefaultRotation;
    m_scale = g_DefaultScale;
    m_localMatrix = g_IdentityMatrix;
    m_matrix = g_IdentityMatrix;
    m_boundSphereRaduis = 0.f;
    m_rebuildMatrix = false;
    m_rebuilded = false;

    m_geometry = f_geometry;

    m_parent = nullptr;
    m_parentBone = -1;

    m_animation = nullptr;
    m_animationTick = 0U;
    m_animState = AnimationState::None;
    m_animationSpeed = -1.f;

    m_skeleton = nullptr;
    if(m_geometry)
    {
        m_boundSphereRaduis = m_geometry->GetBoundSphereRadius();
        if(m_geometry->HasBonesData())
        {
            m_skeleton = new Skeleton(m_geometry->GetBonesDataRef());
            if(m_geometry->HasBonesCollisionData()) m_skeleton->InitStaticBoneCollision(m_geometry->GetBonesCollisionDataRef(), this);
            if(m_geometry->HasJointsData()) m_skeleton->InitDynamicBoneCollision(m_geometry->GetJointsDataRef(), this);
        }
    }
    m_collision = nullptr;
}
ROC::Model::~Model()
{
    delete m_skeleton;
}

void ROC::Model::SetPosition(const glm::vec3 &f_pos)
{
    std::memcpy(&m_position, &f_pos, sizeof(glm::vec3));
    if(m_collision) m_collision->SetPosition(f_pos);
    else m_rebuildMatrix = true;
}
void ROC::Model::GetPosition(glm::vec3 &f_pos)
{
    std::memcpy(&f_pos, &m_position, sizeof(glm::vec3));
}

void ROC::Model::SetRotation(const glm::quat &f_rot)
{
    std::memcpy(&m_rotation, &f_rot, sizeof(glm::quat));
    if(m_collision) m_collision->SetRotation(f_rot);
    else m_rebuildMatrix = true;
}
void ROC::Model::GetRotation(glm::quat &f_rot)
{
    std::memcpy(&f_rot, &m_rotation, sizeof(glm::quat));
}

void ROC::Model::SetScale(const glm::vec3 &f_scl)
{
    std::memcpy(&m_scale, &f_scl, sizeof(glm::vec3));
    m_boundSphereRaduis = (m_geometry ? m_geometry->GetBoundSphereRadius()*glm::compMax(m_scale) : glm::length(m_scale));
    m_rebuildMatrix = true;
}
void ROC::Model::GetScale(glm::vec3 &f_scl)
{
    std::memcpy(&f_scl, &m_scale, sizeof(glm::vec3));
}
void ROC::Model::UpdateMatrix()
{
    m_rebuilded = false;
    if(m_rebuildMatrix)
    {
        btTransform l_transform = btTransform::getIdentity();
        btVector3 l_position(m_position.x, m_position.y, m_position.z);
        l_transform.setOrigin(l_position);
        btQuaternion l_rotation(m_rotation.x, m_rotation.y, m_rotation.z, m_rotation.w);
        l_transform.setRotation(l_rotation);
        l_transform.getOpenGLMatrix(glm::value_ptr(m_localMatrix));

        bool l_useScale = false;
        for(int i = 0; i < 3; i++)
        {
            if(glm::epsilonNotEqual(m_scale[i], g_DefaultScale[i], g_Epsilon))
            {
                l_useScale = true;
                break;
            }
        }
        if(l_useScale) m_localMatrix *= glm::scale(g_IdentityMatrix, m_scale);

        m_rebuildMatrix = false;
        m_rebuilded = true;
    }
    if(m_parent)
    {
        if(m_parentBone != -1)
        {
            if(m_parent->m_skeleton->GetBonesVectorRef()[m_parentBone]->IsRebuilded() || m_parent->m_rebuilded)
            {
                glm::mat4 l_boneMatrix;
                m_parent->m_skeleton->GetBonesVectorRef()[m_parentBone]->GetMatrix(l_boneMatrix);
                std::memcpy(&m_matrix, &m_parent->m_matrix, sizeof(glm::mat4));
                m_matrix *= l_boneMatrix;
                m_matrix *= m_localMatrix;
                m_rebuilded = true;
            }
        }
        else
        {
            if(m_parent->m_rebuilded)
            {
                std::memcpy(&m_matrix, &m_parent->m_matrix, sizeof(glm::mat4));
                m_matrix *= m_localMatrix;
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
        m_animationTick = 0U;
        m_animationSpeed = 1.f;
        m_skeleton->EnableBoneBlending();
        UpdateSkeleton();
    }
    else
    {
        m_animationSpeed = -1.f;
        m_animState = AnimationState::None;
    }
}
void ROC::Model::UpdateAnimation()
{
    if(m_skeleton && m_animation && (m_animState != AnimationState::Paused))
    {
        m_animationTick += static_cast<unsigned int>(static_cast<float>(SystemTick::GetDelta())*m_animationSpeed);
        m_animationTick %= m_animation->GetDuration();
        UpdateSkeleton();
    }
}
bool ROC::Model::PlayAnimation()
{
    if(m_animation)
    {
        if(m_animState != AnimationState::Playing) m_animState = AnimationState::Playing;
    }
    return (m_animation != nullptr);
}
bool ROC::Model::PauseAnimation()
{
    if(m_animation) m_animState = AnimationState::Paused;
    return (m_animation != nullptr);
}
bool ROC::Model::ResetAnimation()
{
    if(m_animation)
    {
        m_animationTick = 0U;
        UpdateSkeleton();
    }
    return (m_animation != nullptr);
}
bool ROC::Model::SetAnimationSpeed(float f_val)
{
    if(m_animation)
    {
        m_animationSpeed = f_val;
        if(m_animationSpeed < 0.f) m_animationSpeed = 0.f;
    }
    return (m_animation != nullptr);
}
bool ROC::Model::SetAnimationProgress(float f_val)
{
    if(m_animation)
    {
        btClamp(f_val, 0.f, 1.f);
        m_animationTick = static_cast<unsigned int>(float(m_animation->GetDuration())*f_val);
    }
    return (m_animation != nullptr);
}
float ROC::Model::GetAnimationProgress() const
{
    return (m_animation ? (static_cast<float>(m_animationTick) / static_cast<float>(m_animation->GetDuration())) : -1.f);
}
bool ROC::Model::SetAnimationBlendFactor(float f_val)
{
    if(m_skeleton) m_skeleton->SetBoneBlendFactor(f_val);
    return (m_skeleton != nullptr);
}
float ROC::Model::GetAnimationBlendFactor() const
{
    return (m_skeleton ? m_skeleton->GetBoneBlendFactor() : -1.f);
}

void ROC::Model::UpdateSkeleton()
{
    if(m_animation) m_animation->GetData(m_animationTick, m_skeleton->GetBonesVectorRef());
    m_skeleton->Update();
}

void ROC::Model::SetCollision(Collision *f_col)
{
    if(m_skeleton)
    {
        btRigidBody *l_body = (f_col ? f_col->GetRigidBody() : m_collision->GetRigidBody());
        bool l_collisionIgnoring = (f_col != nullptr);
        if(m_skeleton->HasStaticBoneCollision())
        {
            for(auto iter : m_skeleton->GetCollisionVectorRef())
            {
                l_body->setIgnoreCollisionCheck(iter->m_rigidBody, l_collisionIgnoring);
                iter->m_rigidBody->setIgnoreCollisionCheck(l_body, l_collisionIgnoring);
            }
        }
        if(m_skeleton->HasDynamicBoneCollision())
        {
            for(auto iter : m_skeleton->GetJointVectorRef())
            {
                for(auto iter1 : iter->m_partsVector)
                {
                    l_body->setIgnoreCollisionCheck(iter1->m_rigidBody, l_collisionIgnoring);
                    iter1->m_rigidBody->setIgnoreCollisionCheck(l_body, l_collisionIgnoring);
                }
            }
        }
    }
    m_collision = f_col;
}
void ROC::Model::UpdateCollision()
{
    if(m_collision)
    {
        m_rebuilded = m_collision->IsActive();
        if(m_rebuilded)
        {
            m_collision->GetTransform(m_localMatrix, m_position, m_rotation);
            if(m_scale != g_DefaultScale) m_localMatrix *= glm::scale(g_IdentityMatrix, m_scale);
            std::memcpy(&m_matrix, &m_localMatrix, sizeof(glm::mat4));
        }
    }
}
