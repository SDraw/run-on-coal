#pragma once

namespace ROC
{

class Core;
class Model;
class Collision;
class PhysicsManager
{
    Core *m_core;

    bool m_enabled;
    btDiscreteDynamicsWorld *m_dynamicWorld;
    btBroadphaseInterface *m_broadPhase;
    btDefaultCollisionConfiguration *m_collisionConfig;
    btCollisionDispatcher *m_dispatcher;
    btSequentialImpulseConstraintSolver *m_solver;
    float m_timeStep;

    btRigidBody *m_floorBody;

    PhysicsManager(const PhysicsManager& that);
    PhysicsManager &operator =(const PhysicsManager &that);
public:
    inline void SetPhysicsEnabled(bool f_value) { m_enabled = f_value; }
    inline bool GetPhysicsEnabled() const { return m_enabled; }
    void SetFloorEnabled(bool f_value);
    inline bool GetFloorEnabled() const { return (m_floorBody != nullptr); }
    void SetGravity(const glm::vec3 &f_grav);
    void GetGravity(glm::vec3 &f_grav);

    void SetCollisionScale(Collision *f_col, const glm::vec3 &f_scale);
    static bool SetModelsCollidable(Model *f_model1, Model *f_model2, bool f_state);

    bool RayCast(const glm::vec3 &f_start, glm::vec3 &f_end, glm::vec3 &f_normal, void *&f_model);
protected:
    explicit PhysicsManager(Core *f_core);
    ~PhysicsManager();

    void UpdateWorldSteps(unsigned int f_fps);

    void AddModel(Model *f_model);
    void RemoveModel(Model *f_model);
    void AddCollision(Collision *f_col);
    void RemoveCollision(Collision *f_col);

    void DoPulse();

    friend Core;
    friend class ElementManager;
    friend class SfmlManager;
};

}
