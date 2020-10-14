#pragma once

namespace ROC
{

class Animation;
class Bone;
struct BoneCollisionData;
struct BoneData;
struct BoneJointData;

class Skeleton final
{
    size_t m_bonesCount;
    std::vector<Bone*> m_bones;
    std::vector<Bone*> m_sortedBones;
    std::vector<glm::mat4> m_poseMatrices;

    enum SkeletonCollisionType : unsigned char
    {
        SCT_Sphere = 0U,
        SCT_Box,
        SCT_Cylinder,
        SCT_Capsule,
        SCT_Cone
    };
    enum SkeletonTransformationType : size_t
    {
        STT_Main = 0U,
        STT_Inverse,
        STT_Bind
    };
    struct SkeletonCollision
    {
        btAlignedObjectArray<btTransform> m_offset; // [0] - main
        btRigidBody *m_rigidBody;
        size_t m_boneID;
    };
    std::vector<SkeletonCollision*> m_collisions;

    struct SkeletonJointPart
    {
        btAlignedObjectArray<btTransform> m_offset; // [0] - main, [1] - inverse, [2] - bone bind
        btRigidBody *m_rigidBody;
        btGeneric6DofSpringConstraint *m_constraint;
        size_t m_boneID;
    };
    struct SkeletonJoint
    {
        btAlignedObjectArray<btTransform> m_transform;
        btRigidBody *m_emptyBody;
        size_t m_boneID;
        std::vector<SkeletonJointPart*> m_parts;
    };
    std::vector<SkeletonJoint*> m_joints;

    static bool ms_physicsEnabled;

    Skeleton(const Skeleton &that) = delete;
    Skeleton& operator=(const Skeleton &that) = delete;
public:
    size_t GetBonesCount() const;

    bool HasStaticBoneCollision() const;
    bool HasDynamicBoneCollision() const;
    bool HasAnyCollision() const;
protected:
    enum SkeletonUpdateStage : unsigned char
    {
        SUS_Static,
        SUS_Dynamic
    };

    explicit Skeleton(const std::vector<BoneData*> &f_data);
    ~Skeleton();

    void Update(Animation *f_anim, unsigned int f_tick, float f_blend);

    const std::vector<Bone*>& GetBones() const;
    const std::vector<glm::mat4>& GetPoseMatrices() const;

    void InitStaticBoneCollision(const std::vector<BoneCollisionData*> &f_vec, void *f_model);
    const std::vector<SkeletonCollision*>& GetCollision() const;

    void InitDynamicBoneCollision(const std::vector<BoneJointData*> &f_vec, void *f_model);
    const std::vector<SkeletonJoint*>& GetJoints() const;

    void UpdateCollision(SkeletonUpdateStage f_stage, const glm::mat4 &f_model);

    static void SetPhysicsEnabled(bool f_state);

    friend class Model;
    friend class RenderManager;
    friend class PhysicsManager;
};

}
