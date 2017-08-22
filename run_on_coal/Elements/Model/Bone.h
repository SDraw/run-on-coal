#pragma once
namespace ROC
{

class BoneFrameData;
class Bone final
{
    std::string m_name;
    BoneFrameData *m_data;
    glm::mat4 m_matrix;
    glm::mat4 m_localMatrix;
    glm::mat4 m_bindMatrix;
    glm::mat4 m_poseMatrix;
    bool m_rebuildMatrix;
    bool m_rebuilded;

    bool m_blend;
    float m_blendValue;

    Bone *m_parent;
    std::vector<Bone*> m_childBoneVector;

    Bone(const Bone& that);
    Bone &operator =(const Bone &that);
public:
    inline bool IsRebuilded() const { return m_rebuilded; }

    void SetBlending(float f_blend);

    inline const glm::mat4& GetLocalMatrix() const { return m_localMatrix; }
    inline const glm::mat4& GetMatrix() const { return m_matrix; }
    inline const glm::mat4& GetBindMatrix() const { return m_bindMatrix; }
    inline const glm::mat4& GetPoseMatrix() const { return m_poseMatrix; }
protected:
    Bone(const std::string &f_name, const glm::quat &f_rot, const glm::vec3 &f_pos, const glm::vec3 &f_scl);
    ~Bone();

    void GenerateBindPose();
    void SetFrameData(BoneFrameData *f_data);
    inline void SetMatrix(const btTransform &f_transform) { f_transform.getOpenGLMatrix(glm::value_ptr(m_matrix)); }
    inline void SetPoseMatrix(const btTransform &f_transform) { f_transform.getOpenGLMatrix(glm::value_ptr(m_poseMatrix)); }
    void Update();

    inline void SetParent( Bone *f_bone) { m_parent = f_bone; }
    inline bool HasParent() const { return (m_parent != nullptr); }
    inline Bone* GetParent() { return m_parent;  }
    inline void AddChild(Bone *f_bone) { m_childBoneVector.push_back(f_bone); }

    friend class Skeleton;
    friend class Animation;
};

}
