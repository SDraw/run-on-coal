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
    glm::mat4 m_offsetMatrix;
    bool m_rebuildMatrix;
    bool m_rebuilded;

    bool m_interpolation;
    float m_interpolationValue;

    Bone *m_parent;
    std::vector<Bone*> m_childBoneVector;

    static glm::mat4 m_identity;
protected:
    Bone(std::string &f_name, glm::quat &f_rot, glm::vec3 &f_pos, glm::vec3 &f_scale);
    ~Bone();

    void SetData(void *f_data);
    void GenerateBindPose();
    void UpdateMatrix();

    inline glm::mat4& GetMatrixRef() { return m_matrix; }
    inline glm::mat4& GetBindMatrixRef() { return m_bindMatrix; }
    inline glm::mat4& GetOffsetMatrixRef() { return m_offsetMatrix; }

    inline void SetParent(Bone *f_bone) { m_parent = f_bone; }
    inline void AddChild(Bone *f_bone) { m_childBoneVector.push_back(f_bone); }

    inline bool IsRebuilded() { return m_rebuilded; }

    friend class Skeleton;
    friend class Model;
};
}
