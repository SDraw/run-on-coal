#pragma once

namespace ROC
{

class Core;
class Constraint;
class Model;
class Collision;
class PhysicsManager
{
    Core *m_core;

    std::set<Model*> m_elementSet;
    std::map<void*,void*> m_bodyMap;

    std::set<Collision*> m_collisionSet;

    bool m_enabled;
    btDiscreteDynamicsWorld* m_dynamicWorld;
    btBroadphaseInterface* m_broadPhase;
    btDefaultCollisionConfiguration* m_collisionConfig;
    btCollisionDispatcher* m_dispatcher;
    btSequentialImpulseConstraintSolver* m_solver;
    
    bool m_floorEnabled;
    btRigidBody* m_groundBody;
public:
    void SetPhysicsEnabled(bool f_value);
    bool GetPhysicsEnabled();
    void SetFloorEnabled(bool f_value);
    bool GetFloorEnabled();
    void SetGravity(glm::vec3 &f_grav);
    void GetGravity(glm::vec3 &f_grav);
    bool SetModelRigidity(Model *f_model, unsigned char f_type, float f_mass, glm::vec3 &f_dim);
    bool RemoveModelRigidity(Model *f_model);
    bool RayCast(glm::vec3 &f_start, glm::vec3 &f_end, glm::vec3 &f_normal, void **f_model);
protected:
    PhysicsManager(Core *f_core);
    ~PhysicsManager();
    void DoPulse();
    void AddRigidSkeleton(Model *f_model);
    void RemoveRigidSkeleton(Model *f_model);
    void AddCollision(Collision *f_col);
    void RemoveCollision(Collision *f_col);
    friend Core;
    friend class ElementManager;
};

}
