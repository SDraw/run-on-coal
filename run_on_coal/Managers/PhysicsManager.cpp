#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ConfigManager.h"
#include "Managers/MemoryManager.h"
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

    m_dynamicWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadPhase, m_solver, m_collisionConfig);
    m_dynamicWorld->setGravity(btVector3(0.f, -9.8f, 0.f));

    m_floorEnabled = false;
    m_groundBody = NULL;

    m_enabled = false;

    unsigned int l_fpsLimit = m_core->GetConfigManager()->GetFPSLimit();
    m_timeStep = (l_fpsLimit == 0U) ? (1.f / 60.f) : (1.f / static_cast<float>(l_fpsLimit));
    m_substeps = static_cast<int>(m_timeStep / (1.f / 60.f)) + 1;
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

void ROC::PhysicsManager::SetFloorEnabled(bool f_value)
{
    if(m_floorEnabled == f_value) return;
    m_floorEnabled = f_value;
    if(m_floorEnabled)
    {
        btStaticPlaneShape *l_groundShape = new btStaticPlaneShape(btVector3(0.f, 1.f, 0.f), 1.f);
        btDefaultMotionState *l_groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0.f, 0.f, 0.f, 1.f), btVector3(0.f, -1.f, 0.f)));
        btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0.f, l_groundMotionState, l_groundShape);
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
void ROC::PhysicsManager::SetGravity(glm::vec3 &f_grav)
{
    m_dynamicWorld->setGravity((btVector3&)f_grav);
    for(auto iter : m_modelsSet)
    {
        iter->GetRigidBody()->setGravity((btVector3&)f_grav);
        iter->GetRigidBody()->activate(true);
    }
}
void ROC::PhysicsManager::GetGravity(glm::vec3 &f_grav)
{
    btVector3 l_grav = m_dynamicWorld->getGravity();
    std::memcpy(&f_grav, l_grav, sizeof(glm::vec3));
}

bool ROC::PhysicsManager::SetModelRigidity(Model *f_model, unsigned char f_type, float f_mass, glm::vec3 &f_dim)
{
    if(m_modelsSet.find(f_model) != m_modelsSet.end()) return true;
    if(f_model->HasRigidSkeleton()) return false;
    if(!f_model->SetRigidity(f_type, f_mass, f_dim)) return false;
    m_modelsSet.insert(f_model);
    f_model->GetRigidBody()->setUserPointer(f_model);
    m_dynamicWorld->addRigidBody(f_model->GetRigidBody());
    return true;
}
bool ROC::PhysicsManager::RemoveModelRigidity(Model *f_model)
{
    auto iter = m_modelsSet.find(f_model);
    if(iter == m_modelsSet.end()) return false;
    m_dynamicWorld->removeRigidBody(f_model->GetRigidBody());
    m_modelsSet.erase(iter);
    if(!f_model->RemoveRigidity()) return false;
    return true;
}

void ROC::PhysicsManager::AddRigidSkeleton(Model *f_model)
{
    for(auto iter : f_model->GetSkeleton()->GetJointVectorRef()) m_dynamicWorld->addRigidBody(iter);
    for(auto iter : f_model->GetSkeleton()->GetChainsVectorRef())
    {
        for(auto iter1 : iter)
        {
            iter1.m_rigidBody->setUserPointer(f_model);
            m_dynamicWorld->addRigidBody(iter1.m_rigidBody);
            m_dynamicWorld->addConstraint(iter1.m_constraint);
        }
    }
}
void ROC::PhysicsManager::RemoveRigidSkeleton(Model *f_model)
{
    for(auto iter : f_model->GetSkeleton()->GetChainsVectorRef())
    {
        for(auto iter1 : iter)
        {
            m_dynamicWorld->removeRigidBody(iter1.m_rigidBody);
            m_dynamicWorld->removeConstraint(iter1.m_constraint);
        }
    }
    for(auto iter : f_model->GetSkeleton()->GetJointVectorRef()) m_dynamicWorld->removeRigidBody(iter);
}

void ROC::PhysicsManager::AddCollision(Collision *f_col)
{
    if(m_collisionsSet.find(f_col) != m_collisionsSet.end()) return;
    m_dynamicWorld->addRigidBody(f_col->GetRigidBody());
    m_collisionsSet.insert(f_col);
    f_col->GetRigidBody()->setUserPointer(f_col);
}
void ROC::PhysicsManager::RemoveCollision(Collision *f_col)
{
    auto iter = m_collisionsSet.find(f_col);
    if(iter == m_collisionsSet.end()) return;
    m_dynamicWorld->removeRigidBody(f_col->GetRigidBody());
    m_collisionsSet.erase(iter);
}

void ROC::PhysicsManager::DoPulse()
{
    if(!m_enabled) return;
    m_dynamicWorld->stepSimulation(m_timeStep, m_substeps);
    for(auto iter : m_modelsSet) iter->UpdateRigidity();
}

bool ROC::PhysicsManager::RayCast(glm::vec3 &f_start, glm::vec3 &f_end, glm::vec3 &f_normal, void **f_model)
{
    if(!std::memcmp(&f_start, &f_end, sizeof(glm::vec3))) return false;
    btCollisionWorld::ClosestRayResultCallback l_result((btVector3&)f_start, (btVector3&)f_end);
    m_dynamicWorld->rayTest((btVector3&)f_start, (btVector3&)f_end, l_result);
    if(!l_result.hasHit()) return false;
    void *l_colObject = l_result.m_collisionObject->getUserPointer();
    *f_model = l_colObject ? (m_core->GetMemoryManager()->IsValidMemoryPointer(l_colObject) ? l_colObject : NULL) : NULL;
    std::memcpy(&f_end, l_result.m_hitPointWorld, sizeof(glm::vec3));
    std::memcpy(&f_normal, l_result.m_hitNormalWorld, sizeof(glm::vec3));
    return true;
}

void ROC::PhysicsManager::UpdateWorldSteps(unsigned int f_fps)
{
    m_timeStep = (f_fps == 0U) ? (1.f / 60.f) : (1.f / static_cast<float>(f_fps));
    m_substeps = static_cast<int>(m_timeStep / (1.f / 60.f)) + 1;
}
