#include "stdafx.h"

#include "Elements/Model/Bone.h"
#include "Elements/Animation/BoneFrameData.h"
#include "Utils/Transformation.h"

namespace ROC
{

extern const glm::mat4 g_identityMatrix;

}

ROC::Bone::Bone(const std::string &f_name, const glm::quat &f_rot, const glm::vec3 &f_pos, const glm::vec3 &f_scl)
{
    m_parent = nullptr;
#ifdef _DEBUG
    m_name.assign(f_name);
#endif

    m_localTransform = new Transformation();
    m_localTransform->SetPosition(f_pos);
    m_localTransform->SetRotation(f_rot);
    m_localTransform->SetScale(f_scl);
    m_localTransform->UpdateMatrix();

    m_fullMatrix = g_identityMatrix;
    m_bindMatrix = g_identityMatrix;
    m_poseMatrix = g_identityMatrix;
    m_updated = true;
    m_dynamic = false;
    m_dynamicBody = nullptr;
}

ROC::Bone::~Bone()
{
    m_children.clear();
    delete m_localTransform;
    m_parent = nullptr;
}

void ROC::Bone::GenerateBindPose()
{
    if(m_parent) m_fullMatrix = m_parent->m_fullMatrix*m_localTransform->GetMatrix();
    else std::memcpy(&m_fullMatrix, &m_localTransform->GetMatrix(), sizeof(glm::mat4));
    m_bindMatrix = glm::inverse(m_fullMatrix);
}


void ROC::Bone::SetDynamic(bool f_val)
{
    m_dynamic = f_val;
}

bool ROC::Bone::IsDynamic() const
{
    return m_dynamic;
}

void ROC::Bone::SetDynamicBody(btRigidBody *f_body)
{
    m_dynamicBody = f_body;
}

const btRigidBody* ROC::Bone::GetDynamicBody() const
{
    return m_dynamicBody;
}

void ROC::Bone::SetParent(Bone *f_bone)
{
    m_parent = f_bone;
}

ROC::Bone* ROC::Bone::GetParent() const
{
    return m_parent;
}

void ROC::Bone::AddChild(ROC::Bone *f_bone)
{
    m_children.push_back(f_bone);
}

const std::vector<ROC::Bone*>& ROC::Bone::GetChildren() const
{
    return m_children;
}

const ROC::Transformation* ROC::Bone::GetLocalTransformation() const
{
    return m_localTransform;
}

const glm::mat4& ROC::Bone::GetBindMatrix() const
{
    return m_bindMatrix;
}

const glm::mat4& ROC::Bone::GetFullMatrix() const
{
    return m_fullMatrix;
}

const glm::mat4& ROC::Bone::GetPoseMatrix() const
{
    return m_poseMatrix;
}

void ROC::Bone::SetFrameData(BoneFrameData *f_data)
{
    m_localTransform->SetPosition(f_data->m_position);
    m_localTransform->SetRotation(f_data->m_rotation);
    m_localTransform->SetScale(f_data->m_scale);
}

void ROC::Bone::SetFrameData(BoneFrameData *f_data, float f_blend)
{
    const glm::vec3 l_pos = glm::mix(m_localTransform->GetPosition(), f_data->m_position, f_blend);
    m_localTransform->SetPosition(l_pos);

    const glm::quat l_rot = glm::slerp(m_localTransform->GetRotation(), f_data->m_rotation, f_blend);
    m_localTransform->SetRotation(l_rot);

    const glm::vec3 l_scl = glm::mix(m_localTransform->GetScale(), f_data->m_scale, f_blend);
    m_localTransform->SetScale(l_scl);
}

void ROC::Bone::SetFullMatrix(const btTransform &f_transform)
{
    f_transform.getOpenGLMatrix(glm::value_ptr(m_fullMatrix));
    m_updated = true;
}

void ROC::Bone::SetPoseMatrix(const btTransform &f_transform)
{
    f_transform.getOpenGLMatrix(glm::value_ptr(m_poseMatrix));
    m_updated = true;
}

bool ROC::Bone::IsUpdated() const
{
    return m_updated;
}

void ROC::Bone::Update()
{
    m_updated = false;
    m_localTransform->UpdateMatrix();
    if(m_parent)
    {
        if(m_parent->IsUpdated() || m_localTransform->IsUpdated())
        {
            m_fullMatrix = m_parent->m_fullMatrix*m_localTransform->GetMatrix();
            m_updated = true;
        }
    }
    else
    {
        if(m_localTransform->IsUpdated())
        {
            std::memcpy(&m_fullMatrix, &m_localTransform->GetMatrix(), sizeof(glm::mat4));
            m_updated = true;
        }
    }
    if(m_updated) m_poseMatrix = m_fullMatrix*m_bindMatrix;
}
