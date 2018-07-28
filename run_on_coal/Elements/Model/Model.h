#pragma once

#include "Elements/Element.h"

#define ROC_MODEL_UPDATE_MATRIX 0
#define ROC_MODEL_UPDATE_COLLISION 1
#define ROC_MODEL_UPDATE_SKELETON1 2
#define ROC_MODEL_UPDATE_SKELETON2 3

namespace ROC
{

class Collision;
class Geometry;
class Skeleton;
class AnimationController;
class Transformation;

class Model final : public Element
{
    Geometry *m_geometry;


    float m_boundSphereRaduis;
    Transformation *m_localTransform;
    glm::mat4 m_fullMatrix;
    bool m_updated;

    Model *m_parent;
    int m_parentBone;

    AnimationController *m_animController;
    Skeleton *m_skeleton;
    Collision *m_collision;
public:
    inline bool HasGeometry() const { return (m_geometry != nullptr); }
    inline Geometry* GetGeometry() { return m_geometry; }

    void SetPosition(const glm::vec3 &f_pos);
    const glm::vec3& GetPosition() const;

    void SetRotation(const glm::quat &f_rot);
    const glm::quat& GetRotation() const;

    void SetScale(const glm::vec3 &f_scl);
    const glm::vec3& GetScale() const;

    inline const glm::mat4& GetFullMatrix() const { return m_fullMatrix; }

    float inline GetBoundSphereRadius() const { return m_boundSphereRaduis; }

    inline bool HasParent() const { return (m_parent != nullptr); }
    inline Model* GetParent() { return m_parent; }

    inline AnimationController* GetAnimationController() { return m_animController; }
    inline bool HasSkeleton() const { return (m_skeleton != nullptr); }

    inline bool HasCollision() const { return (m_collision != nullptr); }
    inline Collision* GetCollision() { return m_collision; }
protected:
    enum ModelUpdateStage : unsigned char
    {
        MUS_Matrix,
        MUS_Collision,
        MUS_Animation,
        MUS_SkeletonCollisionStatic,
        MUS_SkeletonCollisionDynamic
    };

    explicit Model(Geometry *f_geometry);
    ~Model();

    inline void SetGeometry(Geometry *f_geometry) { m_geometry = f_geometry; }

    void Update(ModelUpdateStage f_stage);
    inline const bool IsUpdated() const { return m_updated; }

    void SetParent(Model *f_model, int f_bone = -1);

    inline Skeleton* GetSkeleton() { return m_skeleton; }

    void SetCollision(Collision *f_col);

    friend class ElementManager;
    friend class InheritanceManager;
    friend class PhysicsManager;
    friend class PreRenderManager;
    friend class RenderManager;
};

}
