#include "stdafx.h"
#include "Elements/Animation/BoneFrameData.h"
#include "Elements/Model/Bone.h"

const glm::mat4 g_IdentityMatrix(1.f);

ROC::Bone::Bone(std::string &f_name, glm::quat &f_rot, glm::vec3 &f_pos, glm::vec3 &f_scale)
{
    m_parent = NULL;
    m_name.assign(f_name);
    m_data = new BoneFrameData();
    std::memcpy(&m_data->m_position, &f_pos, sizeof(glm::vec3));
    std::memcpy(&m_data->m_rotation, &f_rot, sizeof(glm::quat));
    std::memcpy(&m_data->m_scale, &f_scale, sizeof(glm::vec3));
    m_rebuildMatrix = false;
    m_rebuilded = false;
    m_interpolation = true;
    m_interpolationValue = 0.f;
}
ROC::Bone::~Bone()
{
    delete m_data;
    m_childBoneVector.clear();
    m_parent = NULL;
}

void ROC::Bone::SetFrameData(BoneFrameData *f_data)
{
    if(m_interpolation)
    {
        m_interpolationValue += 0.1f;
        m_data->m_position = glm::lerp(m_data->m_position, f_data->m_position, m_interpolationValue);
        m_data->m_rotation = glm::slerp(m_data->m_rotation, f_data->m_rotation, m_interpolationValue);
        m_data->m_scale = glm::lerp(m_data->m_scale, f_data->m_scale, m_interpolationValue);
        if(m_interpolationValue >= 0.9f) m_interpolation = false;
        m_rebuildMatrix = true;
    }
    else
    {
        if(m_data->Compare(f_data))
        {
            m_data->CopyFrom(f_data);
            m_rebuildMatrix = true;
        }
    }
}
void ROC::Bone::ResetInterpolation()
{
    m_interpolation = true;
    m_interpolationValue = 0.f;
}
void ROC::Bone::GenerateBindPose()
{
    m_localMatrix = glm::translate(g_IdentityMatrix, m_data->m_position)*glm::mat4_cast(m_data->m_rotation)*glm::scale(g_IdentityMatrix, m_data->m_scale);
    if(m_parent == NULL) m_matrix = m_localMatrix;
    else m_matrix = m_parent->m_matrix*m_localMatrix;
    m_bindMatrix = glm::inverse(m_matrix);
    m_offsetMatrix = m_matrix*m_bindMatrix;
}
void ROC::Bone::UpdateMatrix()
{
    m_rebuilded = false;
    if(m_rebuildMatrix)
    {
        m_localMatrix = glm::translate(g_IdentityMatrix, m_data->m_position)*glm::mat4_cast(m_data->m_rotation)*glm::scale(g_IdentityMatrix, m_data->m_scale);
        if(!m_parent) std::memcpy(&m_matrix, &m_localMatrix, sizeof(glm::mat4));
        else m_matrix = m_parent->m_matrix*m_localMatrix;
        m_offsetMatrix = m_matrix*m_bindMatrix;
        m_rebuilded = true;
        m_rebuildMatrix = false;
    }
    else
    {
        if(m_parent)
        {
            if(m_parent->m_rebuilded)
            {
                m_matrix = m_parent->m_matrix*m_localMatrix;
                m_offsetMatrix = m_matrix*m_bindMatrix;
                m_rebuilded = true;
                m_rebuildMatrix = false;
            }
        }
    }
}
