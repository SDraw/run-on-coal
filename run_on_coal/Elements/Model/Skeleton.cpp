#include "stdafx.h"

#include "Elements/Model/Skeleton.h"

#include "Elements/Animation/BoneFrameData.h"
#include "Elements/Geometry/BoneCollisionData.hpp"
#include "Elements/Geometry/BoneData.hpp"
#include "Elements/Geometry/BoneJointData.hpp"
#include "Elements/Model/Bone.h"

#define ROC_BONECOL_TYPE_SPHERE 0U
#define ROC_BONECOL_TYPE_BOX 1U
#define ROC_BONECOL_TYPE_CYLINDER 2U
#define ROC_BONECOL_TYPE_CAPSULE 3U
#define ROC_BONECOL_TYPE_CONE 4U
#define ROC_SKELETON_TRANSFORMATION_MAIN 0
#define ROC_SKELETON_TRANSFORMATION_INVERSE 1
#define ROC_SKELETON_TRANSFORMATION_BIND 2

namespace ROC
{

extern const glm::mat4 g_IdentityMatrix;

}

ROC::Skeleton::Skeleton(const std::vector<BoneData*> &f_data)
{
    for(auto iter : f_data)
    {
        Bone *l_bone = new Bone(iter->m_name, iter->m_rotation, iter->m_position, iter->m_scale);
        m_boneVector.push_back(l_bone);
    }
    m_boneVector.shrink_to_fit();
    m_bonesCount = m_boneVector.size();
    for(size_t i = 0; i < m_bonesCount; i++)
    {
        if(f_data[i]->m_parent != -1)
        {
            size_t l_parent = static_cast<size_t>(f_data[i]->m_parent);
            m_boneVector[i]->SetParent(m_boneVector[l_parent]);
            m_boneVector[l_parent]->AddChild(m_boneVector[i]);
        }
    }
    if(!m_boneVector.empty())
    {
        std::vector<Bone*> l_bonesStack;
        l_bonesStack.push_back(m_boneVector.front());
        while(!l_bonesStack.empty())
        {
            Bone *l_bone = l_bonesStack.back();
            l_bonesStack.pop_back();
            l_bonesStack.insert(l_bonesStack.end(), l_bone->m_childBoneVector.rbegin(), l_bone->m_childBoneVector.rend());
            m_fastBoneVector.push_back(l_bone);
            l_bone->GenerateBindPose();
        }
    }
    m_poseMatrices.resize(m_bonesCount);
    m_poseMatrices.shrink_to_fit();

    m_hasStaticBoneCollision = false;
    m_hasDynamicBoneCollision = false;
}
ROC::Skeleton::~Skeleton()
{
    for(auto iter : m_boneVector) delete iter;
    m_boneVector.clear();
    m_poseMatrices.clear();

    if(m_hasStaticBoneCollision)
    {
        for(auto iter : m_collisionVector)
        {
            iter->m_offset.clear();
            delete iter->m_rigidBody->getMotionState();
            delete iter->m_rigidBody;
            delete iter;
        }
        m_collisionVector.clear();
    }

    if(m_hasDynamicBoneCollision)
    {
        for(auto iter : m_jointVector)
        {
            for(auto iter1 = iter->m_partsVector.rbegin(); iter1 != iter->m_partsVector.rend(); ++iter1)
            {
                skJoint::jtPart *l_jointPart = *iter1;
                l_jointPart->m_offset.clear();
                l_jointPart->m_constraint->getRigidBodyA().removeConstraintRef(l_jointPart->m_constraint);
                l_jointPart->m_constraint->getRigidBodyB().removeConstraintRef(l_jointPart->m_constraint);
                delete l_jointPart->m_constraint;
                delete l_jointPart->m_rigidBody->getMotionState();
                delete l_jointPart->m_rigidBody;
                delete l_jointPart;
            }
            iter->m_offsetMatrix.clear();
            iter->m_partsVector.clear();
            delete iter->m_emptyBody->getMotionState();
            delete iter->m_emptyBody;
            delete iter;
        }
        m_jointVector.clear();
    }
    m_fastBoneVector.clear();
}

void ROC::Skeleton::Update()
{
    for(auto iter : m_fastBoneVector) iter->Update();
    for(size_t i = 0; i < m_bonesCount; i++) std::memcpy(&m_poseMatrices[i], &m_boneVector[i]->GetPoseMatrix(), sizeof(glm::mat4));
}

void ROC::Skeleton::InitStaticBoneCollision(const std::vector<BoneCollisionData*> &f_vec, void *f_model)
{
    if(!m_hasStaticBoneCollision)
    {
        for(auto iter : f_vec)
        {
            skCollision *l_colData = new skCollision();

            btCollisionShape *l_shape = nullptr;
            switch(iter->m_type)
            {
                case ROC_BONECOL_TYPE_SPHERE:
                    l_shape = new btSphereShape(iter->m_size.x);
                    break;
                case ROC_BONECOL_TYPE_BOX:
                    l_shape = new btBoxShape(btVector3(iter->m_size.x, iter->m_size.y, iter->m_size.z));
                    break;
                case ROC_BONECOL_TYPE_CYLINDER:
                    l_shape = new btCylinderShape(btVector3(iter->m_size.x, iter->m_size.y, iter->m_size.z));
                    break;
                case ROC_BONECOL_TYPE_CAPSULE:
                    l_shape = new btCapsuleShape(iter->m_size.x, iter->m_size.y);
                    break;
                case ROC_BONECOL_TYPE_CONE:
                    l_shape = new btConeShape(iter->m_size.x, iter->m_size.y);
                    break;
                default:
                    l_shape = new btEmptyShape();
            }

            btTransform l_boneTransform, l_bodyOffset = btTransform::getIdentity(), l_bodyTransform;
            l_boneTransform.setFromOpenGLMatrix(glm::value_ptr(m_boneVector[iter->m_boneID]->GetMatrix()));

            l_bodyOffset.setOrigin(btVector3(iter->m_offset.x, iter->m_offset.y, iter->m_offset.z));
            l_bodyOffset.setRotation(btQuaternion(iter->m_offsetRotation.x, iter->m_offsetRotation.y, iter->m_offsetRotation.z, iter->m_offsetRotation.w));
            l_colData->m_offset.push_back(l_bodyOffset);

            l_bodyTransform.mult(l_boneTransform, l_bodyOffset);
            btDefaultMotionState *l_fallMotionState = new btDefaultMotionState(l_bodyTransform);
            btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(0.f, l_fallMotionState, l_shape);
            l_colData->m_rigidBody = new btRigidBody(fallRigidBodyCI);
            l_colData->m_rigidBody->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
            l_colData->m_rigidBody->setActivationState(DISABLE_DEACTIVATION);
            l_colData->m_rigidBody->setUserPointer(f_model);

            l_colData->m_boneID = static_cast<int>(iter->m_boneID);

            m_collisionVector.push_back(l_colData);
        }
        if(!m_collisionVector.empty())
        {
            m_hasStaticBoneCollision = true;
            m_collisionVector.shrink_to_fit();
        }
    }
}

void ROC::Skeleton::InitDynamicBoneCollision(const std::vector<BoneJointData*> &f_vec, void *f_model)
{
    if(!m_hasDynamicBoneCollision)
    {
        for(auto iter : f_vec)
        {
            skJoint *l_joint = new skJoint();
            l_joint->m_boneID = static_cast<size_t>(iter->m_boneID);
            l_joint->m_offsetMatrix.push_back(btTransform());
            l_joint->m_offsetMatrix[ROC_SKELETON_TRANSFORMATION_MAIN].setFromOpenGLMatrix(glm::value_ptr(m_boneVector[l_joint->m_boneID]->GetLocalMatrix()));

            btTransform l_boneTransform;
            l_boneTransform.setFromOpenGLMatrix(glm::value_ptr(m_boneVector[l_joint->m_boneID]->GetMatrix()));

            btCollisionShape *l_jointShape = new btEmptyShape();
            btDefaultMotionState *l_jointFallMotionState = new btDefaultMotionState(l_boneTransform);
            btRigidBody::btRigidBodyConstructionInfo l_jointFallRigidBodyCI(0.f, l_jointFallMotionState, l_jointShape);
            l_joint->m_emptyBody = new btRigidBody(l_jointFallRigidBodyCI);
            l_joint->m_emptyBody->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
            l_joint->m_emptyBody->setActivationState(DISABLE_DEACTIVATION);
            l_joint->m_emptyBody->setUserPointer(f_model);
            m_jointVector.push_back(l_joint);

            for(size_t i = 0, j = iter->m_jointPartVector.size(); i < j; i++)
            {
                BoneJointPartData &l_partData = iter->m_jointPartVector[i];
                skJoint::jtPart *l_jointPart = new skJoint::jtPart();
                l_jointPart->m_boneID = static_cast<size_t>(l_partData.m_boneID);

                btTransform l_jointPartTransform = btTransform::getIdentity(), l_jointPartResultTransform;

                l_boneTransform.setFromOpenGLMatrix(glm::value_ptr(m_boneVector[l_jointPart->m_boneID]->GetMatrix()));
                l_jointPartTransform.setOrigin(btVector3(l_partData.m_offset.x, l_partData.m_offset.y, l_partData.m_offset.z));
                l_jointPartTransform.setRotation(btQuaternion(l_partData.m_rotation.x, l_partData.m_rotation.y, l_partData.m_rotation.z, l_partData.m_rotation.w));

                l_jointPart->m_offset.push_back(l_jointPartTransform);
                l_jointPart->m_offset.push_back(l_jointPartTransform.inverse());
                l_jointPart->m_offset.push_back(btTransform());
                l_jointPart->m_offset[ROC_SKELETON_TRANSFORMATION_BIND].setFromOpenGLMatrix(glm::value_ptr(m_boneVector[l_jointPart->m_boneID]->GetBindMatrix()));

                l_jointPartResultTransform.mult(l_boneTransform, l_jointPartTransform);

                btCollisionShape *l_jointPartShape = nullptr;
                btVector3 l_jointPartInertia;
                switch(l_partData.m_type)
                {
                    case ROC_BONECOL_TYPE_SPHERE:
                        l_jointPartShape = new btSphereShape(l_partData.m_size.x);
                        break;
                    case ROC_BONECOL_TYPE_BOX:
                        l_jointPartShape = new btBoxShape(btVector3(l_partData.m_size.x, l_partData.m_size.y, l_partData.m_size.z));
                        break;
                    case ROC_BONECOL_TYPE_CYLINDER:
                        l_jointPartShape = new btCylinderShape(btVector3(l_partData.m_size.x, l_partData.m_size.y, l_partData.m_size.z));
                        break;
                    case ROC_BONECOL_TYPE_CAPSULE:
                        l_jointPartShape = new btCapsuleShape(l_partData.m_size.x, l_partData.m_size.y);
                        break;
                    case ROC_BONECOL_TYPE_CONE:
                        l_jointPartShape = new btConeShape(l_partData.m_size.x, l_partData.m_size.y);
                        break;
                    default:
                        l_jointPartShape = new btEmptyShape();
                }
                l_jointPartShape->calculateLocalInertia(l_partData.m_mass, l_jointPartInertia);
                btDefaultMotionState *l_jointPartFallMotionState = new btDefaultMotionState(l_jointPartResultTransform);
                btRigidBody::btRigidBodyConstructionInfo l_jointPartFallRigidBodyCI(l_partData.m_mass, l_jointPartFallMotionState, l_jointPartShape, l_jointPartInertia);
                l_jointPart->m_rigidBody = new btRigidBody(l_jointPartFallRigidBodyCI);
                l_jointPart->m_rigidBody->setActivationState(DISABLE_DEACTIVATION);
                l_jointPart->m_rigidBody->setUserPointer(f_model);

                l_jointPart->m_rigidBody->setRestitution(l_partData.m_restutition);
                l_jointPart->m_rigidBody->setFriction(l_partData.m_friction);
                l_jointPart->m_rigidBody->setDamping(l_partData.m_damping.x, l_partData.m_damping.y);

                if(i == 0)
                {
                    // First joint part is connected to joint empty body
                    btTransform l_jointConstraintOffset = btTransform::getIdentity();
                    l_jointPart->m_constraint = new btGeneric6DofSpringConstraint(*l_joint->m_emptyBody, *l_jointPart->m_rigidBody, l_jointConstraintOffset, l_jointPart->m_offset[ROC_SKELETON_TRANSFORMATION_INVERSE], false);
                }
                else
                {
                    // Joint part is connected to previous joint part body
                    btRigidBody *l_prevJointRigidBody = l_joint->m_partsVector.back()->m_rigidBody;
                    btTransform l_prevJointPartToBoneTransform;
                    l_prevJointPartToBoneTransform.mult(l_prevJointRigidBody->getCenterOfMassTransform().inverse(), l_boneTransform);

                    l_jointPart->m_constraint = new btGeneric6DofSpringConstraint(*l_prevJointRigidBody, *l_jointPart->m_rigidBody, l_prevJointPartToBoneTransform, l_jointPart->m_offset[ROC_SKELETON_TRANSFORMATION_INVERSE], false);
                }

                l_jointPart->m_constraint->setLinearLowerLimit(btVector3(l_partData.m_lowerLinearLimit.x, l_partData.m_lowerLinearLimit.y, l_partData.m_lowerLinearLimit.z));
                l_jointPart->m_constraint->setLinearUpperLimit(btVector3(l_partData.m_upperLinearLimit.x, l_partData.m_upperLinearLimit.y, l_partData.m_upperLinearLimit.z));
                l_jointPart->m_constraint->setAngularLowerLimit(btVector3(l_partData.m_lowerAngularLimit.x, l_partData.m_lowerAngularLimit.y, l_partData.m_lowerAngularLimit.z));
                l_jointPart->m_constraint->setAngularUpperLimit(btVector3(l_partData.m_upperAngularLimit.x, l_partData.m_upperAngularLimit.y, l_partData.m_upperAngularLimit.z));
                for(int k = 0; k < 3; k++)
                {
                    if(l_partData.m_linearStiffness[k] > 0.f)
                    {
                        l_jointPart->m_constraint->enableSpring(k, true);
                        l_jointPart->m_constraint->setStiffness(k, l_partData.m_linearStiffness[k]);
                    }
                    if(l_partData.m_angularStiffness[k] > 0.f)
                    {
                        l_jointPart->m_constraint->enableSpring(k + 3, true);
                        l_jointPart->m_constraint->setStiffness(k + 3, l_partData.m_angularStiffness[k]);
                    }
                }

                l_joint->m_partsVector.push_back(l_jointPart);
            }
            l_joint->m_partsVector.shrink_to_fit();
        }

        if(!m_jointVector.empty())
        {
            m_jointVector.shrink_to_fit();
            m_hasDynamicBoneCollision = true;
        }
    }
}

void ROC::Skeleton::UpdateCollision(SkeletonUpdateStage f_stage, const glm::mat4 &f_model, bool f_enabled)
{
    switch(f_stage)
    {
        case SUS_Static:
        {
            if(m_hasStaticBoneCollision || m_hasDynamicBoneCollision)
            {
                btTransform l_model;
                btTransform l_transform1, l_transform2;
                l_model.setFromOpenGLMatrix(glm::value_ptr(f_model));

                if(m_hasStaticBoneCollision)
                {
                    for(auto iter : m_collisionVector)
                    {
                        // BodyGlobal = Model * (Bone * BodyBoneOffset)
                        l_transform1.setFromOpenGLMatrix(glm::value_ptr(m_boneVector[iter->m_boneID]->GetMatrix()));
                        l_transform2.mult(l_transform1, iter->m_offset[ROC_SKELETON_TRANSFORMATION_MAIN]);
                        l_transform1.mult(l_model, l_transform2);
                        f_enabled ? iter->m_rigidBody->getMotionState()->setWorldTransform(l_transform1) : iter->m_rigidBody->setCenterOfMassTransform(l_transform1);
                    }
                }

                if(m_hasDynamicBoneCollision)
                {
                    for(auto iter : m_jointVector)
                    {
                        if(m_boneVector[iter->m_boneID]->HasParent())
                        {
                            // BodyGlobal = Model * (ParentBone * BodyBoneOffset)
                            l_transform1.setFromOpenGLMatrix(glm::value_ptr(m_boneVector[iter->m_boneID]->GetParent()->GetMatrix()));
                            l_transform2.mult(l_transform1, iter->m_offsetMatrix[ROC_SKELETON_TRANSFORMATION_MAIN]);
                            l_transform1.mult(l_model, l_transform2);

                            f_enabled ? iter->m_emptyBody->getMotionState()->setWorldTransform(l_transform1) : iter->m_emptyBody->setCenterOfMassTransform(l_transform1);
                        }
                        else
                        {
                            // BodyGlobal = Model * BodyBoneOffset
                            l_transform1.mult(l_model, iter->m_offsetMatrix[ROC_SKELETON_TRANSFORMATION_MAIN]);

                            f_enabled ? iter->m_emptyBody->getMotionState()->setWorldTransform(l_transform1) : iter->m_emptyBody->setCenterOfMassTransform(l_transform1);
                        }
                    }
                }
            }
        } break;

        case SUS_Dynamic:
        {
            if(m_hasDynamicBoneCollision)
            {
                btTransform l_model;
                btTransform l_transform1, l_transform2;
                l_model.setFromOpenGLMatrix(glm::value_ptr(f_model));
                if(f_enabled)
                {
                    btTransform l_modelInv = l_model.inverse();
                    for(auto iter : m_jointVector)
                    {
                        for(auto iter1 : iter->m_partsVector)
                        {
                            // Pose = (ModelInverse * (BodyGlobal * BodyBoneOffsetInverse)) * BoneBind
                            Bone *l_bone = m_boneVector[iter1->m_boneID];
                            l_transform1.mult(iter1->m_rigidBody->getCenterOfMassTransform(), iter1->m_offset[ROC_SKELETON_TRANSFORMATION_INVERSE]);
                            l_transform2.mult(l_modelInv, l_transform1);
                            l_bone->SetMatrix(l_transform2);
                            l_transform1.mult(l_transform2, iter1->m_offset[ROC_SKELETON_TRANSFORMATION_BIND]);

                            l_bone->SetPoseMatrix(l_transform1);
                            std::memcpy(&m_poseMatrices[iter1->m_boneID], &l_bone->GetPoseMatrix(), sizeof(glm::mat4));
                        }
                    }
                }
                else
                {
                    for(auto iter : m_jointVector)
                    {
                        for(auto iter1 : iter->m_partsVector)
                        {
                            // BodyGlobal = Model * (BoneMatrix * BodyBoneOffset)
                            l_transform1.setFromOpenGLMatrix(glm::value_ptr(m_boneVector[iter1->m_boneID]->GetMatrix()));
                            l_transform2.mult(l_transform1, iter1->m_offset[ROC_SKELETON_TRANSFORMATION_MAIN]);
                            l_transform1.mult(l_model, l_transform2);

                            iter1->m_rigidBody->setCenterOfMassTransform(l_transform1);
                        }
                    }
                }
            }
        } break;
    }
}
