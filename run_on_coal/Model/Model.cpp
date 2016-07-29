#include "stdafx.h"
#include "Model/Animation.h"
#include "Model/Bone.h"
#include "Model/BoneData.h"
#include "Model/BoneChainGroup.h"
#include "Model/Geometry.h"
#include "Model/Model.h"
#include "Model/Skeleton.h"
#include "Utils/Utils.h"


ROC::Model::Model(Geometry *f_geometry)
{
    m_geometry = f_geometry;
    m_rigidBody = NULL;

    if(m_geometry->HasBonesData())
    {
        std::vector<BoneData*> l_bonesData;
        m_geometry->GetBonesData(l_bonesData);
        m_skeleton = new Skeleton(l_bonesData);
        if(m_geometry->HasChainsData())
        {
            std::vector<BoneChainGroup*> l_chainGroups;
            m_geometry->GetChainsData(l_chainGroups);
            m_skeleton->InitRigidity(l_chainGroups);
        }
    }
    else m_skeleton = NULL;
    m_animation = NULL;
    m_animLastTick = 0U;
    m_animCurrentTick = 0U;
    m_animState = AnimationState::None;
    m_animationSpeed = 1.f;

    m_position = glm::vec3(0.f);
    m_rotation = glm::quat(1.f,0.f,0.f,0.f);
    m_scale = glm::vec3(1.f);

    m_localMatrix = glm::mat4(1.f);
    m_matrix = glm::mat4(1.f);
    m_rebuildMatrix = false;

    m_parent = NULL;
    m_parentBone = -1;
}
ROC::Model::~Model()
{
    if(m_skeleton) delete m_skeleton;
    RemoveRigidity();
}

void ROC::Model::UpdateMatrix()
{
    if(!m_parent && !m_rebuildMatrix) return;
    if(m_rebuildMatrix)
    {
        m_localMatrix = glm::translate(Bone::m_identity,m_position)*glm::toMat4(m_rotation)*glm::scale(Bone::m_identity,m_scale);
        m_rebuildMatrix = false;
    }
    if(m_parent)
    {
        m_parent->UpdateMatrix();
        glm::mat4 l_parentMatrix(m_parent->m_matrix);
        if(m_parentBone != -1) 
        {
            glm::mat4 l_parentBoneMatrix;
            m_parent->GetBoneMatrix(m_parentBone,l_parentBoneMatrix);
            l_parentMatrix *= l_parentBoneMatrix;
        }
        m_matrix = l_parentMatrix*m_localMatrix;
    }
    else std::memcpy(&m_matrix,&m_localMatrix,sizeof(glm::mat4));
}

void ROC::Model::UpdateSkeleton()
{
    if(m_animation)
    {
        unsigned int l_leftFrame,l_rightFrame;
        float l_lerpDelta;
        m_animation->GetInterval(m_animCurrentTick,l_leftFrame,l_rightFrame,l_lerpDelta);
        if(!m_animation->GetFrameData(l_rightFrame,l_leftFrame)) return;
        m_skeleton->Update(m_animation->m_leftFrame,m_animation->m_rightFrame,l_lerpDelta);
    }
    else m_skeleton->Update();
}
void ROC::Model::UpdateAnimationTick()
{
    unsigned long l_sysTick = Utils::GetSystemTick();
    unsigned long l_difTick = static_cast<unsigned long>(static_cast<double>(l_sysTick-m_animLastTick)*static_cast<double>(m_animationSpeed));
    m_animLastTick = l_sysTick;
    m_animCurrentTick += l_difTick;
    m_animCurrentTick %= m_animation->m_durationTotal;
}

void ROC::Model::SetPosition(glm::vec3 &f_pos, bool f_uRb)
{
    if(!std::memcmp(&m_position,&f_pos,sizeof(glm::vec3))) return;
    std::memcpy(&m_position,&f_pos,sizeof(glm::vec3));
    m_rebuildMatrix = true;
    if(f_uRb && m_rigidBody)
    {
        btTransform l_transform = m_rigidBody->getCenterOfMassTransform();
        l_transform.setOrigin((btVector3&)m_position);
        m_rigidBody->setCenterOfMassTransform(l_transform);
        m_rigidBody->activate(true);
    }
}
void ROC::Model::GetPosition(glm::vec3 &f_pos, bool f_global)
{
    if(f_global && m_parent)
    {
        btTransform l_transform;
        l_transform.setFromOpenGLMatrix((float*)&m_matrix);
        btVector3 l_pos = l_transform.getOrigin();
        std::memcpy(&f_pos,l_pos,sizeof(glm::vec3));
    }
    else std::memcpy(&f_pos,&m_position,sizeof(glm::vec3));
}

void ROC::Model::SetRotation(glm::quat &f_rot, bool f_uRb)
{
    if(!std::memcmp(&m_rotation,&f_rot,sizeof(glm::quat))) return;
    std::memcpy(&m_rotation,&f_rot,sizeof(glm::quat));
    m_rebuildMatrix = true;
    if(f_uRb && m_rigidBody)
    {
        btTransform l_transform = m_rigidBody->getCenterOfMassTransform();
        l_transform.setRotation((btQuaternion&)m_rotation);
        m_rigidBody->setCenterOfMassTransform(l_transform);
        m_rigidBody->activate(true);
    }
}
void ROC::Model::GetRotation(glm::quat &f_rot, bool f_global)
{
    if(f_global && m_parent)
    {
        btTransform l_transform;
        l_transform.setFromOpenGLMatrix((float*)&m_matrix);
        btQuaternion l_rot = l_transform.getRotation();
        std::memcpy(&f_rot,l_rot,sizeof(glm::vec3));
    }
    else std::memcpy(&f_rot,&m_rotation,sizeof(glm::quat));
}

void ROC::Model::SetScale(glm::vec3 &f_scl)
{
    if(!std::memcmp(&m_scale,&f_scl,sizeof(glm::vec3))) return;
    std::memcpy(&m_scale,&f_scl,sizeof(glm::vec3));
    m_rebuildMatrix = true;
}
void ROC::Model::GetScale(glm::vec3 &f_scl, bool f_global)
{
    if(f_global && m_parent)
    {
        glm::vec3 l_scale,l_skew;
        glm::quat l_rotation;
        glm::vec3 l_translation;
        glm::vec4 perspective;
        glm::decompose(m_matrix,l_scale,l_rotation,l_translation,l_skew,perspective);
        std::memcpy(&f_scl,&l_scale,sizeof(glm::vec3));
    }
    else std::memcpy(&f_scl,&m_scale,sizeof(glm::vec3));
}

unsigned int ROC::Model::GetMaterialCount()
{
    return (m_geometry ? m_geometry->GetMaterialCount() : 0U);
}
unsigned char ROC::Model::GetMaterialType(unsigned int f_material)
{
    return (m_geometry ? m_geometry->GetMaterialType(f_material) : 0U);
}
void ROC::Model::GetMaterialParam(unsigned int f_material,glm::vec4 &f_vec)
{
    if(m_geometry) m_geometry->GetMaterialParam(f_material,f_vec);
}
void ROC::Model::DrawMaterial(unsigned int f_material, bool f_texturize, bool f_binding)
{
    if(m_geometry) m_geometry->DrawMaterial(f_material,f_texturize,f_binding);
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
        UpdateSkeleton();
    }
}
void ROC::Model::UpdateAnimation()
{
    if(!m_skeleton) return;
    if(m_animation)
    {
        if(m_animState == AnimationState::Paused) return;
        UpdateAnimationTick();
        UpdateSkeleton();
    }
}
bool ROC::Model::PlayAnimation()
{
    if(!m_animation) return false;
    if(m_animState == AnimationState::Playing) return true;
    if(m_animState == AnimationState::Paused || m_animLastTick == 0) m_animLastTick = Utils::GetSystemTick();
    m_animState = AnimationState::Playing;
    return true;
}
bool ROC::Model::PauseAnimation()
{
    if(!m_skeleton || !m_animation) return false;
    if(m_animState == AnimationState::Paused) return true;
    m_animState = AnimationState::Paused;
    return true;
}
bool ROC::Model::ResetAnimation()
{
    if(!m_skeleton || !m_animation) return false;
    m_animCurrentTick = 0U;
    m_animLastTick = Utils::GetSystemTick();
    UpdateSkeleton();
    return true;
}
bool ROC::Model::SetAnimationSpeed(float f_val)
{
    if(!m_animation || f_val < 0.f) return false;
    m_animationSpeed = f_val;
    return true;
}
bool ROC::Model::SetAnimationProgress(float f_val)
{
    if(!m_animation || f_val < 0.f || f_val > 1.f) return false;
    m_animCurrentTick = static_cast<unsigned long>(double(m_animation->m_durationTotal)*double(f_val));
    return true;
}
float ROC::Model::GetAnimationProgress() 
{ 
    return (m_animation ? static_cast<float>(double(m_animCurrentTick)/double(m_animation->m_durationTotal)) : -1.f); 
}

bool ROC::Model::HasRigidSkeleton()
{
    return (m_skeleton ? m_skeleton->m_rigid : false);
}
int ROC::Model::GetBonesCount()
{
    return (m_skeleton ? static_cast<int>(m_skeleton->m_bonesCount) : -1);
}
void ROC::Model::GetBoneMatrices(std::vector<glm::mat4> &f_mat)
{
    if(m_skeleton) f_mat.insert(f_mat.begin(),m_skeleton->m_boneMatrices.begin(),m_skeleton->m_boneMatrices.end());
}
void ROC::Model::GetSkeletonRigidData(std::vector<btRigidBody*> &f_rb, std::vector<btTypedConstraint*> &f_cs)
{
    for(auto iter : m_skeleton->m_chainsVector)
    {
        for(auto iter1 : iter)
        {
            if(iter1.m_rigidBody) f_rb.push_back(iter1.m_rigidBody);
            if(iter1.m_constraint) f_cs.push_back(iter1.m_constraint);
        }
    }
    f_rb.insert(f_rb.end(),m_skeleton->m_jointVector.begin(),m_skeleton->m_jointVector.end());
}

bool ROC::Model::GetBoneMatrix(unsigned int f_bone,glm::mat4 &f_mat)
{
    if(!m_skeleton) return false;
    if(f_bone >= m_skeleton->m_bonesCount) return false;
    std::memcpy(&f_mat,&m_skeleton->m_boneVector[f_bone]->m_matrix,sizeof(glm::mat4));
    return true;
}

GLuint ROC::Model::GetMaterialVAO(unsigned int f_material)
{
    return (m_geometry ? m_geometry->GetMaterialVAO(f_material) : 0U);
}
GLuint ROC::Model::GetMaterialTexture(unsigned int f_material)
{
    return (m_geometry ? m_geometry->GetMaterialTexture(f_material) : 0U);
}

//Physics
bool ROC::Model::SetRigidity(unsigned char f_type, float f_mass, glm::vec3 &f_dim)
{
    if(m_rigidBody || m_parent || f_mass < 0.f) return false;
    if(f_type > MODEL_RIGIDITY_TYPE_CONE) return false;

    btVector3 l_inertia;
    btCollisionShape *l_shape = NULL;

    switch(f_type)
    {
        case MODEL_RIGIDITY_TYPE_SPHERE:
            l_shape = new btSphereShape(f_dim.x);
            break;
        case MODEL_RIGIDITY_TYPE_BOX:
            l_shape = new btBoxShape((btVector3&)f_dim);
            break;
        case MODEL_RIGIDITY_TYPE_CYLINDER:
            l_shape = new btCylinderShape((btVector3&)f_dim);
            break;
        case MODEL_RIGIDITY_TYPE_CAPSULE:
            l_shape = new btCapsuleShape(f_dim.x,f_dim.y);
            break;
        case MODEL_RIGIDITY_TYPE_CONE:
            l_shape = new btConeShape(f_dim.x,f_dim.y);
            break;
    }
    l_shape->calculateLocalInertia(f_mass,l_inertia);
    btDefaultMotionState *l_fallMotionState = new btDefaultMotionState(btTransform((btQuaternion&)m_rotation,(btVector3&)m_position));
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(f_mass,l_fallMotionState,l_shape,l_inertia);
    m_rigidBody = new btRigidBody(fallRigidBodyCI);
    return true;
}
bool ROC::Model::RemoveRigidity()
{
    if(!m_rigidBody) return false;
    delete m_rigidBody->getMotionState();
    delete m_rigidBody;
    m_rigidBody = NULL;
    return true;
}
bool ROC::Model::SetVelocity(glm::vec3 &f_val)
{
    if(!m_rigidBody) return false;
    btVector3 l_velocity(f_val.x,f_val.y,f_val.z);
    m_rigidBody->setLinearVelocity(l_velocity);
    m_rigidBody->activate(true);
    return true;
}
bool ROC::Model::GetVelocity(glm::vec3 &f_val)
{
    if(!m_rigidBody) return false;
    btVector3 l_velocity = m_rigidBody->getLinearVelocity();
    std::memcpy(&f_val,l_velocity,sizeof(glm::vec3));
    return true;
}
bool ROC::Model::SetAngularVelocity(glm::vec3 &f_val)
{
    if(!m_rigidBody) return false;
    btVector3 l_velocity(f_val.x,f_val.y,f_val.z);
    m_rigidBody->setAngularVelocity(l_velocity);
    m_rigidBody->activate(true);
    return true;
}
bool ROC::Model::GetAngularVelocity(glm::vec3 &f_val)
{
    if(!m_rigidBody) return false;
    btVector3 l_velocity = m_rigidBody->getAngularVelocity();
    std::memcpy(&f_val,l_velocity,sizeof(glm::vec3));
    return true;
}
float ROC::Model::GetMass()
{
    if(!m_rigidBody) return -1.f;
    float l_invMass = m_rigidBody->getInvMass();
    return ((l_invMass == 0.f) ? 0.f : (1.f/l_invMass));
}
bool ROC::Model::SetFriction(float f_val)
{
    if(!m_rigidBody || f_val < 0.f) return false;
    m_rigidBody->setFriction(f_val);
    m_rigidBody->activate(true);
    return true;
}

void ROC::Model::UpdateSkeletonChains()
{
    if(m_skeleton) m_skeleton->UpdateChains(m_matrix);
}
void ROC::Model::UpdateSkeletonRigidBones()
{
    if(m_skeleton) m_skeleton->UpdateRigidBones(m_matrix);
}

void ROC::Model::UpdateRigidity()
{
    if(m_rigidBody)
    {
        if(m_rigidBody->isActive())
        {
            const btTransform &l_transform = m_rigidBody->getCenterOfMassTransform();
            std::memcpy(&m_position,l_transform.getOrigin(),sizeof(glm::vec3));
            std::memcpy(&m_rotation,l_transform.getRotation(),sizeof(glm::quat));
            l_transform.getOpenGLMatrix((float*)&m_localMatrix);
            std::memcpy(&m_matrix,&m_localMatrix,sizeof(glm::mat4));
        }
    }
}
