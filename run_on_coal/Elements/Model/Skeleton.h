#pragma once
#include "Elements/Geometry/BoneCollisionData.hpp"
#include "Elements/Geometry/BoneData.hpp"
#include "Elements/Geometry/BoneJointData.hpp"

#define ROC_BONECOL_TYPE_SPHERE 0U
#define ROC_BONECOL_TYPE_BOX 1U
#define ROC_BONECOL_TYPE_CYLINDER 2U
#define ROC_BONECOL_TYPE_CAPSULE 3U
#define ROC_BONECOL_TYPE_CONE 4U
#define ROC_SKELETON_UPDATE_COLLISION1 0
#define ROC_SKELETON_UPDATE_COLLISION2 1

namespace ROC
{

class Bone;
class BoneFrameData;
class Skeleton final
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
            btAlignedObjectArray<btTransform> m_offset; // [0] - normal, [1] - inverse, [2] - bone bind
            btRigidBody *m_rigidBody;
            btGeneric6DofSpringConstraint *m_constraint;
            int m_boneID;
        };
        std::vector<jtPart*> m_partsVector;
    };
    std::vector<skJoint*> m_jointVector;
    bool m_hasDynamicBoneCollision;
public:
    inline unsigned int GetBonesCount() const { return m_bonesCount; }

    inline bool HasStaticBoneCollision() const { return m_hasStaticBoneCollision; }
    inline bool HasDynamicBoneCollision() const { return m_hasDynamicBoneCollision; }
protected:
    explicit Skeleton(const std::vector<BoneData*> &f_data);
    ~Skeleton();

    void Update();

    inline std::vector<Bone*>& GetBones() { return m_boneVector; }
    inline const std::vector<glm::mat4>& GetBoneMatrices() const { return m_boneMatrices; }

    void InitStaticBoneCollision(const std::vector<BoneCollisionData*> &f_vec, void *f_model);
    inline const std::vector<skCollision*>& GetCollision() const { return m_collisionVector; }

    void InitDynamicBoneCollision(const std::vector<BoneJointData*> &f_vec, void *f_model);
    inline const std::vector<skJoint*>& GetJoints() const { return m_jointVector; }

    void UpdateCollision(int f_state, const glm::mat4 &f_model, bool f_enabled);

    friend class Model;
    friend class RenderManager;
    friend class PhysicsManager;
};

}
