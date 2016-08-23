#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ConfigManager.h"
#include "Managers/PhysicsManager.h"
#include "Model/Model.h"
#include "Model/Skeleton.h"
#include "Scene/Collision.h"

ROC::PhysicsManager::PhysicsManager(Core *f_core)
{
    m_core = f_core;

    m_broadPhase = new btDbvtBroadphase();
    m_collisionConfig = new btDefaultCollisionConfiguration();
    m_dispatcher = new btCollisionDispatcher(m_collisionConfig);
    m_solver = new btSequentialImpulseConstraintSolver();

    m_dynamicWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_broadPhase,m_solver,m_collisionConfig);
    m_dynamicWorld->setGravity(btVector3(0.f,-9.8f,0.f));

    m_floorEnabled = false;
    m_groundBody = NULL;

    m_enabled = false;

    m_timeStep = 1.f/static_cast<float>(m_core->GetConfigManager()->GetFPSLimit());
}
ROC::PhysicsManager::~PhysicsManager()
{
    if(m_groundBody)
    {
        m_dynamicWorld->removeRigidBody(m_groundBody);
        delete m_groundBody->getMotionState();
        delete m_groundBody;
    }

    delete m_dynamicWorld;
    delete m_solver;
    delete m_dispatcher;
    delete m_collisionConfig;
    delete m_broadPhase;
}

void ROC::PhysicsManager::DoPulse()
{
    if(!m_enabled) return;
    m_dynamicWorld->stepSimulation(m_timeStep,2,m_timeStep);
    for(auto iter:m_elementSet) iter->UpdateRigidity();
}

void ROC::PhysicsManager::SetFloorEnabled(bool f_value)
{
    if(m_floorEnabled==f_value) return;
    m_floorEnabled = f_value;
    if(m_floorEnabled)
    {
        btStaticPlaneShape *l_groundShape = new btStaticPlaneShape(btVector3(0.f,1.f,0.f),1.f);
        btDefaultMotionState *l_groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0.f,0.f,0.f,1.f),btVector3(0.f,-1.f,0.f)));
        btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0.f,l_groundMotionState,l_groundShape);
        m_groundBody = new btRigidBody(groundRigidBodyCI);
        m_dynamicWorld->addRigidBody(m_groundBody);
    }
    else
    {
        m_dynamicWorld->removeRigidBody(m_groundBody);
        delete m_groundBody->getMotionState();
        delete m_groundBody;
        m_groundBody = NULL;
    }
}

void ROC::PhysicsManager::GetGravity(glm::vec3 &f_grav)
{
    btVector3 l_grav = m_dynamicWorld->getGravity();
    std::memcpy(&f_grav,l_grav,sizeof(glm::vec3));
}

bool ROC::PhysicsManager::RayCast(glm::vec3 &f_start,glm::vec3 &f_end,glm::vec3 &f_normal,void **f_model)
{
    if(!std::memcmp(&f_start,&f_end,sizeof(glm::vec3))) return false;
    btCollisionWorld::ClosestRayResultCallback l_result((btVector3&)f_start,(btVector3&)f_end);
    m_dynamicWorld->rayTest((btVector3&)f_start,(btVector3&)f_end,l_result);
    if(!l_result.hasHit()) return false;
    btVector3 l_hitEnd = l_result.m_hitPointWorld;
    btVector3 l_hitNormal = l_result.m_hitNormalWorld;
    auto iter = m_bodyMap.find((void*)l_result.m_collisionObject);
    if(iter!=m_bodyMap.end()) *f_model = iter->second;
    std::memcpy(&f_end,l_hitEnd,sizeof(glm::vec3));
    std::memcpy(&f_normal,l_hitNormal,sizeof(glm::vec3));
    return true;
}

bool ROC::PhysicsManager::SetModelRigidity(Model *f_model,unsigned char f_type,float f_mass,glm::vec3 &f_dim)
{
    if(f_model->HasRigidSkeleton()) return false;
    if(!f_model->SetRigidity(f_type,f_mass,f_dim)) return false;
    m_elementSet.insert(f_model);
    m_bodyMap.insert(std::pair<void*,Model*>(f_model->m_rigidBody,f_model));
    m_dynamicWorld->addRigidBody(f_model->m_rigidBody);
    return true;
}
bool ROC::PhysicsManager::RemoveModelRigidity(Model *f_model)
{
    auto iter = m_elementSet.find(f_model);
    if(iter==m_elementSet.end()) return false;
    auto iter1 = m_bodyMap.find(f_model->m_rigidBody);
    if(iter1!=m_bodyMap.end()) m_bodyMap.erase(iter1);
    m_dynamicWorld->removeRigidBody(f_model->m_rigidBody);
    m_elementSet.erase(iter);
    if(!f_model->RemoveRigidity()) return false;
    return true;
}

void ROC::PhysicsManager::AddRigidSkeleton(Model *f_model)
{
    for(auto iter:f_model->m_skeleton->m_chainsVector)
    {
        for(auto iter1:iter)
        {
            m_dynamicWorld->addRigidBody(iter1.m_rigidBody);
            m_dynamicWorld->addConstraint(iter1.m_constraint);
        }
    }
    for(auto iter:f_model->m_skeleton->m_jointVector) m_dynamicWorld->addRigidBody(iter);
}
void ROC::PhysicsManager::RemoveRigidSkeleton(Model *f_model)
{
    for(auto iter:f_model->m_skeleton->m_chainsVector)
    {
        for(auto iter1:iter)
        {
            m_dynamicWorld->removeRigidBody(iter1.m_rigidBody);
            m_dynamicWorld->removeConstraint(iter1.m_constraint);
        }
    }
    for(auto iter:f_model->m_skeleton->m_jointVector) m_dynamicWorld->removeRigidBody(iter);
}

void ROC::PhysicsManager::AddCollision(Collision *f_col)
{
    if(m_collisionSet.find(f_col)!=m_collisionSet.end()) return;
    m_dynamicWorld->addRigidBody(f_col->m_rigidBody);
    m_collisionSet.insert(f_col);
    m_bodyMap.insert(std::pair<void*,void*>(f_col->m_rigidBody,f_col));
}
void ROC::PhysicsManager::RemoveCollision(Collision *f_col)
{
    auto iter = m_collisionSet.find(f_col);
    if(iter==m_collisionSet.end()) return;
    auto iter1 = m_bodyMap.find(f_col->m_rigidBody);
    if(iter1!=m_bodyMap.end()) m_bodyMap.erase(iter1);
    m_dynamicWorld->removeRigidBody(f_col->m_rigidBody);
    m_collisionSet.erase(iter);
}
