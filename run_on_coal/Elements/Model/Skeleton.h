#pragma once

namespace ROC
{

class Bone;
struct BoneCollisionData;
struct BoneData;
class BoneFrameData;
struct BoneJointData;

class Skeleton final
{
    size_t m_bonesCount;
    std::vector<Bone*> m_boneVector;
    std::vector<Bone*> m_fastBoneVector;
    std::vector<glm::mat4> m_poseMatrices;

    struct skCollision
    {
        btAlignedObjectArray<btTransform> m_offset; // [0] - normal
        btRigidBody *m_rigidBody;
        int m_boneID;
    };
    std::vector<skCollision*> m_collisionVector;
    bool m_hasStaticBoneCollision;

    struct skJoint
    {
        btAlignedObjectArray<btTransform> m_offsetMatrix; // [0] - main
        btRigidBody *m_emptyBody;
        size_t m_boneID;
        struct jtPart
        {
            btAlignedObjectArray<btTransform> m_offset; // [0] - main, [1] - inverse, [2] - bone bind
            btRigidBody *m_rigidBody;
            btGeneric6DofSpringConstraint *m_constraint;
            size_t m_boneID;
        };
        std::vector<jtPart*> m_partsVector;
    };
    std::vector<skJoint*> m_jointVector;
    bool m_hasDynamicBoneCollision;
public:
    inline size_t GetBonesCount() const { return m_bonesCount; }

    inline bool HasStaticBoneCollision() const { return m_hasStaticBoneCollision; }
    inline bool HasDynamicBoneCollision() const { return m_hasDynamicBoneCollision; }
protected:
    enum SkeletonUpdateStage : unsigned char
    {
        SUS_Static,
        SUS_Dynamic
    };

    explicit Skeleton(const std::vector<BoneData*> &f_data);
    ~Skeleton();

    void Update();

    inline std::vector<Bone*>& GetBones() { return m_boneVector; }
    inline const std::vector<glm::mat4>& GetPoseMatrices() const { return m_poseMatrices; }

    void InitStaticBoneCollision(const std::vector<BoneCollisionData*> &f_vec, void *f_model);
    inline const std::vector<skCollision*>& GetCollision() const { return m_collisionVector; }

    void InitDynamicBoneCollision(const std::vector<BoneJointData*> &f_vec, void *f_model);
    inline const std::vector<skJoint*>& GetJoints() const { return m_jointVector; }

    void UpdateCollision(SkeletonUpdateStage f_stage, const glm::mat4 &f_model, bool f_enabled);

    friend class Model;
    friend class RenderManager;
    friend class PhysicsManager;
};

}
