#pragma once
namespace ROC
{
class Bone
{
    std::string m_name;
    struct bnStoring
    {
        glm::vec3 m_pos;
        glm::quat m_rot;
        glm::vec3 m_scl;
    } m_data;
    glm::mat4 m_matrix;
    glm::mat4 m_localMatrix;
    glm::mat4 m_bindMatrix;

    Bone *m_parent;
    std::vector<Bone*> m_childBoneVector;

    static glm::mat4 m_identity;
protected:
    glm::mat4 m_offsetMatrix;
    bool m_rebuildMatrix;

    bool m_interpolation;
    float m_interpolationValue;

    Bone(std::string &f_name, glm::quat &f_rot, glm::vec3 &f_pos, glm::vec3 &f_scale);
    ~Bone();

    void SetData(void *f_data);
    void GenerateBindPose();
    void UpdateMatrix();

    inline void SetParent(Bone *f_bone) { m_parent = f_bone; }
    inline void AddChild(Bone *f_bone) { m_childBoneVector.push_back(f_bone); }

    void GenerateFastTree(std::vector<Bone*> &f_vec);
    friend class Skeleton;
    friend class Model;
};
}
