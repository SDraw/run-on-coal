#include "stdafx.h"

#include "Managers/PhysicsManager.h"
#include "Core/Core.h"
#include "Elements/Collision.h"
#include "Elements/Model/Model.h"
#include "Elements/Model/Skeleton.h"

#include "Managers/ConfigManager.h"
#include "Managers/MemoryManager.h"
#include "Utils/SystemTick.h"

#define ROC_PHYSICS_DEFAULT_TIMESTEP 1.f/60.f
#define ROC_PHYSICS_DEFAULT_SUBSTEPS 10

ROC::PhysicsManager::PhysicsManager(Core *f_core)
{
    m_core = f_core;

    m_broadPhase = new btDbvtBroadphase();
    m_collisionConfig = new btDefaultCollisionConfiguration();
    m_dispatcher = new btCollisionDispatcher(m_collisionConfig);
    m_solver = new btSequentialImpulseConstraintSolver();

    m_dynamicWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadPhase, m_solver, m_collisionConfig);
    m_dynamicWorld->setGravity(btVector3(0.f, -9.8f, 0.f));

    m_floorBody = nullptr;

    m_enabled = false;

    unsigned int l_fpsLimit = m_core->GetConfigManager()->GetFPSLimit();
    m_timeStep = (l_fpsLimit == 0U) ? ROC_PHYSICS_DEFAULT_TIMESTEP : (1.f / static_cast<float>(l_fpsLimit));
}
ROC::PhysicsManager::~PhysicsManager()
{
    if(m_floorBody)
    {
        m_dynamicWorld->removeRigidBody(m_floorBody);
        delete m_floorBody->getMotionState();
        delete m_floorBody;
    }

    delete m_dynamicWorld;
    delete m_solver;
    delete m_dispatcher;
    delete m_collisionConfig;
    delete m_broadPhase;
}

void ROC::PhysicsManager::SetFloorEnabled(bool f_value)
{
    if(f_value && (m_floorBody == nullptr))
    {
        btStaticPlaneShape *l_groundShape = new btStaticPlaneShape(btVector3(0.f, 1.f, 0.f), 0.f);
        btDefaultMotionState *l_groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0.f, 0.f, 0.f, 1.f), btVector3(0.f, 0.f, 0.f)));
        btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0.f, l_groundMotionState, l_groundShape);
        m_floorBody = new btRigidBody(groundRigidBodyCI);
        m_dynamicWorld->addRigidBody(m_floorBody);
    }
    else if(!f_value && (m_floorBody != nullptr))
    {
        m_dynamicWorld->removeRigidBody(m_floorBody);
        delete m_floorBody->getMotionState();
        delete m_floorBody;
        m_floorBody = nullptr;
    }
}
void ROC::PhysicsManager::SetGravity(const glm::vec3 &f_grav)
{
    btVector3 l_grav(f_grav.x, f_grav.y, f_grav.z);
    m_dynamicWorld->setGravity(l_grav);

    btCollisionObjectArray &l_objectArray = m_dynamicWorld->getCollisionObjectArray();
    for(int i = 0, j = l_objectArray.size(); i < j; i++)
    {
        btRigidBody *l_body = dynamic_cast<btRigidBody*>(l_objectArray[i]);
        if(l_body)
        {
            l_body->setGravity(l_grav);
            l_body->activate(true);
        }
    }

}
void ROC::PhysicsManager::GetGravity(glm::vec3 &f_grav)
{
    std::memcpy(&f_grav, m_dynamicWorld->getGravity().m_floats, sizeof(glm::vec3));
}

void ROC::PhysicsManager::SetCollisionScale(Collision *f_col, const glm::vec3 &f_scale)
{
    btRigidBody *l_body = f_col->GetRigidBody();
    if(l_body)
    {
        m_dynamicWorld->removeRigidBody(l_body);
        f_col->SetScale(f_scale);
        m_dynamicWorld->addRigidBody(l_body);
    }
}
bool ROC::PhysicsManager::SetModelsCollidable(Model *f_model1, Model *f_model2, bool f_state)
{
    std::vector<btRigidBody*> l_bodies1, l_bodies2;
    if(f_model1->HasCollision()) l_bodies1.push_back(f_model1->GetCollision()->GetRigidBody());
    else
    {
        if(f_model1->HasSkeleton())
        {
            Skeleton *l_skeleton = f_model1->GetSkeleton();
            if(l_skeleton->HasStaticBoneCollision())
            {
                for(auto iter : l_skeleton->GetCollisionVectorRef()) l_bodies1.push_back(iter->m_rigidBody);
            }
            if(l_skeleton->HasDynamicBoneCollision())
            {
                for(auto iter : l_skeleton->GetJointVectorRef())
                {
                    for(auto iter1 : iter->m_partsVector) l_bodies1.push_back(iter1->m_rigidBody);
                }
            }
        }
    }
    if(f_model2->HasCollision()) l_bodies2.push_back(f_model2->GetCollision()->GetRigidBody());
    else
    {
        if(f_model2->HasSkeleton())
        {
            Skeleton *l_skeleton = f_model2->GetSkeleton();
            if(l_skeleton->HasStaticBoneCollision())
            {
                for(auto iter : l_skeleton->GetCollisionVectorRef()) l_bodies2.push_back(iter->m_rigidBody);
            }
            if(l_skeleton->HasDynamicBoneCollision())
            {
                for(auto iter : l_skeleton->GetJointVectorRef())
                {
                    for(auto iter1 : iter->m_partsVector) l_bodies2.push_back(iter1->m_rigidBody);
                }
            }
        }
    }
    for(auto iter1 : l_bodies1)
    {
        for(auto iter2 : l_bodies2)
        {
            iter1->setIgnoreCollisionCheck(iter2, !f_state);
            iter2->setIgnoreCollisionCheck(iter1, !f_state);
        }
    }
    return (!l_bodies1.empty() && !l_bodies2.empty());
}

void ROC::PhysicsManager::UpdateWorldSteps(unsigned int f_fps)
{
    m_timeStep = (f_fps == 0U) ? ROC_PHYSICS_DEFAULT_TIMESTEP : (1.f / static_cast<float>(f_fps));
}

void ROC::PhysicsManager::AddModel(Model *f_model)
{
    if(f_model->HasSkeleton())
    {
        for(auto iter : f_model->GetSkeleton()->GetCollisionVectorRef()) m_dynamicWorld->addRigidBody(iter->m_rigidBody);

        for(auto iter : f_model->GetSkeleton()->GetJointVectorRef())
        {
            m_dynamicWorld->addRigidBody(iter->m_emptyBody);
            for(auto iter1 : iter->m_partsVector)
            {
                m_dynamicWorld->addRigidBody(iter1->m_rigidBody);
                m_dynamicWorld->addConstraint(iter1->m_constraint, true);
            }
        }
    }
}
void ROC::PhysicsManager::RemoveModel(Model *f_model)
{
    if(f_model->HasSkeleton())
    {
        for(auto iter : f_model->GetSkeleton()->GetCollisionVectorRef()) m_dynamicWorld->removeRigidBody(iter->m_rigidBody);

        for(auto iter : f_model->GetSkeleton()->GetJointVectorRef())
        {
            m_dynamicWorld->removeRigidBody(iter->m_emptyBody);
            for(auto iter1 : iter->m_partsVector)
            {
                m_dynamicWorld->removeRigidBody(iter1->m_rigidBody);
                m_dynamicWorld->removeConstraint(iter1->m_constraint);
            }
        }
    }
}

void ROC::PhysicsManager::AddCollision(Collision *f_col)
{
    m_dynamicWorld->addRigidBody(f_col->GetRigidBody());
}
void ROC::PhysicsManager::RemoveCollision(Collision *f_col)
{
    m_dynamicWorld->removeRigidBody(f_col->GetRigidBody());
}

bool ROC::PhysicsManager::RayCast(const glm::vec3 &f_start, glm::vec3 &f_end, glm::vec3 &f_normal, Element *&f_element)
{
    bool l_result = false;
    if(f_start != f_end)
    {
        btVector3 l_start(f_start.x, f_start.y, f_start.z), l_end(f_end.x, f_end.y, f_end.z);
        btCollisionWorld::ClosestRayResultCallback l_rayResult(l_start, l_end);
        m_dynamicWorld->rayTest(l_start, l_end, l_rayResult);
        if(l_rayResult.hasHit())
        {
            void *l_colObject = l_rayResult.m_collisionObject->getUserPointer();
            if(l_colObject)
            {
                if(m_core->GetMemoryManager()->IsValidMemoryPointer(l_colObject)) f_element = reinterpret_cast<Element*>(l_colObject);
            }
            std::memcpy(&f_end, l_rayResult.m_hitPointWorld.m_floats, sizeof(glm::vec3));
            std::memcpy(&f_normal, l_rayResult.m_hitNormalWorld.m_floats, sizeof(glm::vec3));
            l_result = true;
        }
    }
    return l_result;
}

void ROC::PhysicsManager::DoPulse()
{
    if(m_enabled) m_dynamicWorld->stepSimulation(m_timeStep, ROC_PHYSICS_DEFAULT_SUBSTEPS, ROC_PHYSICS_DEFAULT_TIMESTEP);
}
