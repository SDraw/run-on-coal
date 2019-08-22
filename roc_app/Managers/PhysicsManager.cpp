#include "stdafx.h"

#include "Managers/PhysicsManager.h"
#include "Core/Core.h"
#include "Elements/Element.h"
#include "Elements/Collision.h"
#include "Elements/Model/Model.h"

#include "Managers/ConfigManager.h"
#include "Managers/ElementManager.h"
#include "Elements/Model/Skeleton.h"

namespace ROC
{

const float g_PhysicsDefaultTimestep = 1.f / 120.f;
const int g_PhysicsDefaultSubstepsCount = 10;

}

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
    m_timeStep = (l_fpsLimit == 0U) ? g_PhysicsDefaultTimestep : (1.f / static_cast<float>(l_fpsLimit));
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

void ROC::PhysicsManager::SetPhysicsEnabled(bool f_value)
{
    if(m_enabled != f_value)
    {
        m_enabled = f_value;
        ROC::Skeleton::SetPhysicsEnabled(m_enabled);
    }
}
bool ROC::PhysicsManager::GetPhysicsEnabled() const 
{ 
    return m_enabled;
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
bool ROC::PhysicsManager::GetFloorEnabled() const 
{ 
    return (m_floorBody != nullptr);
}

void ROC::PhysicsManager::SetGravity(const glm::vec3 &f_grav)
{
    btVector3 l_grav(f_grav.x, f_grav.y, f_grav.z);
    m_dynamicWorld->setGravity(l_grav);

    btCollisionObjectArray &l_objectArray = m_dynamicWorld->getCollisionObjectArray();
    for(int i = 0, j = l_objectArray.size(); i < j; i++)
    {
        btRigidBody *l_body = btRigidBody::upcast(l_objectArray[i]);
        if(l_body)
        {
            l_body->setGravity(l_grav);
            l_body->activate(true);
        }
    }
}
void ROC::PhysicsManager::GetGravity(glm::vec3 &f_grav) const
{
    std::memcpy(&f_grav, m_dynamicWorld->getGravity().m_floats, sizeof(glm::vec3));
}

void ROC::PhysicsManager::SetCollisionScale(ICollision *f_col, const glm::vec3 &f_scale)
{
    SetCollisionScale(dynamic_cast<Collision*>(f_col), f_scale);
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

void ROC::PhysicsManager::UpdateWorldSteps(unsigned int f_fps)
{
    m_timeStep = (f_fps == 0U) ? g_PhysicsDefaultTimestep : (1.f / static_cast<float>(f_fps));
}

void ROC::PhysicsManager::AddModel(Model *f_model)
{
    if(f_model->HasSkeleton())
    {
        Skeleton *l_skeleton = f_model->GetSkeleton();
        if(l_skeleton->HasStaticBoneCollision())
        {
            for(auto l_skeletonCol : l_skeleton->GetCollision()) m_dynamicWorld->addRigidBody(l_skeletonCol->m_rigidBody);
        }
        if(l_skeleton->HasDynamicBoneCollision())
        {
            for(auto l_joint : l_skeleton->GetJoints())
            {
                m_dynamicWorld->addRigidBody(l_joint->m_emptyBody);
                for(auto l_jointPart : l_joint->m_parts)
                {
                    m_dynamicWorld->addRigidBody(l_jointPart->m_rigidBody);
                    m_dynamicWorld->addConstraint(l_jointPart->m_constraint, true);
                }
            }
        }
    }
}
void ROC::PhysicsManager::RemoveModel(Model *f_model)
{
    if(f_model->HasSkeleton())
    {
        Skeleton *l_skeleton = f_model->GetSkeleton();
        if(l_skeleton->HasStaticBoneCollision())
        {
            for(auto l_skeletonCol : l_skeleton->GetCollision()) m_dynamicWorld->removeRigidBody(l_skeletonCol->m_rigidBody);
        }
        if(l_skeleton->HasDynamicBoneCollision())
        {
            for(auto l_joint : l_skeleton->GetJoints())
            {
                m_dynamicWorld->removeRigidBody(l_joint->m_emptyBody);
                for(auto l_jointPart : l_joint->m_parts)
                {
                    m_dynamicWorld->removeRigidBody(l_jointPart->m_rigidBody);
                    m_dynamicWorld->removeConstraint(l_jointPart->m_constraint);
                }
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

bool ROC::PhysicsManager::RayCast(const glm::vec3 &f_start, glm::vec3 &f_end, glm::vec3 &f_normal, IElement *&f_element)
{
    return RayCast(f_start, f_end, f_normal, reinterpret_cast<Element*&>(f_element));
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
                if(m_core->GetElementManager()->IsValidElement(reinterpret_cast<Element*>(l_colObject))) f_element = reinterpret_cast<Element*>(l_colObject);
            }
            std::memcpy(&f_end, l_rayResult.m_hitPointWorld.m_floats, sizeof(glm::vec3));
            std::memcpy(&f_normal, l_rayResult.m_hitNormalWorld.m_floats, sizeof(glm::vec3));
            l_result = true;
        }
    }
    return l_result;
}

void ROC::PhysicsManager::SetDebugDrawer(btIDebugDraw *f_drawer)
{
    m_dynamicWorld->setDebugDrawer(f_drawer);
}
void ROC::PhysicsManager::DrawDebugWorld()
{
    m_dynamicWorld->debugDrawWorld();
}

void ROC::PhysicsManager::DoPulse()
{
    if(m_enabled) m_dynamicWorld->stepSimulation(m_timeStep, g_PhysicsDefaultSubstepsCount, g_PhysicsDefaultTimestep);
}
