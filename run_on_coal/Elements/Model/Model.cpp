#include "stdafx.h"
#include "Elements/Animation/Animation.h"
#include "Elements/Collision.h"
#include "Elements/Geometry/BoneData.h"
#include "Elements/Geometry/BoneJointData.h"
#include "Elements/Geometry/Geometry.h"
#include "Elements/Model/Bone.h"
#include "Elements/Model/Model.h"
#include "Elements/Model/Skeleton.h"
#include "Utils/SystemTick.h"
#include "Utils/Utils.h"

const glm::mat4 g_IdentityMatrix(1.f);
const glm::vec3 g_DefaultPosition(0.f);
const glm::quat g_DefaultRotation(1.f, 0.f, 0.f, 0.f);
const glm::vec3 g_DefaultScale(1.f, 1.f, 1.f);

ROC::Model::Model(Geometry *f_geometry)
{
    m_elementType = ElementType::ModelElement;

    m_position = glm::vec3(0.f);
    m_rotation = glm::quat(1.f, 0.f, 0.f, 0.f);
    m_scale = glm::vec3(1.f);
    m_localMatrix = glm::mat4(1.f);
    m_matrix = glm::mat4(1.f);
    m_boundSphere = 0.f;
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
        m_boundSphere = m_geometry->GetBoundSphere();
        if(m_geometry->HasBonesData())
        {
            m_skeleton = new Skeleton(m_geometry->GetBonesDataRef());
            if(m_geometry->HasBonesCollisionData()) m_skeleton->InitStaticBoneCollision(m_geometry->GetBonesCollisionDataRef());
            if(m_geometry->HasJointsData()) m_skeleton->InitDynamicBoneCollision(m_geometry->GetJointsDataRef());
        }
    }
    m_collision = NULL;
}
ROC::Model::~Model()
{
    if(m_skeleton) delete m_skeleton;
}

void ROC::Model::SetPosition(const glm::vec3 &f_pos)
{
    if(std::memcmp(&m_position, &f_pos, sizeof(glm::vec3)) != 0)
    {
        std::memcpy(&m_position, &f_pos, sizeof(glm::vec3));
        if(m_collision) m_collision->SetPosition(f_pos);
        else m_rebuildMatrix = true;
    }
}
void ROC::Model::GetPosition(glm::vec3 &f_pos, bool f_global)
{
    if(m_collision) m_collision->GetPosition(f_pos);
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

void ROC::Model::SetRotation(const glm::quat &f_rot)
{
    if(std::memcmp(&m_rotation, &f_rot, sizeof(glm::quat)) != 0)
    {
        std::memcpy(&m_rotation, &f_rot, sizeof(glm::quat));
        if(m_collision) m_collision->SetRotation(f_rot);
        else m_rebuildMatrix = true;
    }
}
void ROC::Model::GetRotation(glm::quat &f_rot, bool f_global)
{
    if(m_collision) m_collision->GetRotation(f_rot);
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

void ROC::Model::SetScale(const glm::vec3 &f_scl)
{
    if(!m_collision && (std::memcmp(&m_scale, &f_scl, sizeof(glm::vec3)) != 0))
    {
        std::memcpy(&m_scale, &f_scl, sizeof(glm::vec3));
        m_boundSphere = m_geometry->GetBoundSphere()*glm::compMax(m_scale);
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
        if(std::memcmp(&m_position, &g_DefaultPosition, sizeof(glm::vec3)) != 0) m_localMatrix = glm::translate(g_IdentityMatrix, m_position);
        else std::memcpy(&m_localMatrix, &g_IdentityMatrix, sizeof(glm::mat4));
        if(std::memcmp(&m_rotation, &g_DefaultRotation, sizeof(glm::quat)) != 0) m_localMatrix *= glm::mat4_cast(m_rotation);
        if(std::memcmp(&m_scale, &g_DefaultScale, sizeof(glm::vec3)) != 0) m_localMatrix *= glm::scale(g_IdentityMatrix, m_scale);
        m_rebuildMatrix = false;
        m_rebuilded = true;
    }
    if(m_parent)
    {
        if(m_parentBone != -1)
        {
            if(m_parent->m_skeleton->GetBonesVectorRef()[m_parentBone]->IsRebuilded() || m_parent->m_rebuilded)
            {
                std::memcpy(&m_matrix, &m_parent->m_matrix, sizeof(glm::mat4));
                m_matrix *= m_parent->m_skeleton->GetBonesVectorRef()[m_parentBone]->GetMatrixRef();
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
    if(m_animation) m_animation->GetData(m_animCurrentTick, m_skeleton->GetBonesVectorRef());
    m_skeleton->Update();
}
bool ROC::Model::HasSkeletonStaticBoneCollision() const
{
    return (m_skeleton ? m_skeleton->HasStaticBoneCollision() : false);
}
bool ROC::Model::HasSkeletonDynamicBoneCollision() const
{
    return (m_skeleton ? m_skeleton->HasDynamicBoneCollision() : false);
}

void ROC::Model::UpdateCollision()
{
    if(m_collision)
    {
        m_rebuilded = m_collision->IsActive();
        if(m_rebuilded)
        {
            m_collision->GetTransform(m_localMatrix, m_position, m_rotation);
            if(std::memcmp(&m_scale,&g_DefaultScale,sizeof(glm::vec3)) != 0) m_localMatrix *= glm::scale(g_IdentityMatrix, m_scale);
            std::memcpy(&m_matrix, &m_localMatrix, sizeof(glm::mat4));
        }
    }
}
