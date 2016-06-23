#pragma once
namespace ROC
{

class Bone
{
    Bone *m_parent;
    std::vector<Bone*> m_childBoneVector;

    std::string m_name;
    glm::quat m_rotation;
    glm::vec3 m_position;
    glm::vec3 m_scale;

    glm::mat4 m_matrix;
    glm::mat4 m_localMatrix;
    glm::mat4 m_bindMatrix;
    glm::mat4 m_offsetMatrix;

    bool m_rebuildMatrix;
    bool m_forcedPosition;
    bool m_forcedRotation;
    bool m_forcedScale;
protected:
    Bone(std::string &f_name, glm::quat &f_rot, glm::vec3 &f_pos, glm::vec3 &f_scale);
    ~Bone();
    void SetParent(Bone *f_bone);
    void AddChild(Bone *f_bone);
    void UpdateMatrix();
    void GenerateBindPose();
    void SetPosition(glm::vec3 &f_pos, bool f_forced = false);
    void SetRotation(glm::quat &f_rot, bool f_forced = false);
    void SetScale(glm::vec3 &f_scale, bool f_forced = false);
    friend class Skeleton;
    friend class Model;
};

}
