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

    std::set<Model*> m_modelsSet;

    bool m_enabled;
    btDiscreteDynamicsWorld* m_dynamicWorld;
    btBroadphaseInterface* m_broadPhase;
    btDefaultCollisionConfiguration* m_collisionConfig;
    btCollisionDispatcher* m_dispatcher;
    btSequentialImpulseConstraintSolver* m_solver;
    float m_timeStep;
    int m_substeps;

    bool m_floorEnabled;
    btRigidBody* m_groundBody;

    PhysicsManager(const PhysicsManager& that);
    PhysicsManager &operator =(const PhysicsManager &that);
public:
    inline void SetPhysicsEnabled(bool f_value) { m_enabled = f_value; }
    inline bool GetPhysicsEnabled() { return m_enabled; }
    void SetFloorEnabled(bool f_value);
    inline bool GetFloorEnabled() { return m_floorEnabled; }
    void SetGravity(glm::vec3 &f_grav);
    void GetGravity(glm::vec3 &f_grav);

    bool SetModelCollision(Model *f_model, unsigned char f_type, float f_mass, glm::vec3 &f_dim);
    bool RemoveModelCollision(Model *f_model);

    bool RayCast(glm::vec3 &f_start, glm::vec3 &f_end, glm::vec3 &f_normal, void **f_model);
protected:
    explicit PhysicsManager(Core *f_core);
    ~PhysicsManager();

    void AddModel(Model *f_model);
    void RemoveModel(Model *f_model);
    void AddCollision(Collision *f_col);
    void RemoveCollision(Collision *f_col);

    void DoPulse();

    void UpdateWorldSteps(unsigned int f_fps);
    friend Core;
    friend class ElementManager;
    friend class SfmlManager;
};
}
