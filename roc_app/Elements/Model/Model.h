#pragma once
#include "Interfaces/IModel.h"
#include "Elements/Collidable.h"

namespace ROC
{

class Animation;
class AnimationController;
class Bone;
class Collision;
class Geometry;
class Skeleton;
class Transformation;

class Model final : public Collidable, public virtual IModel
{
    Geometry *m_geometry;
    float m_boundSphereRaduis;

    Transformation *m_localTransform;
    glm::mat4 m_fullMatrix;
    bool m_updated;

    bool m_visibility;
    float m_visibilityDistance;

    Model *m_parentModel;
    Bone *m_parentBone;

    AnimationController *m_animController;
    Skeleton *m_skeleton;
    Collision *m_collision;

    Model(const Model &that) = delete;
    Model& operator=(const Model &that) = delete;

    // ROC::Element
    void OnParentRemoved(Element *f_parent) override;

    // ROC::Collidable
    void GetRigidBodies(std::vector<btRigidBody*> &f_vec);

    // ROC::IModel
    IGeometry* GetIGeometry() const;
    bool AttachTo(IModel *f_model, int f_bone = -1);
    IModel* GetParentIModel() const;
    bool SetICollision(ICollision *f_col);
    ICollision* GetICollsion() const;
    bool SetIAnimation(IAnimation *f_anim);
    IAnimation* GetIAnimation() const;
public:
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

    bool SetCollision(Collision *f_col);
    bool RemoveCollision();
    Collision* GetCollision() const;

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

    Skeleton* GetSkeleton() const;

    void SetVisibility(bool f_state);
    bool IsVisible() const;

    void SetVisibilityDistance(float f_dist);
    float GetVisibilityDistance() const;

    bool IsUpdated() const;
    void Update(ModelUpdateStage f_stage);


    friend class ElementManager;
    friend class PhysicsManager;
    friend class PreRenderManager;
    friend class RenderManager;
    friend class Scene;
};

}
