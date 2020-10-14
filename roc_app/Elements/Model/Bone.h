#pragma once
namespace ROC
{

class BoneFrameData;
class Transformation;

class Bone final
{
#ifdef _DEBUG
    std::string m_name;
#endif
    Transformation *m_localTransform;
    glm::mat4 m_fullMatrix;
    glm::mat4 m_bindMatrix;
    glm::mat4 m_poseMatrix;
    btRigidBody *m_dynamicBody;
    bool m_dynamic;
    bool m_updated;

    Bone *m_parent;
    std::vector<Bone*> m_children;

    Bone(const Bone &that) = delete;
    Bone& operator=(const Bone &that) = delete;
public:
    Bone* GetParent() const;
    const std::vector<Bone*>& GetChildren() const;

    const Transformation* GetLocalTransformation() const;
    const glm::mat4& GetBindMatrix() const;
    const glm::mat4& GetFullMatrix() const;
    const glm::mat4& GetPoseMatrix() const;

    bool IsDynamic() const;
    const btRigidBody* GetDynamicBody() const;

    bool IsUpdated() const;
protected:
    Bone(const std::string &f_name, const glm::quat &f_rot, const glm::vec3 &f_pos, const glm::vec3 &f_scl);
    ~Bone();

    void GenerateBindPose();
    void SetFrameData(BoneFrameData *f_data);
    void SetFrameData(BoneFrameData *f_data, float f_blend);

    void SetParent(Bone *f_bone);
    void AddChild(Bone *f_bone);

    void SetDynamic(bool f_val);
    void SetDynamicBody(btRigidBody *f_body);

    void SetFullMatrix(const btTransform &f_transform);
    void SetPoseMatrix(const btTransform &f_transform);

    void Update();

    friend class Animation;
    friend class Model;
    friend class Skeleton;
};

}
