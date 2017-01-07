#pragma once
#define BC_TYPE_SPHERE 0U
#define BC_TYPE_BOX 1U
#define BC_TYPE_CYLINDER 2U
#define BC_TYPE_CAPSULE 3U
#define BC_TYPE_CONE 4U

namespace ROC
{

class Bone;
class BoneCollisionData;
class BoneData;
class BoneFrameData;
class BoneJointData;
class Skeleton
{
    unsigned int m_bonesCount;
    std::vector<Bone*> m_boneVector;
    std::vector<Bone*> m_fastBoneVector;
    std::vector<glm::mat4> m_boneMatrices;

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
        btAlignedObjectArray<btTransform> m_localMatrix; // [0] - normal
        btRigidBody *m_emptyBody;
        int m_boneID;
        struct jtPart
        {
            btAlignedObjectArray<btTransform> m_offset; // [0] - normal, [1] - inverse, [2] - bind
            btRigidBody *m_rigidBody;
            btGeneric6DofSpringConstraint *m_constraint;
            int m_boneID;
        };
        std::vector<jtPart*> m_partsVector;
    };
    std::vector<skJoint*> m_jointVector;
    bool m_hasDynamicBoneCollision;
protected:
    explicit Skeleton(std::vector<BoneData*> &f_data);
    ~Skeleton();

    void Update(std::vector<BoneFrameData*> &f_data);
    void Update();
    void ResetBonesInterpolation();

    void InitStaticBoneCollision(std::vector<BoneCollisionData*> &f_vec);
    void InitDynamicBoneCollision(std::vector<BoneJointData*> &f_vec);
    void UpdateCollision_S1(glm::mat4 &f_model, bool f_enabled);
    void UpdateCollision_S2(glm::mat4 &f_model, bool f_enabled);
    inline std::vector<skCollision*> &GetCollisionVectorRef() { return m_collisionVector; }
    inline std::vector<skJoint*>& GetJointVectorRef() { return m_jointVector; }
    inline bool HasStaticBoneCollision() { return m_hasStaticBoneCollision; }
    inline bool HasDynamicBoneCollision() { return m_hasDynamicBoneCollision; }

    inline unsigned int GetBonesCount() { return m_bonesCount; }
    inline std::vector<Bone*>& GetBonesVectorRef() { return m_boneVector; }
    inline std::vector<glm::mat4>& GetBoneMatricesVectorRef() { return m_boneMatrices; }

    friend class Model;
    friend class InheritanceManager;
    friend class PreRenderManager;
    friend class RenderManager;
    friend class PhysicsManager;
};

}
