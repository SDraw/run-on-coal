#pragma once
#include "Interfaces/IModel.h"
#include "Elements/Element.h"
#include "Elements/Collision.h"
#include "Elements/Animation/Animation.h"
#include "Elements/Geometry/Geometry.h"

namespace ROC
{

class AnimationController;
class Bone;
class Skeleton;
class Transformation;

class Model final : public Element, public virtual IModel
{
    Geometry *m_geometry;
    float m_boundSphereRaduis;

    Transformation *m_localTransform;
    glm::mat4 m_fullMatrix;
    bool m_updated;

    Model *m_parent;
    Bone *m_parentBone;

    AnimationController *m_animController;
    Skeleton *m_skeleton;
    Collision *m_collision;

    Model(const Model &that) = delete;
    Model& operator=(const Model &that) = delete;
public:
    inline bool HasGeometry() const { return (m_geometry != nullptr); }
    Geometry* GetGeometry() const;
    float GetBoundSphereRadius() const;

    void SetPosition(const glm::vec3 &f_pos);
    const glm::vec3& GetPosition() const;
    void SetRotation(const glm::quat &f_rot);
    const glm::quat& GetRotation() const;
    void SetScale(const glm::vec3 &f_scl);
    const glm::vec3& GetScale() const;
    const glm::mat4& GetFullMatrix() const;

    inline bool HasParent() const { return (m_parent != nullptr); }
    Model* GetParent() const;

    inline bool HasCollision() const { return (m_collision != nullptr); }
    Collision* GetCollsion() const;

    Animation* GetAnimation() const;
    bool PlayAnimation();
    bool PauseAnimation();
    bool ResetAnimation();
    bool GetAnimationProperty(ModelAnimationProperty f_prop, float &f_value);
    bool SetAnimationProperty(ModelAnimationProperty f_prop, float f_value);
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
    void SetParent(Model *f_model, int f_bone = -1);
    void SetCollision(Collision *f_col);

    inline bool HasAnimationController() const { return (m_animController != nullptr); }
    inline AnimationController* GetAnimationController() const { return m_animController; }

    inline bool HasSkeleton() const { return (m_skeleton != nullptr); }
    inline Skeleton* GetSkeleton() const { return m_skeleton; }

    void Update(ModelUpdateStage f_stage);
    inline bool IsUpdated() const { return m_updated; }

    friend class ElementManager;
    friend class InheritanceManager;
    friend class PhysicsManager;
    friend class PreRenderManager;
    friend class RenderManager;
};

}
