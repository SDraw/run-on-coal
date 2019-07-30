#pragma once
#include "Interfaces/IModel.h"
#include "Elements/Collidable.h"
#include "Elements/Collision.h"
#include "Elements/Animation/Animation.h"
#include "Elements/Geometry/Geometry.h"

namespace ROC
{

class AnimationController;
class Bone;
class Skeleton;
class Transformation;

class Model final : public Collidable, public virtual IModel
{
    Geometry *m_geometry;
    float m_boundSphereRaduis;

    Transformation *m_localTransform;
    glm::mat4 m_fullMatrix;
    bool m_updated;

    Model *m_parentModel;
    Bone *m_parentBone;

    AnimationController *m_animController;
    Skeleton *m_skeleton;
    Collision *m_collision;

    Model(const Model &that) = delete;
    Model& operator=(const Model &that) = delete;

    void GetRigidBodies(std::vector<btRigidBody*> &f_vec);

    // Interfaces reroute
    bool AttachTo(IModel *f_model, int f_bone = -1);
    bool SetCollision(ICollision *f_col);
    bool SetAnimation(IAnimation *f_anim);
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

    bool AttachTo(Model *f_model, int f_bone = -1);
    bool Dettach();
    Model* GetParentModel() const;
    inline bool HasParentModel() const { return (m_parentModel != nullptr); }

    bool SetCollision(Collision *f_col);
    bool RemoveCollision();
    Collision* GetCollsion() const;
    inline bool HasCollision() const { return (m_collision != nullptr); }

    bool SetAnimation(Animation *f_anim);
    bool RemoveAnimation();
    Animation* GetAnimation() const;
    bool PlayAnimation(bool f_loop);
    bool PauseAnimation();
    bool ResetAnimation();
    bool GetAnimationProperty(ModelAnimationProperty f_prop, float &f_value);
    bool SetAnimationProperty(ModelAnimationProperty f_prop, float f_value);
protected:
    enum ModelUpdateStage : unsigned char
    {
        MUS_Matrix = 0U,
        MUS_Collision,
        MUS_Animation,
        MUS_SkeletonCollisionStatic,
        MUS_SkeletonCollisionDynamic
    };

    explicit Model(Geometry *f_geometry);
    ~Model();

    inline bool HasSkeleton() const { return (m_skeleton != nullptr); }
    inline Skeleton* GetSkeleton() const { return m_skeleton; }

    void Update(ModelUpdateStage f_stage);
    inline bool IsUpdated() const { return m_updated; }

    void OnParentLinkDestroyed(Element *f_parent);

    friend class ElementManager;
    friend class PhysicsManager;
    friend class PreRenderManager;
    friend class RenderManager;
    friend class Scene;
};

}
