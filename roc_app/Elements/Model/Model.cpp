#include "stdafx.h"

#include "Elements/Model/Model.h"
#include "Elements/Animation/Animation.h"
#include "Elements/Collision.h"
#include "Elements/Geometry/Geometry.h"
#include "Elements/Model/AnimationController.h"
#include "Elements/Model/Skeleton.h"
#include "Utils/Transformation.h"

#include "Elements/Model/Bone.h"

namespace ROC
{

extern const glm::mat4 g_IdentityMatrix;
extern const glm::vec3 g_EmptyVec3;

}

ROC::Model::Model(Geometry *f_geometry)
{
    m_elementType = ET_Model;

    m_localTransform = new Transformation();
    m_fullMatrix = g_IdentityMatrix;
    m_boundSphereRaduis = 0.f;
    m_updated = false;

    m_geometry = f_geometry;

    m_parentModel = nullptr;
    m_parentBone = nullptr;

    m_animController = nullptr;
    m_skeleton = nullptr;
    if(m_geometry)
    {
        m_boundSphereRaduis = m_geometry->GetBoundSphereRadius();
        if(m_geometry->HasBonesData())
        {
            m_skeleton = new Skeleton(m_geometry->GetBonesData());
            if(m_geometry->HasBonesCollisionData()) m_skeleton->InitStaticBoneCollision(m_geometry->GetBonesCollisionData(), this);
            if(m_geometry->HasJointsData()) m_skeleton->InitDynamicBoneCollision(m_geometry->GetJointsData(), this);
            m_animController = new AnimationController();
        }

        Element::AddParent(m_geometry);
        m_geometry->AddChild(this);
    }

    m_collision = nullptr;
}
ROC::Model::~Model()
{
    delete m_localTransform;
    delete m_animController;
    delete m_skeleton;
}

float ROC::Model::GetBoundSphereRadius() const
{
    return m_boundSphereRaduis;
}

void ROC::Model::SetPosition(const glm::vec3 &f_pos)
{
    m_localTransform->SetPosition(f_pos);
}
const glm::vec3& ROC::Model::GetPosition() const
{
    return m_localTransform->GetPosition();
}

void ROC::Model::SetRotation(const glm::quat &f_rot)
{
    m_localTransform->SetRotation(f_rot);
}
const glm::quat& ROC::Model::GetRotation() const
{
    return m_localTransform->GetRotation();
}

void ROC::Model::SetScale(const glm::vec3 &f_scl)
{
    m_localTransform->SetScale(f_scl);
}
const glm::vec3& ROC::Model::GetScale() const
{
    return m_localTransform->GetScale();
}

const glm::mat4& ROC::Model::GetFullMatrix() const
{
    return m_fullMatrix;
}

bool ROC::Model::AttachTo(Model *f_model, int f_bone)
{
    bool l_result = false;
    if((m_parentModel != f_model) && !Element::HasChild(f_model))
    {
        if(m_parentModel) Element::RemoveParent(m_parentModel);

        m_parentModel = f_model;
        Element::AddParent(m_parentModel);
        m_parentModel->AddChild(this);

        if((f_bone > -1) && m_parentModel->HasSkeleton())
        {
            size_t l_bone = std::min(static_cast<size_t>(f_bone), m_parentModel->GetSkeleton()->GetBonesCount() - 1U);
            m_parentBone = m_parentModel->GetSkeleton()->GetBones()[l_bone];
        }
        else m_parentBone = nullptr;

        l_result = true;
    }
    return l_result;
}
bool ROC::Model::Dettach()
{
    bool l_result = false;
    if(m_parentModel)
    {
        Element::RemoveParent(m_parentModel);
        m_parentModel = nullptr;
        m_parentBone = nullptr;

        l_result = true;
    }
    return l_result;
}

bool ROC::Model::SetCollision(Collision *f_col)
{
    bool l_result = false;
    if(m_collision != f_col)
    {
        if(m_collision) Element::RemoveParent(m_collision);

        m_collision = f_col;
        Element::AddParent(m_collision);
        m_collision->AddChild(this);

        l_result = true;
    }
    return l_result;
}
bool ROC::Model::RemoveCollision()
{
    bool l_result = false;
    if(m_collision)
    {
        Element::RemoveParent(m_collision);
        m_collision = nullptr;
        l_result = true;
    }
    return l_result;
}

bool ROC::Model::SetAnimation(Animation *f_anim)
{
    bool l_result = false;
    if(m_skeleton && m_animController)
    {
        if(m_skeleton->GetBonesCount() == f_anim->GetBonesCount())
        {
            if(m_animController->GetAnimation() != f_anim)
            {
                Animation *l_oldAnim = m_animController->GetAnimation();
                if(l_oldAnim) Element::RemoveParent(l_oldAnim);

                Element::AddParent(f_anim);
                f_anim->AddChild(this);

                m_animController->SetAnimation(f_anim);

                l_result = true;
            }
        }
    }
    return l_result;
}
bool ROC::Model::RemoveAnimation()
{
    bool l_result = false;
    if(m_animController)
    {
        Animation *l_anim = m_animController->GetAnimation();
        if(l_anim)
        {
            Element::RemoveParent(l_anim);
            m_animController->SetAnimation(nullptr);

            l_result = true;
        }
    }
    return l_result;
}
ROC::Animation* ROC::Model::GetAnimation() const
{
    Animation *l_anim = nullptr;
    if(m_animController) l_anim = m_animController->GetAnimation();
    return l_anim;
}
bool ROC::Model::PlayAnimation(bool f_loop)
{
    bool l_result = false;
    if(m_animController) l_result = m_animController->Play(f_loop);
    return l_result;
}
bool ROC::Model::PauseAnimation()
{
    bool l_result = false;
    if(m_animController) l_result = m_animController->Pause();
    return l_result;
}
bool ROC::Model::ResetAnimation()
{
    bool l_result = false;
    if(m_animController) l_result = m_animController->Reset();
    return l_result;
}
bool ROC::Model::GetAnimationProperty(ModelAnimationProperty f_prop, float &f_value)
{
    if(m_animController)
    {
        switch(f_prop)
        {
            case MAP_Speed:
                f_value = m_animController->GetSpeed();
                break;
            case MAP_Progress:
                f_value = m_animController->GetProgress();
                break;
            case Map_BlendTime:
                f_value = static_cast<float>(m_animController->GetBlendTime());
                break;
        }
    }
    return (m_animController != nullptr);
}
bool ROC::Model::SetAnimationProperty(ModelAnimationProperty f_prop, float f_value)
{
    if(m_animController)
    {
        switch(f_prop)
        {
            case MAP_Speed:
                m_animController->SetSpeed(f_value);
                break;
            case MAP_Progress:
                m_animController->SetProgress(f_value);
                break;
            case Map_BlendTime:
                m_animController->SetBlendTime(static_cast<unsigned int>(f_value));
                break;
        }
    }
    return (m_animController != nullptr);
}

void ROC::Model::Update(ModelUpdateStage f_stage)
{
    switch(f_stage)
    {
        case MUS_Collision:
        {
            if(m_collision)
            {
                m_updated = false;
                m_localTransform->UpdateMatrix();

                if(m_collision->IsActive() || m_localTransform->IsUpdated())
                {
                    m_collision->GetMatrix(m_fullMatrix);
                    m_fullMatrix *= m_localTransform->GetMatrix();
                    m_updated = true;
                }
            }
        } break;

        case MUS_Matrix:
        {
            m_updated = false;
            m_localTransform->UpdateMatrix();

            if(m_parentModel)
            {
                if(m_parentBone)
                {
                    if(m_parentModel->IsUpdated() || m_parentBone->IsUpdated() || m_localTransform->IsUpdated())
                    {
                        if(m_parentBone->IsDynamic())
                        {
                            // Critical stuff, add inverse bone body offset later
                            m_parentBone->GetDynamicBody()->getCenterOfMassTransform().getOpenGLMatrix(glm::value_ptr(m_fullMatrix));
                            m_fullMatrix *= m_localTransform->GetMatrix();
                        }
                        else
                        {
                            m_fullMatrix = m_parentBone->GetFullMatrix()*m_localTransform->GetMatrix();
                            m_fullMatrix = m_parentModel->m_fullMatrix*m_fullMatrix;
                        }
                        m_updated = true;
                    }
                }
                else
                {
                    if(m_parentModel->IsUpdated() || m_localTransform->IsUpdated())
                    {
                        m_fullMatrix = m_parentModel->m_fullMatrix*m_localTransform->GetMatrix();
                        m_updated = true;
                    }
                }
            }
            else
            {
                if(m_localTransform->IsUpdated())
                {
                    std::memcpy(&m_fullMatrix, &m_localTransform->GetMatrix(), sizeof(glm::mat4));
                    m_updated = true;
                }
            }

            // Update bounding sphere
            if(m_updated)
            {
                if(m_geometry)
                {
                    if(m_parentModel)
                    {
                        const glm::vec4 l_boundNormal(m_geometry->GetBoundSphereRadius(), 0.f, 0.f, 0.f);
                        const glm::vec4 l_boundNormalGlobal = m_fullMatrix*l_boundNormal;
                        m_boundSphereRaduis = glm::length(l_boundNormalGlobal);
                    }
                    else
                    {
                        m_boundSphereRaduis = m_geometry->GetBoundSphereRadius();
                        if(m_localTransform->IsScaled()) m_boundSphereRaduis *= glm::length(m_localTransform->GetScale());
                    }
                }
            }
        } break;

        case MUS_Animation:
        {
            if(m_skeleton && m_animController)
            {
                Animation *l_anim = m_animController->GetAnimation();
                if(l_anim)
                {
                    m_animController->Update();
                    if(m_animController->IsPlaying()) m_skeleton->Update(l_anim, m_animController->GetTick(), m_animController->GetBlendValue());
                }
            }
        } break;

        case MUS_SkeletonCollisionStatic:
        {
            if(m_skeleton) m_skeleton->UpdateCollision(Skeleton::SUS_Static, m_fullMatrix);
        } break;

        case MUS_SkeletonCollisionDynamic:
        {
            if(m_skeleton) m_skeleton->UpdateCollision(Skeleton::SUS_Dynamic, m_fullMatrix);
        } break;
    }
}

void ROC::Model::OnParentLinkDestroyed(Element *f_parent)
{
    switch(f_parent->GetElementType())
    {
        case ET_Animation:
            m_animController->SetAnimation(nullptr);
            break;
        case ET_Geometry:
            m_geometry = nullptr;
            break;
        case ET_Model:
        {
            m_parentModel = nullptr;
            m_parentBone = nullptr;
        } break;
        case ET_Collision:
            m_collision = nullptr;
            break;
    }

    Element::OnParentLinkDestroyed(f_parent);
}

// ROC::Collidable
void ROC::Model::GetRigidBodies(std::vector<btRigidBody*> &f_vec)
{
    if(m_skeleton)
    {
        for(auto l_skeletonCol : m_skeleton->GetCollision()) f_vec.push_back(l_skeletonCol->m_rigidBody);
        for(auto l_joint : m_skeleton->GetJoints())
        {
            f_vec.push_back(l_joint->m_emptyBody);
            for(auto l_jointPart : l_joint->m_parts) f_vec.push_back(l_jointPart->m_rigidBody);
        }
    }
}

// ROC::IModel
ROC::IGeometry* ROC::Model::GetIGeometry() const
{
    return m_geometry;
}
bool ROC::Model::AttachTo(IModel *f_model, int f_bone)
{
    return AttachTo(dynamic_cast<Model*>(f_model), f_bone);
}
ROC::IModel* ROC::Model::GetParentIModel() const
{
    return m_parentModel;
}
bool ROC::Model::SetICollision(ICollision *f_col)
{
    return SetCollision(dynamic_cast<Collision*>(f_col));
}
ROC::ICollision* ROC::Model::GetICollsion() const
{
    return m_collision;
}
bool ROC::Model::SetIAnimation(IAnimation *f_anim)
{
    return SetAnimation(dynamic_cast<Animation*>(f_anim));
}
ROC::IAnimation* ROC::Model::GetIAnimation() const
{
    return GetAnimation();
}
