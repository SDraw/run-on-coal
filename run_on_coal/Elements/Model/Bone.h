#pragma once
namespace ROC
{

class BoneFrameData;
class Bone
{
    std::string m_name;
    BoneFrameData *m_data;
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

    Bone(const Bone& that);
    Bone &operator =(const Bone &that);
protected:
    Bone(const std::string &f_name, const glm::quat &f_rot, const glm::vec3 &f_pos, const glm::vec3 &f_scale);
    ~Bone();

    void SetFrameData(BoneFrameData *f_data);
    void ResetInterpolation();
    void GenerateBindPose();
    void UpdateMatrix();

    inline glm::mat4& GetLocalMatrixRef() { return m_localMatrix; }
    inline glm::mat4& GetMatrixRef() { return m_matrix; }
    inline glm::mat4& GetBindMatrixRef() { return m_bindMatrix; }
    inline glm::mat4& GetOffsetMatrixRef() { return m_offsetMatrix; }

    inline void SetParent( Bone *f_bone) { m_parent = f_bone; }
    inline bool HasParent() const { return (m_parent != NULL); }
    inline Bone* GetParent() { return m_parent;  }
    inline void AddChild(Bone *f_bone) { m_childBoneVector.push_back(f_bone); }

    inline bool IsRebuilded() const { return m_rebuilded; }
    inline void ForceRebuildState(bool f_state) { m_rebuilded = f_state; }

    friend class Skeleton;
    friend class Model;
    friend class Animation;
};

}
