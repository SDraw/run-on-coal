#include "stdafx.h"
#include "Model/Bone.h"
#include "Model/BoneData.h"
#include "Model/BoneChainGroup.h"
#include "Model/Skeleton.h"

ROC::Skeleton::Skeleton(std::vector<BoneData*> &f_data)
{
    for(auto iter : f_data)
    {
        Bone *l_bone = new Bone(iter->m_name, iter->m_rotation, iter->m_position, iter->m_scale);
        m_boneVector.push_back(l_bone);
    }
    m_bonesCount = m_boneVector.size();
    for(size_t i = 0, j = f_data.size(); i < j; i++)
    {
        if(f_data[i]->m_parent != -1)
        {
            m_boneVector[i]->SetParent(m_boneVector[f_data[i]->m_parent]);
            m_boneVector[f_data[i]->m_parent]->AddChild(m_boneVector[i]);
        }
    }
    if(!m_boneVector.empty())
    {
        std::list<Bone*> l_bonesStack;
        l_bonesStack.push_back(m_boneVector[0]);
        while(!l_bonesStack.empty())
        {
            Bone *l_bone = l_bonesStack.back();
            l_bonesStack.pop_back();
            l_bonesStack.insert(l_bonesStack.end(), l_bone->m_childBoneVector.rbegin(), l_bone->m_childBoneVector.rend());
            m_fastBoneVector.push_back(l_bone);
            l_bone->GenerateBindPose();
        }
    }
    m_boneMatrices.resize(m_bonesCount);
    m_rigid = false;
    m_jointsCount = 0U;
}
ROC::Skeleton::~Skeleton()
{
    for(auto iter : m_boneVector) delete iter;
    m_boneVector.clear();
    m_boneMatrices.clear();
    if(m_rigid)
    {
        for(auto iter : m_chainsVector)
        {
            for(auto iter1 : iter) delete iter1.m_constraint;
            for(auto iter1 : iter)
            {
                delete iter1.m_rigidBody->getMotionState();
                delete iter1.m_rigidBody;
            }
        }
        for(auto iter : m_jointVector)
        {
            delete iter->getMotionState();
            delete iter;
        }
    }
    m_fastBoneVector.clear();
}

void ROC::Skeleton::Update(std::vector<float> &f_left, std::vector<float> &f_right, float f_lerp)
{
    for(size_t i = 0, l_bonePos = 0; i < m_bonesCount; i++, l_bonePos += 10)
    {
        std::memcpy(&m_leftData, &f_left[l_bonePos], sizeof(skFastStoring));
        std::memcpy(&m_rightData, &f_right[l_bonePos], sizeof(skFastStoring));

        m_interpolated.m_pos = glm::lerp(m_leftData.m_pos, m_rightData.m_pos, f_lerp);
        m_interpolated.m_rot = glm::slerp(m_leftData.m_rot, m_rightData.m_rot, f_lerp);
        m_interpolated.m_scale = glm::lerp(m_leftData.m_scale, m_rightData.m_scale, f_lerp);
        m_boneVector[i]->SetData(&m_interpolated);
    }
    Update();
}
void ROC::Skeleton::Update()
{
    for(auto iter : m_fastBoneVector) iter->UpdateMatrix();
    for(size_t i = 0; i < m_bonesCount; i++) std::memcpy(&m_boneMatrices[i], &m_boneVector[i]->m_offsetMatrix, sizeof(glm::mat4));
}
void ROC::Skeleton::ResetBonesInterpolation()
{
    for(auto iter : m_boneVector)
    {
        iter->m_interpolation = true;
        iter->m_interpolationValue = 0.f;
    }
}

void ROC::Skeleton::InitRigidity(std::vector<BoneChainGroup*> &f_vec)
{
    if(m_rigid) return;
    m_chainsVector.resize(f_vec.size());
    for(size_t i = 0, ii = m_chainsVector.size(); i < ii; i++)
    {
        for(size_t j = 0, jj = f_vec[i]->m_boneChainDataVector.size(); j < jj; j++)
        {
            BoneChainGroup::BoneChainData *l_chainData = f_vec[i]->m_boneChainDataVector[j];
            skChain l_skChain;
            l_skChain.m_boneID = l_chainData->m_boneID;
            btVector3 l_inertia;
            btSphereShape *l_shape = new btSphereShape(l_chainData->m_size.x);
            l_shape->calculateLocalInertia(l_chainData->m_mass, l_inertia);

            btTransform l_transform;
            l_transform.setFromOpenGLMatrix((float*)&m_boneVector[l_skChain.m_boneID]->m_matrix);
            btDefaultMotionState *l_fallMotionState = new btDefaultMotionState(l_transform);

            btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(l_chainData->m_mass, l_fallMotionState, l_shape, l_inertia);
            l_skChain.m_rigidBody = new btRigidBody(fallRigidBodyCI);
            l_skChain.m_rigidBody->setDamping(0.75f, 0.75f);

            if(j > 0)
            {
                l_skChain.m_rigidBody->setActivationState(DISABLE_DEACTIVATION);
                btVector3 l_pivotA(0.f, 0.f, 0.f);
                btTransform l_parentTransform = m_chainsVector[i][j - 1].m_rigidBody->getWorldTransform();
                btTransform l_offsetTransform = l_parentTransform.inverse()*l_transform;
                l_skChain.m_rigidBody->setIgnoreCollisionCheck(m_chainsVector[i][j - 1].m_rigidBody, true);
                m_chainsVector[i][j - 1].m_rigidBody->setIgnoreCollisionCheck(l_skChain.m_rigidBody, true);
                l_skChain.m_constraint = new btPoint2PointConstraint(*l_skChain.m_rigidBody, *m_chainsVector[i][j - 1].m_rigidBody, l_pivotA, l_offsetTransform.getOrigin());
            }
            else
            {
                btVector3 l_pivotA(0.f, 0.f, 0.f);
                btVector3 l_jointInertia;
                btSphereShape *l_jointShape = new btSphereShape(0.01f);
                l_shape->calculateLocalInertia(0.f, l_jointInertia);
                btDefaultMotionState *l_jointFallMotionState = new btDefaultMotionState(l_transform);
                btRigidBody::btRigidBodyConstructionInfo l_jointFallRigidBodyCI(0.f, l_jointFallMotionState, l_jointShape, l_jointInertia);
                btRigidBody *l_joint = new btRigidBody(l_jointFallRigidBodyCI);
                l_joint->setDamping(0.75f, 0.75f);

                l_skChain.m_rigidBody->setIgnoreCollisionCheck(l_joint, true);
                l_joint->setIgnoreCollisionCheck(l_skChain.m_rigidBody, true);

                l_skChain.m_constraint = new btPoint2PointConstraint(*l_skChain.m_rigidBody, *l_joint, l_pivotA, l_pivotA);
                m_jointVector.push_back(l_joint);
            }
            m_chainsVector[i].push_back(l_skChain);
        }
    }
    m_jointsCount = m_jointVector.size();
    m_rigid = true;
}

void ROC::Skeleton::UpdateJoints(glm::mat4 &f_model, bool f_enabled)
{
    if(!m_rigid) return;
    btTransform l_transform, l_model, l_bone;
    l_model.setFromOpenGLMatrix((float*)&f_model);

    for(size_t i = 0; i < m_jointsCount; i++)
    {
        skChain &l_chain = m_chainsVector[i][0];
        btRigidBody *l_joint = m_jointVector[i];
        l_bone.setFromOpenGLMatrix((float*)&m_boneVector[l_chain.m_boneID]->m_matrix);
        l_transform.mult(l_model, l_bone);
        if(f_enabled) l_transform.setRotation(l_joint->getCenterOfMassTransform().getRotation());
        l_joint->setCenterOfMassTransform(l_transform);
    }
}

void ROC::Skeleton::UpdateRigidBones(glm::mat4 &f_model, bool f_enabled)
{
    if(!m_rigid) return;
    btTransform l_model;
    l_model.setFromOpenGLMatrix((float*)&f_model);
    if(f_enabled)
    {
        btTransform l_modelInv = l_model.inverse();
        btTransform l_boneBind, l_bodyLocal, l_result;
        for(auto iter : m_chainsVector)
        {
            for(auto iter1 : iter)
            {
                Bone *l_bone = m_boneVector[iter1.m_boneID];
                const btTransform &l_bodyTransform = iter1.m_rigidBody->getCenterOfMassTransform();
                l_boneBind.setFromOpenGLMatrix((float*)&l_bone->m_bindMatrix);
                l_bodyLocal.mult(l_modelInv, l_bodyTransform);
                l_result.mult(l_bodyLocal, l_boneBind);
                l_result.getOpenGLMatrix((float*)&l_bone->m_offsetMatrix);
                std::memcpy(&m_boneMatrices[iter1.m_boneID], &l_bone->m_offsetMatrix, sizeof(glm::mat4));
            }
        }
    }
    else
    {
        btTransform l_transform, l_bone;
        for(auto iter : m_chainsVector)
        {
            for(auto iter1 : iter)
            {
                l_bone.setFromOpenGLMatrix((float*)&m_boneVector[iter1.m_boneID]->m_matrix);
                l_transform.mult(l_model, l_bone);
                iter1.m_rigidBody->setCenterOfMassTransform(l_transform);
            }
        }
    }
}