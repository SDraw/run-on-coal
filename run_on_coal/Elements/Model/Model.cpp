#include "stdafx.h"

#include "Elements/Model/Model.h"
#include "Utils/Transformation.h"

#include "Elements/Collision.h"
#include "Elements/Geometry/Geometry.h"
#include "Elements/Model/AnimationController.h"
#include "Elements/Model/Bone.h"
#include "Elements/Model/Skeleton.h"

namespace ROC
{

extern const glm::mat4 g_IdentityMatrix;

}

ROC::Model::Model(Geometry *f_geometry)
{
    m_elementType = ET_Model;
    m_elementTypeName.assign("Model");

    m_localTransform = new Transformation();
    m_fullMatrix = g_IdentityMatrix;
    m_boundSphereRaduis = 0.f;
    m_updated = false;

    m_geometry = f_geometry;

    m_parent = nullptr;
    m_parentBone = nullptr;

    m_animController = new AnimationController();
    m_skeleton = nullptr;
    if(m_geometry)
    {
        m_boundSphereRaduis = m_geometry->GetBoundSphereRadius();
        if(m_geometry->HasBonesData())
        {
            m_skeleton = new Skeleton(m_geometry->GetBonesData());
            if(m_geometry->HasBonesCollisionData()) m_skeleton->InitStaticBoneCollision(m_geometry->GetBonesCollisionData(), this);
            if(m_geometry->HasJointsData()) m_skeleton->InitDynamicBoneCollision(m_geometry->GetJointsData(), this);
        }
    }
    m_collision = nullptr;
}
ROC::Model::~Model()
{
    delete m_localTransform;
    delete m_animController;
    delete m_skeleton;
}

void ROC::Model::SetPosition(const glm::vec3 &f_pos)
{
    m_localTransform->SetPosition(f_pos);
    if(m_collision) m_collision->SetPosition(f_pos);
}
const glm::vec3& ROC::Model::GetPosition() const
{
    return m_localTransform->GetPosition();
}

void ROC::Model::SetRotation(const glm::quat &f_rot)
{
    m_localTransform->SetRotation(f_rot);
    if(m_collision) m_collision->SetRotation(f_rot);
}
const glm::quat& ROC::Model::GetRotation() const
{
    return m_localTransform->GetRotation();
}

void ROC::Model::SetScale(const glm::vec3 &f_scl)
{
    m_localTransform->SetScale(f_scl);
    if(m_geometry)
    {
        m_boundSphereRaduis = m_geometry->GetBoundSphereRadius();
        if(m_localTransform->IsScaled()) m_boundSphereRaduis *= glm::length(f_scl);
    }
}
const glm::vec3& ROC::Model::GetScale() const
{
    return m_localTransform->GetScale();
}

void ROC::Model::SetParent(Model *f_model, int f_bone)
{
    m_parent = f_model;
    if(m_parent && (f_bone != -1)) m_parentBone = m_parent->GetSkeleton()->GetBones()[static_cast<size_t>(f_bone)];
    else m_parentBone = nullptr;
}

void ROC::Model::SetCollision(Collision *f_col)
{
    if(m_skeleton)
    {
        btRigidBody *l_body = (f_col ? f_col->GetRigidBody() : m_collision->GetRigidBody());
        m_skeleton->SetCollisionIgnoring(l_body, (f_col != nullptr));
    }
    m_collision = f_col;
}

void ROC::Model::Update(ModelUpdateStage f_stage)
{
    switch(f_stage)
    {
        case MUS_Matrix:
        {
            m_updated = false;
            m_localTransform->UpdateMatrix();
            if(m_parent)
            {
                if(m_parentBone)
                {
                    if(m_parent->IsUpdated() || m_parentBone->IsUpdated() || m_localTransform->IsUpdated())
                    {
                        if(m_parentBone->IsDynamic())
                        {
                            // Almost accurate, inverse bone body offset is needed
                            m_parentBone->GetDynamicBody()->getWorldTransform().getOpenGLMatrix(glm::value_ptr(m_fullMatrix));
                            m_fullMatrix = m_fullMatrix*m_localTransform->GetMatrix();
                        }
                        else
                        {
                            m_fullMatrix = m_parentBone->GetFullMatrix()*m_localTransform->GetMatrix();
                            m_fullMatrix = m_parent->m_fullMatrix*m_fullMatrix;
                        }
                        m_updated = true;
                    }
                }
                else
                {
                    if(m_parent->IsUpdated() || m_localTransform->IsUpdated())
                    {
                        m_fullMatrix = m_parent->m_fullMatrix*m_localTransform->GetMatrix();
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
        } break;

        case MUS_Collision:
        {
            if(m_collision)
            {
                m_updated = false;
                if(m_collision->IsActive())
                {
                    glm::vec3 l_pos;
                    m_collision->GetPosition(l_pos);
                    m_localTransform->SetPosition(l_pos);

                    glm::quat l_rot;
                    m_collision->GetRotation(l_rot);
                    m_localTransform->SetRotation(l_rot);

                    m_localTransform->UpdateMatrix();
                    if(m_localTransform->IsUpdated())
                    {
                        std::memcpy(&m_fullMatrix, &m_localTransform->GetMatrix(), sizeof(glm::mat4));
                        m_updated = true;
                    }
                }
            }
        } break;

        case MUS_Animation:
        {
            if(m_skeleton)
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
