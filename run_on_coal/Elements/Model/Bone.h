#pragma once
namespace ROC
{

class BoneFrameData;
class Transformation;

class Bone final
{
    std::string m_name;
    Transformation *m_localTransform;
    glm::mat4 m_fullMatrix;
    glm::mat4 m_bindMatrix;
    glm::mat4 m_poseMatrix;
    bool m_updated;

    Bone *m_parent;
    std::vector<Bone*> m_childBoneVector;

    Bone(const Bone& that);
    Bone& operator=(const Bone &that);
public:
    inline const bool IsUpdated() const { return m_updated; }
    inline const Transformation* GetLocalTransformation() const { return m_localTransform; }
    inline const glm::mat4& GetBindMatrix() const { return m_bindMatrix; }
    inline const glm::mat4& GetFullMatrix() const { return m_fullMatrix; }
    inline const glm::mat4& GetPoseMatrix() const { return m_poseMatrix; }
protected:
    Bone(const std::string &f_name, const glm::quat &f_rot, const glm::vec3 &f_pos, const glm::vec3 &f_scl);
    ~Bone();

    void GenerateBindPose();
    void SetFrameData(BoneFrameData *f_data);
    void SetFrameData(BoneFrameData *f_data, float f_blend);
    void Update();

    inline void SetParent(Bone *f_bone) { m_parent = f_bone; }
    inline bool HasParent() const { return (m_parent != nullptr); }
    inline Bone* GetParent() { return m_parent; }
    inline void AddChild(Bone *f_bone) { m_childBoneVector.push_back(f_bone); }

    void SetFullMatrix(const btTransform &f_transform);
    void SetPoseMatrix(const btTransform &f_transform);

    friend class Skeleton;
    friend class Animation;
};

}
