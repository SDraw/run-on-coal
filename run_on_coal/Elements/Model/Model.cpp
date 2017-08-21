#include "stdafx.h"

#include "Elements/Model/Model.h"

#include "Elements/Collision.h"
#include "Elements/Geometry/Geometry.h"
#include "Elements/Model/AnimationController.h"
#include "Elements/Model/Bone.h"
#include "Elements/Model/Skeleton.h"

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

    m_localPosition = g_DefaultPosition;
    m_localRotation = g_DefaultRotation;
    m_localScale = g_DefaultScale;
    m_localMatrix = g_IdentityMatrix;
    m_globalMatrix = g_IdentityMatrix;
    m_boundSphereRaduis = 0.f;
    m_useScale = false;
    m_rebuildMatrix = false;
    m_rebuilded = false;
    m_globalPosition = g_DefaultPosition;
    m_globalRotation = g_DefaultRotation;
    m_globalScale = g_DefaultScale;

    m_geometry = f_geometry;

    m_parent = nullptr;
    m_parentBone = -1;

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
    delete m_animController;
    delete m_skeleton;
}

void ROC::Model::SetLocalPosition(const glm::vec3 &f_pos)
{
    std::memcpy(&m_localPosition, &f_pos, sizeof(glm::vec3));
    if(!m_parent) std::memcpy(&m_globalPosition, &m_localPosition, sizeof(glm::vec3));
    if(m_collision) m_collision->SetPosition(f_pos);
    else m_rebuildMatrix = true;
}
void ROC::Model::SetLocalRotation(const glm::quat &f_rot)
{
    std::memcpy(&m_localRotation, &f_rot, sizeof(glm::quat));
    if(!m_parent) std::memcpy(&m_globalRotation, &m_localRotation, sizeof(glm::quat));
    if(m_collision) m_collision->SetRotation(f_rot);
    else m_rebuildMatrix = true;
}
void ROC::Model::SetLocalScale(const glm::vec3 &f_scl)
{
    std::memcpy(&m_localScale, &f_scl, sizeof(glm::vec3));
    if(!m_parent)
    {
        std::memcpy(&m_globalScale, &m_localScale, sizeof(glm::vec3));
        if(m_geometry) m_boundSphereRaduis = m_geometry->GetBoundSphereRadius()*glm::compMax(m_localScale);
    }

    bool l_useScale = false;
    for(int i = 0; i < 3; i++)
    {
        if(glm::epsilonNotEqual(m_localScale[i], g_DefaultScale[i], g_Epsilon))
        {
            l_useScale = true;
            break;
        }
    }
    m_useScale = l_useScale;

    m_rebuildMatrix = true;
}

void ROC::Model::UpdateGlobalTransform()
{
    if(m_parent)
    {
        btTransform l_transform;
        l_transform.setFromOpenGLMatrix(glm::value_ptr(m_globalMatrix));

        std::memcpy(&m_globalPosition, l_transform.getOrigin().m_floats, sizeof(glm::vec3));

        btQuaternion l_globalRotation = l_transform.getRotation();
        for(int i = 0; i < 4; i++) m_globalRotation[i] = l_globalRotation[i];

        std::memcpy(&m_parent->m_globalScale, &m_globalScale, sizeof(glm::vec3));
        m_globalScale *= m_localScale;
        if(m_geometry) m_boundSphereRaduis = m_geometry->GetBoundSphereRadius()*glm::compMax(m_globalScale);
    }
}

void ROC::Model::SetParent(Model *f_model, int f_bone)
{
    m_parent = f_model;
    m_parentBone = f_bone;
    m_rebuildMatrix = true;
}

void ROC::Model::SetCollision(Collision *f_col)
{
    if(m_skeleton)
    {
        btRigidBody *l_body = (f_col ? f_col->GetRigidBody() : m_collision->GetRigidBody());
        bool l_collisionIgnoring = (f_col != nullptr);
        if(m_skeleton->HasStaticBoneCollision())
        {
            for(auto iter : m_skeleton->GetCollision())
            {
                l_body->setIgnoreCollisionCheck(iter->m_rigidBody, l_collisionIgnoring);
                iter->m_rigidBody->setIgnoreCollisionCheck(l_body, l_collisionIgnoring);
            }
        }
        if(m_skeleton->HasDynamicBoneCollision())
        {
            for(auto iter : m_skeleton->GetJoints())
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

void ROC::Model::Update(int f_state, bool f_arg1)
{
    switch(f_state)
    {
        case ROC_MODEL_UPDATE_MATRIX:
        {
            m_rebuilded = false;
            if(m_rebuildMatrix)
            {
                btTransform l_transform = btTransform::getIdentity();
                l_transform.setOrigin(btVector3(m_localPosition.x, m_localPosition.y, m_localPosition.z));
                l_transform.setRotation(btQuaternion(m_localRotation.x, m_localRotation.y, m_localRotation.z, m_localRotation.w));
                l_transform.getOpenGLMatrix(glm::value_ptr(m_localMatrix));
                if(m_useScale) m_localMatrix *= glm::scale(g_IdentityMatrix, m_localScale);

                m_rebuildMatrix = false;
                m_rebuilded = true;
            }
            if(m_parent)
            {
                if(m_parentBone != -1)
                {
                    if(m_parent->m_skeleton->GetBones()[m_parentBone]->IsRebuilded() || m_parent->m_rebuilded)
                    {
                        const glm::mat4 &l_boneMatrix = m_parent->m_skeleton->GetBones()[m_parentBone]->GetMatrix();
                        std::memcpy(&m_globalMatrix, &m_parent->m_globalMatrix, sizeof(glm::mat4));
                        m_globalMatrix *= l_boneMatrix;
                        m_globalMatrix *= m_localMatrix;
                        UpdateGlobalTransform();
                        m_rebuilded = true;
                    }
                }
                else
                {
                    if(m_parent->m_rebuilded)
                    {
                        std::memcpy(&m_globalMatrix, &m_parent->m_globalMatrix, sizeof(glm::mat4));
                        m_globalMatrix *= m_localMatrix;
                        UpdateGlobalTransform();
                        m_rebuilded = true;
                    }
                }
            }
            else
            {
                if(m_rebuilded) std::memcpy(&m_globalMatrix, &m_localMatrix, sizeof(glm::mat4));
            }
        } break;

        case ROC_MODEL_UPDATE_COLLISION:
        {
            if(m_collision)
            {
                m_rebuilded = m_collision->IsActive();
                if(m_rebuilded)
                {
                    m_collision->GetTransform(m_localMatrix, m_localPosition, m_localRotation);
                    if(m_useScale) m_localMatrix *= glm::scale(g_IdentityMatrix, m_localScale);
                    std::memcpy(&m_globalPosition, &m_localPosition, sizeof(glm::vec3));
                    std::memcpy(&m_globalRotation, &m_localRotation, sizeof(glm::quat));
                    std::memcpy(&m_globalMatrix, &m_localMatrix, sizeof(glm::mat4));
                }
            }
        } break;

        case ROC_MODEL_UPDATE_SKELETON1:
        {
            if(m_skeleton)
            {
                m_animController->Update(m_skeleton->GetBones());
                m_skeleton->UpdateCollision(ROC_SKELETON_UPDATE_COLLISION1, m_globalMatrix, f_arg1);
                m_skeleton->Update();
            }
        } break;

        case ROC_MODEL_UPDATE_SKELETON2:
        {
            if(m_skeleton) m_skeleton->UpdateCollision(ROC_SKELETON_UPDATE_COLLISION2, m_globalMatrix, f_arg1);
        } break;
    }
}
