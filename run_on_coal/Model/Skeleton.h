#pragma once

namespace ROC
{

class Bone;
class BoneChainGroup;
class Skeleton
{
    struct skFastStoring
    {
        glm::vec3 m_pos;
        glm::quat m_rot;
        glm::vec3 m_scale;
    };
    skFastStoring m_leftData,m_rightData,m_interpolated;
    std::vector<Bone*> m_fastBoneVector;
    size_t m_jointsCount;
protected:
    size_t m_bonesCount;
    std::vector<Bone*> m_boneVector;
    std::vector<glm::mat4> m_boneMatrices;
    struct skChain
    {
        btRigidBody *m_rigidBody;
        btTypedConstraint *m_constraint;
        int m_boneID;
    };
    std::vector<std::vector<skChain>> m_chainsVector;
    std::vector<btRigidBody*> m_jointVector;
    bool m_rigid;
    Skeleton(std::vector<BoneData*> &f_data);
    ~Skeleton();
    void InitRigidity(std::vector<BoneChainGroup*> &f_vec);
    void Update(std::vector<float> &f_left, std::vector<float> &f_right, float f_lerp);
    void Update();
    void UpdateJoints(glm::mat4 &f_model, bool f_enabled);
    void UpdateRigidBones(glm::mat4 &f_model, bool f_enabled);
    void ResetBonesInterpolation();
    friend class Model;
};

}
