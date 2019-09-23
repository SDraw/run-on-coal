#include "stdafx.h"

#include "Elements/Model/Skeleton.h"
#include "Elements/Model/Bone.h"
#include "Elements/Geometry/BoneCollisionData.hpp"
#include "Elements/Geometry/BoneData.hpp"
#include "Elements/Geometry/BoneJointData.hpp"

#include "Elements/Animation/Animation.h"
#include "Elements/Animation/BoneFrameData.h"
#include "Utils/Transformation.h"

namespace ROC
{

extern const glm::mat4 g_IdentityMatrix;

}

bool ROC::Skeleton::ms_physicsEnabled = false;

ROC::Skeleton::Skeleton(const std::vector<BoneData*> &f_data)
{
    for(auto l_boneData : f_data)
    {
        Bone *l_bone = new Bone(l_boneData->m_name, l_boneData->m_rotation, l_boneData->m_position, l_boneData->m_scale);
        m_bones.push_back(l_bone);
    }
    m_bones.shrink_to_fit();
    m_bonesCount = m_bones.size();
    for(size_t i = 0; i < m_bonesCount; i++)
    {
        if(f_data[i]->m_parent != -1)
        {
            size_t l_parent = static_cast<size_t>(f_data[i]->m_parent);
            m_bones[i]->SetParent(m_bones[l_parent]);
            m_bones[l_parent]->AddChild(m_bones[i]);
        }
    }
    if(!m_bones.empty())
    {
        std::vector<Bone*> l_bonesStack;
        l_bonesStack.push_back(m_bones.front());
        while(!l_bonesStack.empty())
        {
            Bone *l_bone = l_bonesStack.back();
            l_bonesStack.pop_back();

            auto &l_boneChildren = l_bone->GetChildren();
            l_bonesStack.insert(l_bonesStack.end(), l_boneChildren.rbegin(), l_boneChildren.rend());
            m_sortedBones.push_back(l_bone);
            l_bone->GenerateBindPose();
        }
        m_sortedBones.shrink_to_fit();
    }
    m_poseMatrices.assign(m_bonesCount, g_IdentityMatrix);
    m_poseMatrices.shrink_to_fit();

    m_hasStaticBoneCollision = false;
    m_hasDynamicBoneCollision = false;
}
ROC::Skeleton::~Skeleton()
{
    for(auto l_bone : m_bones) delete l_bone;
    m_bones.clear();
    m_poseMatrices.clear();

    if(m_hasStaticBoneCollision)
    {
        for(auto l_col : m_collisions)
        {
            l_col->m_offset.clear();
            delete l_col->m_rigidBody->getMotionState();
            delete l_col->m_rigidBody;
            delete l_col;
        }
        m_collisions.clear();
    }

    if(m_hasDynamicBoneCollision)
    {
        for(auto l_joint : m_joints)
        {
            for(auto l_part = l_joint->m_parts.rbegin(); l_part != l_joint->m_parts.rend(); ++l_part)
            {
                SkeletonJointPart *l_jointPart = *l_part;
                l_jointPart->m_offset.clear();
                l_jointPart->m_constraint->getRigidBodyA().removeConstraintRef(l_jointPart->m_constraint);
                l_jointPart->m_constraint->getRigidBodyB().removeConstraintRef(l_jointPart->m_constraint);
                delete l_jointPart->m_constraint;
                delete l_jointPart->m_rigidBody->getMotionState();
                delete l_jointPart->m_rigidBody;
                delete l_jointPart;
            }
            l_joint->m_transform.clear();
            l_joint->m_parts.clear();
            delete l_joint->m_emptyBody->getMotionState();
            delete l_joint->m_emptyBody;
            delete l_joint;
        }
        m_joints.clear();
    }
    m_sortedBones.clear();
}

void ROC::Skeleton::InitStaticBoneCollision(const std::vector<BoneCollisionData*> &f_vec, void *f_model)
{
    if(!m_hasStaticBoneCollision)
    {
        for(const auto l_colData : f_vec)
        {
            if(l_colData->m_boneID >= m_bonesCount) continue; // Invalid bone id

            SkeletonCollision *l_skeletonCol = new SkeletonCollision();

            btCollisionShape *l_shape = nullptr;
            switch(l_colData->m_type)
            {
                case SCT_Sphere:
                    l_shape = new btSphereShape(l_colData->m_size.x);
                    break;
                case SCT_Box:
                    l_shape = new btBoxShape(btVector3(l_colData->m_size.x, l_colData->m_size.y, l_colData->m_size.z));
                    break;
                case SCT_Cylinder:
                    l_shape = new btCylinderShape(btVector3(l_colData->m_size.x, l_colData->m_size.y, l_colData->m_size.z));
                    break;
                case SCT_Capsule:
                    l_shape = new btCapsuleShape(l_colData->m_size.x, l_colData->m_size.y);
                    break;
                case SCT_Cone:
                    l_shape = new btConeShape(l_colData->m_size.x, l_colData->m_size.y);
                    break;
                default:
                    l_shape = new btEmptyShape();
                    break;
            }

            btTransform l_boneTransform, l_bodyOffset = btTransform::getIdentity(), l_bodyTransform;
            l_boneTransform.setFromOpenGLMatrix(glm::value_ptr(m_bones[static_cast<size_t>(l_colData->m_boneID)]->GetFullMatrix()));

            l_bodyOffset.setOrigin(btVector3(l_colData->m_offset.x, l_colData->m_offset.y, l_colData->m_offset.z));
            l_bodyOffset.setRotation(btQuaternion(l_colData->m_offsetRotation.x, l_colData->m_offsetRotation.y, l_colData->m_offsetRotation.z, l_colData->m_offsetRotation.w));
            l_skeletonCol->m_offset.push_back(l_bodyOffset);

            l_bodyTransform.mult(l_boneTransform, l_bodyOffset);
            btDefaultMotionState *l_fallMotionState = new btDefaultMotionState(l_bodyTransform);
            btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(0.f, l_fallMotionState, l_shape);
            l_skeletonCol->m_rigidBody = new btRigidBody(fallRigidBodyCI);
            l_skeletonCol->m_rigidBody->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
            l_skeletonCol->m_rigidBody->setActivationState(DISABLE_DEACTIVATION);
            l_skeletonCol->m_rigidBody->setUserPointer(f_model);

            l_skeletonCol->m_boneID = static_cast<size_t>(l_colData->m_boneID);

            m_collisions.push_back(l_skeletonCol);
        }
        if(!m_collisions.empty())
        {
            m_hasStaticBoneCollision = true;
            m_collisions.shrink_to_fit();
        }
    }
}

void ROC::Skeleton::InitDynamicBoneCollision(const std::vector<BoneJointData*> &f_vec, void *f_model)
{
    if(!m_hasDynamicBoneCollision)
    {
        for(const auto l_jointData : f_vec)
        {
            if(l_jointData->m_boneID >= m_bonesCount) continue; // Invalid bone id

            SkeletonJoint *l_joint = new SkeletonJoint();
            l_joint->m_boneID = static_cast<size_t>(l_jointData->m_boneID);
            l_joint->m_transform.push_back(btTransform::getIdentity()); // Local bone transformation
            l_joint->m_transform[STT_Main].setFromOpenGLMatrix(glm::value_ptr(m_bones[l_joint->m_boneID]->GetLocalTransformation()->GetMatrix()));

            btTransform l_boneTransform;
            l_boneTransform.setFromOpenGLMatrix(glm::value_ptr(m_bones[l_joint->m_boneID]->GetFullMatrix()));

            btCollisionShape *l_jointShape = new btEmptyShape();
            btDefaultMotionState *l_jointFallMotionState = new btDefaultMotionState(l_boneTransform);
            btRigidBody::btRigidBodyConstructionInfo l_jointFallRigidBodyCI(0.f, l_jointFallMotionState, l_jointShape);
            l_joint->m_emptyBody = new btRigidBody(l_jointFallRigidBodyCI);
            l_joint->m_emptyBody->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
            l_joint->m_emptyBody->setActivationState(DISABLE_DEACTIVATION);
            l_joint->m_emptyBody->setUserPointer(f_model);
            m_joints.push_back(l_joint);

            for(size_t i = 0, j = l_jointData->m_jointParts.size(); i < j; i++)
            {
                const BoneJointPartData &l_jointPartData = l_jointData->m_jointParts[i];
                if(l_jointPartData.m_boneID >= m_bonesCount) continue; // Invalid bone id

                SkeletonJointPart *l_jointPart = new SkeletonJointPart();
                l_jointPart->m_boneID = static_cast<size_t>(l_jointPartData.m_boneID);

                btTransform l_jointPartTransform = btTransform::getIdentity();

                l_boneTransform.setFromOpenGLMatrix(glm::value_ptr(m_bones[l_jointPart->m_boneID]->GetFullMatrix()));
                l_jointPartTransform.setOrigin(btVector3(l_jointPartData.m_offset.x, l_jointPartData.m_offset.y, l_jointPartData.m_offset.z));
                l_jointPartTransform.setRotation(btQuaternion(l_jointPartData.m_rotation.x, l_jointPartData.m_rotation.y, l_jointPartData.m_rotation.z, l_jointPartData.m_rotation.w));

                l_jointPart->m_offset.push_back(l_jointPartTransform); // Bone offset transformation
                l_jointPart->m_offset.push_back(l_jointPartTransform.inverse()); // Bone offset inversed transformation
                l_jointPart->m_offset.push_back(btTransform::getIdentity()); // Bone bind matrix (inversed full bone matrix)
                l_jointPart->m_offset[STT_Bind].setFromOpenGLMatrix(glm::value_ptr(m_bones[l_jointPart->m_boneID]->GetBindMatrix()));

                btTransform l_jointPartResultTransform;
                l_jointPartResultTransform.mult(l_boneTransform, l_jointPartTransform);

                btCollisionShape *l_jointPartShape = nullptr;
                btVector3 l_jointPartInertia;
                switch(l_jointPartData.m_type)
                {
                    case SCT_Sphere:
                        l_jointPartShape = new btSphereShape(l_jointPartData.m_size.x);
                        break;
                    case SCT_Box:
                        l_jointPartShape = new btBoxShape(btVector3(l_jointPartData.m_size.x, l_jointPartData.m_size.y, l_jointPartData.m_size.z));
                        break;
                    case SCT_Cylinder:
                        l_jointPartShape = new btCylinderShape(btVector3(l_jointPartData.m_size.x, l_jointPartData.m_size.y, l_jointPartData.m_size.z));
                        break;
                    case SCT_Capsule:
                        l_jointPartShape = new btCapsuleShape(l_jointPartData.m_size.x, l_jointPartData.m_size.y);
                        break;
                    case SCT_Cone:
                        l_jointPartShape = new btConeShape(l_jointPartData.m_size.x, l_jointPartData.m_size.y);
                        break;
                    default:
                        l_jointPartShape = new btEmptyShape();
                        break;
                }
                l_jointPartShape->calculateLocalInertia(l_jointPartData.m_mass, l_jointPartInertia);
                btDefaultMotionState *l_jointPartFallMotionState = new btDefaultMotionState(l_jointPartResultTransform);
                btRigidBody::btRigidBodyConstructionInfo l_jointPartFallRigidBodyCI(l_jointPartData.m_mass, l_jointPartFallMotionState, l_jointPartShape, l_jointPartInertia);
                l_jointPart->m_rigidBody = new btRigidBody(l_jointPartFallRigidBodyCI);
                l_jointPart->m_rigidBody->setActivationState(DISABLE_DEACTIVATION);
                l_jointPart->m_rigidBody->setUserPointer(f_model);

                l_jointPart->m_rigidBody->setRestitution(l_jointPartData.m_restutition);
                l_jointPart->m_rigidBody->setFriction(l_jointPartData.m_friction);
                l_jointPart->m_rigidBody->setDamping(l_jointPartData.m_damping.x, l_jointPartData.m_damping.y);

                if(i == 0U)
                {
                    // First joint part is connected to joint empty body
                    btTransform l_jointConstraintOffset = btTransform::getIdentity();
                    l_jointPart->m_constraint = new btGeneric6DofSpringConstraint(*l_joint->m_emptyBody, *l_jointPart->m_rigidBody, l_jointConstraintOffset, l_jointPart->m_offset[STT_Inverse], false);
                }
                else
                {
                    // Joint part is connected to previous joint part body
                    btRigidBody *l_prevJointRigidBody = l_joint->m_parts.back()->m_rigidBody;
                    btTransform l_prevJointPartToBoneTransform;
                    l_prevJointPartToBoneTransform.mult(l_prevJointRigidBody->getCenterOfMassTransform().inverse(), l_boneTransform);

                    l_jointPart->m_constraint = new btGeneric6DofSpringConstraint(*l_prevJointRigidBody, *l_jointPart->m_rigidBody, l_prevJointPartToBoneTransform, l_jointPart->m_offset[STT_Inverse], false);
                }
                l_jointPart->m_constraint->setDbgDrawSize(0.5f);

                l_jointPart->m_constraint->setLinearLowerLimit(btVector3(l_jointPartData.m_lowerLinearLimit.x, l_jointPartData.m_lowerLinearLimit.y, l_jointPartData.m_lowerLinearLimit.z));
                l_jointPart->m_constraint->setLinearUpperLimit(btVector3(l_jointPartData.m_upperLinearLimit.x, l_jointPartData.m_upperLinearLimit.y, l_jointPartData.m_upperLinearLimit.z));
                l_jointPart->m_constraint->setAngularLowerLimit(btVector3(l_jointPartData.m_lowerAngularLimit.x, l_jointPartData.m_lowerAngularLimit.y, l_jointPartData.m_lowerAngularLimit.z));
                l_jointPart->m_constraint->setAngularUpperLimit(btVector3(l_jointPartData.m_upperAngularLimit.x, l_jointPartData.m_upperAngularLimit.y, l_jointPartData.m_upperAngularLimit.z));
                for(int k = 0; k < 3; k++)
                {
                    if(l_jointPartData.m_linearStiffness[k] > 0.f)
                    {
                        l_jointPart->m_constraint->enableSpring(k, true);
                        l_jointPart->m_constraint->setStiffness(k, l_jointPartData.m_linearStiffness[k]);
                    }
                    if(l_jointPartData.m_angularStiffness[k] > 0.f)
                    {
                        l_jointPart->m_constraint->enableSpring(k + 3, true);
                        l_jointPart->m_constraint->setStiffness(k + 3, l_jointPartData.m_angularStiffness[k]);
                    }
                }
                l_joint->m_parts.push_back(l_jointPart);

                // Set bone as dynamic
                m_bones[l_jointPart->m_boneID]->SetDynamic(true);
                m_bones[l_jointPart->m_boneID]->SetDynamicBody(l_jointPart->m_rigidBody);
            }
            l_joint->m_parts.shrink_to_fit();
        }

        if(!m_joints.empty())
        {
            m_joints.shrink_to_fit();
            m_hasDynamicBoneCollision = true;
        }
    }
}

void ROC::Skeleton::Update(Animation *f_anim, unsigned int f_tick, float f_blend)
{
    f_anim->GetData(f_tick, m_bones, f_blend);
    for(auto l_bone : m_sortedBones) l_bone->Update();
    for(size_t i = 0U; i < m_bonesCount; i++) std::memcpy(&m_poseMatrices[i], &m_bones[i]->GetPoseMatrix(), sizeof(glm::mat4));
}

void ROC::Skeleton::UpdateCollision(SkeletonUpdateStage f_stage, const glm::mat4 &f_model)
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
                    for(auto l_skeletonCol : m_collisions)
                    {
                        // BodyGlobal = Model * (Bone * BoneOffset)
                        l_transform1.setFromOpenGLMatrix(glm::value_ptr(m_bones[l_skeletonCol->m_boneID]->GetFullMatrix()));
                        l_transform2.mult(l_transform1, l_skeletonCol->m_offset[STT_Main]);
                        l_transform1.mult(l_model, l_transform2);
                        ms_physicsEnabled ? l_skeletonCol->m_rigidBody->getMotionState()->setWorldTransform(l_transform1) : l_skeletonCol->m_rigidBody->setCenterOfMassTransform(l_transform1);
                    }
                }

                if(m_hasDynamicBoneCollision)
                {
                    for(auto l_joint : m_joints)
                    {
                        if(m_bones[l_joint->m_boneID]->HasParent())
                        {
                            // BodyGlobal = Model * (ParentBoneFull * Joint)
                            l_transform1.setFromOpenGLMatrix(glm::value_ptr(m_bones[l_joint->m_boneID]->GetParent()->GetFullMatrix()));
                            l_transform2.mult(l_transform1, l_joint->m_transform[STT_Main]);
                            l_transform1.mult(l_model, l_transform2);
                        }
                        else
                        {
                            // BodyGlobal = Model * Joint
                            l_transform1.mult(l_model, l_joint->m_transform[STT_Main]);
                        }
                        ms_physicsEnabled ? l_joint->m_emptyBody->getMotionState()->setWorldTransform(l_transform1) : l_joint->m_emptyBody->setCenterOfMassTransform(l_transform1);
                    }
                }
            }
        } break;

        case SUS_Dynamic:
        {
            if(m_hasDynamicBoneCollision)
            {
                btTransform l_transform1, l_transform2;
                if(ms_physicsEnabled)
                {
                    // btTransform works bad with inversion of imported scaled matrices
                    btTransform l_modelInv = btTransform::getIdentity();
                    glm::mat4 l_invMat4 = glm::inverse(f_model);
                    l_modelInv.setFromOpenGLMatrix(glm::value_ptr(l_invMat4));

                    for(auto l_joint : m_joints)
                    {
                        for(auto l_jointPart : l_joint->m_parts)
                        {
                            // BoneFull = ((ModelInverse * BodyGlobal) * BodyBoneOffsetInverse)
                            Bone *l_bone = m_bones[l_jointPart->m_boneID];
                            l_transform1.mult(l_modelInv, l_jointPart->m_rigidBody->getWorldTransform());
                            l_transform2.mult(l_transform1, l_jointPart->m_offset[STT_Inverse]);
                            l_bone->SetFullMatrix(l_transform2);

                            // BonePose = BoneFull * BoneBind
                            l_transform1.mult(l_transform2, l_jointPart->m_offset[STT_Bind]);
                            l_bone->SetPoseMatrix(l_transform1);
                            std::memcpy(&m_poseMatrices[l_jointPart->m_boneID], &l_bone->GetPoseMatrix(), sizeof(glm::mat4));
                        }
                    }
                }
                else
                {
                    btTransform l_model;
                    l_model.setFromOpenGLMatrix(glm::value_ptr(f_model));
                    for(auto l_joint : m_joints)
                    {
                        for(auto l_jointPart : l_joint->m_parts)
                        {
                            // BodyGlobal = Model * (BoneMatrix * BodyBoneOffset)
                            l_transform1.setFromOpenGLMatrix(glm::value_ptr(m_bones[l_jointPart->m_boneID]->GetFullMatrix()));
                            l_transform2.mult(l_transform1, l_jointPart->m_offset[STT_Main]);
                            l_transform1.mult(l_model, l_transform2);

                            l_jointPart->m_rigidBody->setCenterOfMassTransform(l_transform1);
                        }
                    }
                }
            }
        } break;
    }
}

void ROC::Skeleton::SetPhysicsEnabled(bool f_state)
{
    ms_physicsEnabled = f_state;
}
