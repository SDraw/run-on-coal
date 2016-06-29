#include "stdafx.h"
#include "Model/Bone.h"
#include "Model/Geometry.h"
#include "Model/Skeleton.h"

ROC::Skeleton::Skeleton(std::vector<Geometry::geometryBoneData> &f_data)
{
    for(auto iter : f_data)
    {
        Bone *l_bone = new Bone(iter.m_name,iter.m_rotation,iter.m_position,iter.m_scale);
        m_boneVector.push_back(l_bone);
    }
    for(size_t i=0, j=f_data.size(); i < j; i++)
    {
        if(f_data[i].m_parent != -1)
        {
             m_boneVector[i]->SetParent(m_boneVector[f_data[i].m_parent]);
             m_boneVector[f_data[i].m_parent]->AddChild(m_boneVector[i]);
        }
    }
    m_boneVector[0]->GenerateBindPose();
    m_boneMatrices.resize(m_boneVector.size());
    m_rigid = false;
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
            for(auto iter1 : iter)
            {
                if(iter1.m_constraint) delete iter1.m_constraint;
            }
            for(auto iter1 : iter)
            {
                delete iter1.m_rigidBody->getMotionState();
                delete iter1.m_rigidBody;
            }
        }
    }
}

void ROC::Skeleton::Update(std::vector<float> &f_left, std::vector<float> &f_right, float f_lerp)
{
    skFastStoring l_leftData,l_rightData;

    for(size_t i=0, j=m_boneVector.size(); i < j; i++)
    {
        int l_bonePos = i*10;

        std::memcpy(&l_leftData,&f_left[l_bonePos],sizeof(skFastStoring));
        std::memcpy(&l_rightData,&f_right[l_bonePos],sizeof(skFastStoring));

        m_boneVector[i]->SetPosition(glm::lerp(l_leftData.m_pos,l_rightData.m_pos,f_lerp));
        m_boneVector[i]->SetRotation(glm::slerp(l_leftData.m_rot,l_rightData.m_rot,f_lerp));
        m_boneVector[i]->SetScale(glm::lerp(l_leftData.m_scale,l_rightData.m_scale,f_lerp));
    }
    m_boneVector[0]->UpdateMatrix();
    for(size_t i=0, j=m_boneVector.size(); i < j; i++) std::memcpy(&m_boneMatrices[i],&m_boneVector[i]->m_offsetMatrix,sizeof(glm::mat4));
}
void ROC::Skeleton::Update()
{
    m_boneVector[0]->UpdateMatrix();
    for(size_t i=0, j=m_boneVector.size(); i < j; i++) std::memcpy(&m_boneMatrices[i],&m_boneVector[i]->m_offsetMatrix,sizeof(glm::mat4));
}

void ROC::Skeleton::InitRigidity(std::vector<std::vector<Geometry::geometryChainData>> &f_vec)
{
    m_chainsVector.resize(f_vec.size());
    for(size_t i=0, ii=m_chainsVector.size(); i < ii; i++)
    {
        for(size_t j=0, jj=f_vec[i].size(); j < jj; j++)
        {
            skChain l_chain;
            l_chain.m_boneID = f_vec[i][j].m_boneID;
            btVector3 l_inertia;
            btSphereShape *l_shape = new btSphereShape(f_vec[i][j].m_size.x);
            l_shape->calculateLocalInertia(f_vec[i][j].m_mass,l_inertia);

            btTransform l_transform;
            l_transform.setFromOpenGLMatrix((float*)&m_boneVector[l_chain.m_boneID]->m_matrix);
            btDefaultMotionState *l_fallMotionState = new btDefaultMotionState(l_transform);

            btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(f_vec[i][j].m_mass,l_fallMotionState,l_shape,l_inertia);
            l_chain.m_rigidBody = new btRigidBody(fallRigidBodyCI);
            l_chain.m_rigidBody->setDamping(0.5f,0.5f);

            if(j > 0)
            {
                l_chain.m_rigidBody->setActivationState(DISABLE_DEACTIVATION);
                btVector3 l_pivotA(0.f,0.f,0.f);
                btTransform l_parentTransform = m_chainsVector[i][j-1].m_rigidBody->getWorldTransform();
                btTransform l_offsetTransform = l_parentTransform.inverse()*l_transform;
                l_chain.m_rigidBody->setIgnoreCollisionCheck(m_chainsVector[i][j-1].m_rigidBody,true);
                m_chainsVector[i][j-1].m_rigidBody->setIgnoreCollisionCheck(l_chain.m_rigidBody,true);
                l_chain.m_constraint = new btPoint2PointConstraint(*l_chain.m_rigidBody,*m_chainsVector[i][j-1].m_rigidBody,l_pivotA,l_offsetTransform.getOrigin());
            }
            else l_chain.m_constraint = NULL;
            m_chainsVector[i].push_back(l_chain);
        }
    }
    m_rigid = true;
}

void ROC::Skeleton::UpdateChains(glm::mat4 &f_model)
{
    if(!m_rigid) return;
    for(auto iter : m_chainsVector)
    {
        for(auto iter1 : iter)
        {
            btVector3 l_null(0.f,0.f,0.f);
            btRigidBody *l_body = iter1.m_rigidBody;
            glm::mat4 l_boneWorld = f_model*m_boneVector[iter1.m_boneID]->m_matrix;
            btTransform l_transform;
            l_transform.setFromOpenGLMatrix((float*)&l_boneWorld);
            btTransform l_bodyTransform = l_body->getCenterOfMassTransform();
            l_transform.setRotation(l_bodyTransform.getRotation());
            l_body->setCenterOfMassTransform(l_transform);
            l_body->setLinearVelocity(l_null);
            break;
        }
    }
}

void ROC::Skeleton::UpdateRigidBones(glm::mat4 &f_model)
{
    if(!m_rigid) return;
    glm::mat4 l_modelInv = glm::inverse(f_model);
    for(auto iter : m_chainsVector)
    {
        for(auto iter1 : iter)
        {
            Bone *l_bone = m_boneVector[iter1.m_boneID];
            btTransform l_transform = iter1.m_rigidBody->getCenterOfMassTransform();
            glm::mat4 l_bodyWorld(1.f);
            l_transform.getOpenGLMatrix((float*)&l_bodyWorld);
            l_bone->m_offsetMatrix = (l_modelInv*l_bodyWorld)*l_bone->m_bindMatrix;
            std::memcpy(&m_boneMatrices[iter1.m_boneID],&l_bone->m_offsetMatrix,sizeof(glm::mat4));
        }
    }
}
