#pragma once
#include "Elements/Element.h"
#define MODEL_RIGIDITY_TYPE_NONE -1
#define MODEL_RIGIDITY_TYPE_SPHERE 0
#define MODEL_RIGIDITY_TYPE_BOX 1
#define MODEL_RIGIDITY_TYPE_CYLINDER 2
#define MODEL_RIGIDITY_TYPE_CAPSULE 3
#define MODEL_RIGIDITY_TYPE_CONE 4
#define MODEL_TYPE_NONE 0
#define MODEL_TYPE_STATIC 1
#define MODEL_TYPE_ANIMATED 2

namespace ROC
{

class Collision;
class Geometry;
class Skeleton;
class Animation;
class Model : public Element
{
    Geometry *m_geometry;
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;
    glm::mat4 m_localMatrix;
    glm::mat4 m_matrix;
    float m_boundSphereRaduis;
    bool m_rebuildMatrix;
    bool m_rebuilded;

    Model *m_parent;
    int m_parentBone;

    Animation *m_animation;
    unsigned int m_animLastTick;
    unsigned int m_animCurrentTick;
    enum AnimationState { None = 0U, Paused, Playing } m_animState;
    float m_animationSpeed;

    Skeleton *m_skeleton;

    Collision *m_collision;

    void UpdateSkeleton();

    Model(const Model& that);
    Model &operator =(const Model &that);
public:
    inline bool HasGeometry() const { return (m_geometry != NULL); }
    inline Geometry* GetGeometry() { return m_geometry; }

    void SetPosition(const glm::vec3 &f_pos);
    void GetPosition(glm::vec3 &f_pos, bool f_global = false);
    void SetRotation(const glm::quat &f_rot);
    void GetRotation(glm::quat &f_rot, bool f_global = false);
    void SetScale(const glm::vec3 &f_scl);
    void GetScale(glm::vec3 &f_scl, bool f_global = false);

    inline Model* GetParent() { return m_parent; }

    inline bool HasAnimation() const { return (m_animation != NULL); }
    inline Animation* GetAnimation() { return m_animation; }
    bool PlayAnimation();
    bool PauseAnimation();
    bool ResetAnimation();
    bool SetAnimationSpeed(float f_val);
    inline float GetAnimationSpeed() const { return (m_animation ? m_animationSpeed : -1.f); }
    bool SetAnimationProgress(float f_val);
    float GetAnimationProgress() const;

    inline bool HasSkeleton() const { return (m_skeleton != NULL); }

    inline bool HasCollision() const { return (m_collision != NULL); }
    inline Collision* GetCollision() { return m_collision; }
protected:
    explicit Model(Geometry *f_geometry);
    ~Model();

    inline void SetGeometry(Geometry *f_geometry) { m_geometry = f_geometry; }

    inline glm::mat4& GetMatrixRef() { return m_matrix; }
    void UpdateMatrix();

    float inline GetBoundSphereRadius() const { return m_boundSphereRaduis; }

    void SetParent(Model *f_model, int f_bone = -1);

    void SetAnimation(Animation *f_anim);
    void UpdateAnimation();

    inline Skeleton* GetSkeleton() { return m_skeleton; }

    void SetCollision(Collision *f_col);
    void UpdateCollision();

    friend class ElementManager;
    friend class InheritanceManager;
    friend class PhysicsManager;
    friend class PreRenderManager;
    friend class RenderManager;
};

}
