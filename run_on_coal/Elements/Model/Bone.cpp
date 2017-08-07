#include "stdafx.h"

#include "Elements/Model/Bone.h"
#include "Elements/Animation/BoneFrameData.h"

#define ROC_BONE_BLEND_FACTOR 0.1f

namespace ROC
{

extern const glm::mat4 g_IdentityMatrix;
extern const glm::vec3 g_DefaultPosition;
extern const glm::quat g_DefaultRotation;
extern const glm::vec3 g_DefaultScale;
extern const float g_Epsilon;

}

ROC::Bone::Bone(const std::string &f_name, const glm::quat &f_rot, const glm::vec3 &f_pos, const glm::vec3 &f_scl)
{
    m_parent = nullptr;
    m_name.assign(f_name);
    m_data = new BoneFrameData(f_pos, f_rot, f_scl);
    m_rebuildMatrix = false;
    m_rebuilded = false;
    m_interpolation = true;
    m_blendFactor = ROC_BONE_BLEND_FACTOR;
    m_blendValue = 0.f;
}
ROC::Bone::~Bone()
{
    delete m_data;
    m_childBoneVector.clear();
    m_parent = nullptr;
}

void ROC::Bone::SetFrameData(BoneFrameData *f_data)
{
    if(m_interpolation)
    {
        m_blendValue += m_blendFactor;
        if(m_blendValue >= 1.f)
        {
            m_interpolation = false;
            m_blendValue = 1.f;
        }
        m_data->SetInterpolated(f_data, m_blendValue);
        m_rebuildMatrix = true;
    }
    else
    {
        if(!m_data->IsEqual(f_data))
        {
            BoneFrameData::Copy(f_data, m_data);
            m_rebuildMatrix = true;
        }
    }
}
void ROC::Bone::EnableBlending(float f_blend)
{
    m_interpolation = true;
    m_blendFactor = f_blend;
    m_blendValue = 0.f;
}
void ROC::Bone::GenerateBindPose()
{
    btTransform l_transform = btTransform::getIdentity();
    btVector3 l_position(m_data->m_position.x, m_data->m_position.y, m_data->m_position.z);
    l_transform.setOrigin(l_position);
    btQuaternion l_rotation(m_data->m_rotation.x, m_data->m_rotation.y, m_data->m_rotation.z, m_data->m_rotation.w);
    l_transform.setRotation(l_rotation);
    l_transform.getOpenGLMatrix(glm::value_ptr(m_localMatrix));

    bool l_useScale = false;
    for(int i = 0; i < 3; i++)
    {
        if(glm::epsilonNotEqual(m_data->m_scale[i], g_DefaultScale[i], g_Epsilon))
        {
            l_useScale = true;
            break;
        }
    }
    if(l_useScale) m_localMatrix *= glm::scale(g_IdentityMatrix, m_data->m_scale);

    if(m_parent == nullptr) std::memcpy(&m_matrix, &m_localMatrix, sizeof(glm::mat4));
    else
    {
        std::memcpy(&m_matrix, &m_parent->m_matrix, sizeof(glm::mat4));
        m_matrix *= m_localMatrix;
    }
    m_bindMatrix = glm::inverse(m_matrix);
    std::memcpy(&m_offsetMatrix, &m_matrix, sizeof(glm::mat4));
    m_offsetMatrix *= m_bindMatrix;
}
void ROC::Bone::UpdateMatrix()
{
    m_rebuilded = false;
    if(m_rebuildMatrix)
    {
        btTransform l_transform = btTransform::getIdentity();
        btVector3 l_position(m_data->m_position.x, m_data->m_position.y, m_data->m_position.z);
        l_transform.setOrigin(l_position);
        btQuaternion l_rotation(m_data->m_rotation.x, m_data->m_rotation.y, m_data->m_rotation.z, m_data->m_rotation.w);
        l_transform.setRotation(l_rotation);
        l_transform.getOpenGLMatrix(glm::value_ptr(m_localMatrix));

        bool l_useScale = false;
        for(int i = 0; i < 3; i++)
        {
            if(glm::epsilonNotEqual(m_data->m_scale[i], g_DefaultScale[i], g_Epsilon))
            {
                l_useScale = true;
                break;
            }
        }
        if(l_useScale) m_localMatrix *= glm::scale(g_IdentityMatrix, m_data->m_scale);


        if(!m_parent) std::memcpy(&m_matrix, &m_localMatrix, sizeof(glm::mat4));
        else
        {
            std::memcpy(&m_matrix, &m_parent->m_matrix, sizeof(glm::mat4));
            m_matrix *= m_localMatrix;
        }
        std::memcpy(&m_offsetMatrix, &m_matrix, sizeof(glm::mat4));
        m_offsetMatrix *= m_bindMatrix;
        m_rebuilded = true;
        m_rebuildMatrix = false;
    }
    else
    {
        if(m_parent)
        {
            if(m_parent->m_rebuilded)
            {
                std::memcpy(&m_matrix, &m_parent->m_matrix, sizeof(glm::mat4));
                m_matrix *= m_localMatrix;
                std::memcpy(&m_offsetMatrix, &m_matrix, sizeof(glm::mat4));
                m_offsetMatrix *= m_bindMatrix;
                m_rebuilded = true;
                m_rebuildMatrix = false;
            }
        }
    }
}
