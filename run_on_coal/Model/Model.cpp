#include "stdafx.h"
#include "Model/Animation.h"
#include "Model/Bone.h"
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
        std::vector<Geometry::geometryBoneData> l_bonesData;
        m_geometry->GetBonesData(l_bonesData);
        m_skeleton = new Skeleton(l_bonesData);
        if(m_geometry->HasChainsData())
        {
            std::vector<std::vector<Geometry::geometryChainData>> l_chains;
            m_geometry->GetChainsData(l_chains);
            m_skeleton->InitRigidity(l_chains);
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

bool ROC::Model::UpdateMatrix()
{
    bool l_updated = false;
    if(m_rebuildMatrix)
    {
        glm::mat4 l_indent(1.f);
        m_localMatrix = glm::translate(l_indent,m_position)*glm::toMat4(m_rotation)*glm::scale(l_indent,m_scale);
        l_updated = true;
        m_rebuildMatrix = false;
    }
    if(m_parent)
    {
        if(m_parent->UpdateMatrix() || m_parentBone != -1 || l_updated)
        {
            glm::mat4 l_parentMatrix;
            m_parent->GetMatrix(l_parentMatrix);
            if(m_parentBone != -1) 
            {
                glm::mat4 l_parentBoneMatrix;
                m_parent->GetBoneMatrix(m_parentBone,l_parentBoneMatrix);
                l_parentMatrix *= l_parentBoneMatrix;
            }
            m_matrix = l_parentMatrix*m_localMatrix;
            l_updated = true;
        }
    }
    else std::memcpy(&m_matrix,&m_localMatrix,sizeof(glm::mat4));
    return l_updated;
}
void ROC::Model::UpdateSkeleton()
{
    if(m_animation)
    {
        int l_leftFrame,l_rightFrame;
        float l_lerpDelta;
        if(!m_animation->GetInterval(m_animCurrentTick,l_leftFrame,l_rightFrame,l_lerpDelta)) return;

        std::vector<float> l_leftFrameData,l_rightFrameData;
        if(!m_animation->GetFrameData(l_leftFrame,l_leftFrameData)) return;
        if(!m_animation->GetFrameData(l_rightFrame,l_rightFrameData)) return;
        m_skeleton->Update(l_leftFrameData,l_rightFrameData,l_lerpDelta);
    }
    else m_skeleton->Update();
}
void ROC::Model::UpdateAnimationTick()
{
    unsigned long l_sysTick = Utils::GetSystemTick();
    unsigned long l_difTick = unsigned(long(double(l_sysTick-m_animLastTick)*double(m_animationSpeed)));
    m_animLastTick = l_sysTick;
    m_animCurrentTick += l_difTick;
    m_animCurrentTick %= m_animation->m_durationTotal;
}

bool ROC::Model::IsDrawable()
{
    return (m_geometry != NULL);
}
int ROC::Model::GetType()
{
    //0 - none, 1 - static, 2 - rigged/animated
    return (m_geometry ? (m_skeleton ? 2 : 1) : 0);
}
bool ROC::Model::IsRigid()
{
    return (m_rigidBody != NULL);
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

void ROC::Model::GetMatrix(glm::mat4 &f_mat)
{
    std::memcpy(&f_mat,&m_matrix,sizeof(glm::mat4));
}
unsigned int ROC::Model::GetMaterialCount()
{
    return (m_geometry ? m_geometry->GetMaterialCount() : 0);
}
void ROC::Model::GetMaterialType(unsigned int f_material, std::bitset<8U> &f_type)
{
    if(!m_geometry) return;
    m_geometry->GetMaterialType(f_material,f_type);
}
void ROC::Model::GetMaterialParam(unsigned int f_material,glm::vec4 &f_vec)
{
    if(!m_geometry) return;
    m_geometry->GetMaterialParam(f_material,f_vec);
}
void ROC::Model::DrawMaterial(unsigned int f_material, bool f_texturize, bool f_binding)
{
    if(!m_geometry) return;
    m_geometry->DrawMaterial(f_material,f_texturize,f_binding);
}

void ROC::Model::SetParent(Model *f_model, int f_bone)
{
    m_parent = f_model;
    m_parentBone = f_bone;
    m_rebuildMatrix = true;
}
ROC::Model* ROC::Model::GetParent()
{
    return m_parent;
}

ROC::Animation* ROC::Model::GetAnimation()
{
    return m_animation;
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
    }
    UpdateSkeleton();
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
float ROC::Model::GetAnimationSpeed()
{
    return m_animation ? m_animationSpeed : -1.f;
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

bool ROC::Model::SetBonePosition(unsigned int f_bone, glm::vec3 &f_val)
{
    if(!m_skeleton) return false;
    if(m_skeleton->m_boneVector.size() <= f_bone) return false;
    m_skeleton->m_boneVector[f_bone]->SetPosition(f_val,true);
    return true;
}
bool ROC::Model::SetBoneRotation(unsigned int f_bone, glm::quat &f_val)
{
    if(!m_skeleton) return false;
    if(m_skeleton->m_boneVector.size() <= f_bone) return false;
    m_skeleton->m_boneVector[f_bone]->SetRotation(f_val,true);
    return true;
}
bool ROC::Model::SetBoneScale(unsigned int f_bone, glm::vec3 &f_val)
{
    if(!m_skeleton) return false;
    if(m_skeleton->m_boneVector.size() <= f_bone) return false;
    m_skeleton->m_boneVector[f_bone]->SetScale(f_val,true);
    return true;
}

bool ROC::Model::HasSkeleton()
{
    return (m_skeleton != NULL);
}
bool ROC::Model::HasRigidSkeleton()
{
    return (m_skeleton ? m_skeleton->m_rigid : false);
}
int ROC::Model::GetBonesCount()
{
    return (m_skeleton ? static_cast<int>(m_skeleton->m_boneVector.size()) : -1);
}
void ROC::Model::GetBoneMatrices(std::vector<glm::mat4> &f_mat)
{
    if(!m_skeleton) return;
    f_mat.insert(f_mat.begin(),m_skeleton->m_boneMatrices.begin(),m_skeleton->m_boneMatrices.end());
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
}

bool ROC::Model::GetBoneMatrix(unsigned int f_bone,glm::mat4 &f_mat)
{
    if(!m_skeleton) return false;
    if(f_bone >= m_skeleton->m_boneVector.size()) return false;
    std::memcpy(&f_mat,&m_skeleton->m_boneVector[f_bone]->m_matrix,sizeof(glm::mat4));
    return true;
}

void ROC::Model::SetGeometry(Geometry *f_geometry)
{
    m_geometry = f_geometry;
}

GLuint ROC::Model::GetMaterialVAO(unsigned int f_material)
{
    if(!m_geometry) return 0U;
    return m_geometry->GetMaterialVAO(f_material);
}
GLuint ROC::Model::GetMaterialTexture(unsigned int f_material)
{
    if(!m_geometry) return 0U;
    return m_geometry->GetMaterialTexture(f_material);
}

//Physics
bool ROC::Model::SetRigidity(unsigned char f_type, float f_mass, glm::vec3 &f_dim)
{
    if(m_rigidBody || m_parent || f_mass < 0.f) return false;
    if(f_type >= RigidType::Last) return false;

    btVector3 l_inertia;
    btCollisionShape *l_shape = NULL;

    switch(f_type)
    {
        case RigidType::Box:
            l_shape = new btBoxShape((btVector3&)f_dim);
            break;
        case RigidType::Sphere:
            l_shape = new btSphereShape(f_dim.x);
            break;
        case RigidType::Cylinder:
            l_shape = new btCylinderShape((btVector3&)f_dim);
            break;
        case RigidType::Capsule:
            l_shape = new btCapsuleShape(f_dim.x,f_dim.y);
            break;
        case RigidType::Cone:
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
float ROC::Model::GetFriction()
{
    return (m_rigidBody ? m_rigidBody->getFriction() : -1.f);
}

btRigidBody* ROC::Model::GetRidigBody()
{
    return m_rigidBody;
}

void ROC::Model::UpdateSkeletonChains()
{
    if(!m_skeleton) return;
    m_skeleton->UpdateChains(m_matrix);
}
void ROC::Model::UpdateSkeletonRigidBones()
{
    if(!m_skeleton) return;
    m_skeleton->UpdateRigidBones(m_matrix);
}
