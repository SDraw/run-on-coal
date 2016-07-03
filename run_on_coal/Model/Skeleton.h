#pragma once

namespace ROC
{

class Geometry;
class Bone;
class Skeleton
{
    struct skFastStoring
    {
        glm::vec3 m_pos;
        glm::quat m_rot;
        glm::vec3 m_scale;
    };
    skFastStoring m_leftData,m_rightData;

    std::vector<Bone*> m_boneVector;

    struct skChain
    {
        btRigidBody *m_rigidBody;
        btTypedConstraint *m_constraint;
        int m_boneID;
    };
    std::vector<std::vector<skChain>> m_chainsVector;
    bool m_rigid;
    glm::mat4 m_rigidBodyWorld;
protected:
    std::vector<glm::mat4> m_boneMatrices;
    Skeleton(std::vector<Geometry::geometryBoneData> &f_data);
    ~Skeleton();
    void InitRigidity(std::vector<std::vector<Geometry::geometryChainData>> &f_vec);
    void Update(std::vector<float> &f_left, std::vector<float> &f_right, float f_lerp);
    void Update();
    void UpdateChains(glm::mat4 &f_model);
    void UpdateRigidBones(glm::mat4 &f_model);

    friend class Model;
};

}
