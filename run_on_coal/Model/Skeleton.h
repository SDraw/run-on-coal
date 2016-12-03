#pragma once

namespace ROC
{
class Bone;
class BoneChainGroup;
class BoneData;
class Skeleton
{
    unsigned int m_bonesCount;
    std::vector<Bone*> m_boneVector;
    std::vector<Bone*> m_fastBoneVector;
    std::vector<glm::mat4> m_boneMatrices;

    size_t m_jointsCount;
    bool m_rigid;
    struct skChain
    {
        btRigidBody *m_rigidBody;
        btTypedConstraint *m_constraint;
        int m_boneID;
    };
    std::vector<std::vector<skChain>> m_chainsVector;
    std::vector<btRigidBody*> m_jointVector;

    struct skFastStoring
    {
        glm::vec3 m_pos;
        glm::quat m_rot;
        glm::vec3 m_scale;
    };
    skFastStoring m_leftData, m_rightData, m_interpolated;
protected:
    explicit Skeleton(std::vector<BoneData*> &f_data);
    ~Skeleton();

    void Update(std::vector<float> &f_left, std::vector<float> &f_right, float f_lerp);
    void Update();
    void ResetBonesInterpolation();

    void InitRigidity(std::vector<BoneChainGroup*> &f_vec);
    void UpdateJoints(glm::mat4 &f_model, bool f_enabled);
    void UpdateRigidBones(glm::mat4 &f_model, bool f_enabled);
    inline std::vector<std::vector<skChain>>& GetChainsVectorRef() { return m_chainsVector; }
    inline std::vector<btRigidBody*>& GetJointVectorRef() { return m_jointVector; }

    inline size_t GetBonesCount() { return m_bonesCount; }
    inline std::vector<Bone*>& GetBonesVectorRef() { return m_boneVector; }
    inline std::vector<glm::mat4>& GetBoneMatricesVectorRef() { return m_boneMatrices; }

    friend class Model;
    friend class InheritanceManager;
    friend class PreRenderManager;
    friend class RenderManager;
    friend class PhysicsManager;
};
}
