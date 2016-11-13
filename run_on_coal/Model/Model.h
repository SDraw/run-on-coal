#pragma once
#define MODEL_RIGIDITY_TYPE_SPHERE 0U
#define MODEL_RIGIDITY_TYPE_BOX 1U
#define MODEL_RIGIDITY_TYPE_CYLINDER 2U
#define MODEL_RIGIDITY_TYPE_CAPSULE 3U
#define MODEL_RIGIDITY_TYPE_CONE 4U
#define MODEL_TYPE_NONE 0
#define MODEL_TYPE_STATIC 1
#define MODEL_TYPE_ANIMATED 2

namespace ROC
{
class Geometry;
class Skeleton;
class Animation;
class Model
{
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;
    glm::mat4 m_localMatrix;
    bool m_rebuildMatrix;

    Model *m_parent;
    int m_parentBone;

    Animation *m_animation;
    unsigned int m_animLastTick;
    unsigned int m_animCurrentTick;
    enum AnimationState { None = 0U, Paused, Playing } m_animState;
    float m_animationSpeed;

    void UpdateSkeleton();

    Model(const Model& that);
    Model &operator =(const Model &that);
public:
    inline Geometry* GetGeometry() { return m_geometry; }

    void SetPosition(glm::vec3 &f_pos);
    void GetPosition(glm::vec3 &f_pos, bool f_global = false);
    void SetRotation(glm::quat &f_rot);
    void GetRotation(glm::quat &f_rot, bool f_global = false);
    void SetScale(glm::vec3 &f_scl);
    void GetScale(glm::vec3 &f_scl, bool f_global = false);

    inline Model* GetParent() { return m_parent; }

    inline Animation* GetAnimation() { return m_animation; }
    bool PlayAnimation();
    bool PauseAnimation();
    bool ResetAnimation();
    bool SetAnimationSpeed(float f_val);
    inline float GetAnimationSpeed() { return (m_animation ? m_animationSpeed : -1.f); }
    bool SetAnimationProgress(float f_val);
    float GetAnimationProgress();

    inline bool HasSkeleton() { return (m_skeleton != NULL); }
    bool HasRigidSkeleton();

    inline bool IsRigid() { return (m_rigidBody != NULL); }
    bool SetVelocity(glm::vec3 &f_val);
    bool GetVelocity(glm::vec3 &f_val);
    bool SetAngularVelocity(glm::vec3 &f_val);
    bool GetAngularVelocity(glm::vec3 &f_val);
    float GetMass();
    bool SetFriction(float f_val);
    inline float GetFriction() { return (m_rigidBody ? m_rigidBody->getFriction() : -1.f); }

    inline bool IsDrawable() { return (m_geometry != NULL); }
    inline int GetType() { return (m_geometry ? (m_skeleton ? MODEL_TYPE_ANIMATED : MODEL_TYPE_STATIC) : MODEL_TYPE_NONE); }
protected:
    Geometry *m_geometry;
    glm::mat4 m_matrix;
    Skeleton *m_skeleton;
    btRigidBody* m_rigidBody;

    explicit Model(Geometry *f_geometry);
    ~Model();

    inline void SetGeometry(Geometry *f_geometry) { m_geometry = f_geometry; }
    void UpdateMatrix();
    void SetParent(Model *f_model, int f_bone = -1);
    void SetAnimation(Animation *f_anim);
    void UpdateAnimation();
    bool SetRigidity(unsigned char f_type, float f_mass, glm::vec3 &f_dim);
    bool RemoveRigidity();
    void UpdateRigidity();

    friend class ElementManager;
    friend class InheritanceManager;
    friend class PhysicsManager;
    friend class PreRenderManager;
    friend class RenderManager;
};
}
