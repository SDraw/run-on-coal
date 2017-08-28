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
class Model final : public Element
{
    Geometry *m_geometry;

    glm::vec3 m_localPosition;
    glm::vec3 m_globalPosition;
    glm::quat m_localRotation;
    glm::quat m_globalRotation;
    glm::vec3 m_localScale;
    glm::vec3 m_globalScale;
    glm::mat4 m_localMatrix;
    glm::mat4 m_globalMatrix;
    float m_boundSphereRaduis;
    bool m_useScale;
    bool m_rebuildMatrix;
    bool m_rebuilded;

    Model *m_parent;
    int m_parentBone;

    AnimationController *m_animController;
    Skeleton *m_skeleton;
    Collision *m_collision;

    void UpdateGlobalTransform();

    Model(const Model& that);
    Model &operator =(const Model &that);
public:
    inline bool HasGeometry() const { return (m_geometry != nullptr); }
    inline Geometry* GetGeometry() { return m_geometry; }

    void SetLocalPosition(const glm::vec3 &f_pos);
    inline const glm::vec3& GetLocalPosition() const { return m_localPosition; }
    inline const glm::vec3& GetGlobalPosition() const { return m_globalPosition; }

    void SetLocalRotation(const glm::quat &f_rot);
    inline const glm::quat& GetLocalRotation() const { return m_localRotation; }
    inline const glm::quat& GetGlobalRotation() const { return m_globalRotation; }

    void SetLocalScale(const glm::vec3 &f_scl);
    inline const glm::vec3& GetLocalScale() const { return m_localScale; }
    inline const glm::vec3& GetGlobalScale() const { return m_globalScale; }

    inline const glm::mat4& GetLocalMatrix() const { return m_localMatrix; }
    inline const glm::mat4& GetGlobalMatrix() const { return m_globalMatrix; }

    float inline GetBoundSphereRadius() const { return m_boundSphereRaduis; }

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
        MUS_SkeletonStatic,
        MUS_SkeletonDynamic
    };

    explicit Model(Geometry *f_geometry);
    ~Model();

    inline void SetGeometry(Geometry *f_geometry) { m_geometry = f_geometry; }

    void Update(ModelUpdateStage f_stage, bool f_arg1 = false);

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
