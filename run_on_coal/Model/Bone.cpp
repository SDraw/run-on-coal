#include "stdafx.h"
#include "Model/Bone.h"

glm::mat4 ROC::Bone::m_identity = glm::mat4(1.f);

ROC::Bone::Bone(std::string &f_name, glm::quat &f_rot, glm::vec3 &f_pos, glm::vec3 &f_scale)
{
    m_parent = NULL;
    m_name.insert(m_name.begin(),f_name.begin(),f_name.end());
    std::memcpy(&m_data.m_rot, &f_rot, sizeof(glm::quat));
    std::memcpy(&m_data.m_pos, &f_pos, sizeof(glm::vec3));
    std::memcpy(&m_data.m_scl, &f_scale, sizeof(glm::vec3));
    m_rebuildMatrix = false;
    m_rebuilded = false;
    m_interpolation = true;
    m_interpolationValue = 0.f;
}
ROC::Bone::~Bone()
{
    m_childBoneVector.clear();
    m_parent = NULL;
}

void ROC::Bone::SetData(void *f_data)
{
    if(m_interpolation)
    {
        bnStoring *l_data = static_cast<bnStoring*>(f_data);
        m_interpolationValue += 0.1f;
        m_data.m_pos = glm::lerp(m_data.m_pos, l_data->m_pos, m_interpolationValue);
        m_data.m_rot = glm::slerp(m_data.m_rot, l_data->m_rot, m_interpolationValue);
        m_data.m_scl = glm::lerp(m_data.m_scl, l_data->m_scl, m_interpolationValue);
        if(m_interpolationValue >= 0.9f) m_interpolation = false;
        m_rebuildMatrix = true;
    }
    else
    {
        if(!std::memcmp(&m_data, f_data, sizeof(bnStoring))) return;
        std::memcpy(&m_data, f_data, sizeof(bnStoring));
        m_rebuildMatrix = true;
    }
}
void ROC::Bone::GenerateBindPose()
{
    m_localMatrix = glm::translate(m_identity, m_data.m_pos)*glm::mat4_cast(m_data.m_rot)*glm::scale(m_identity, m_data.m_scl);
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
        m_localMatrix = glm::translate(m_identity, m_data.m_pos)*glm::mat4_cast(m_data.m_rot)*glm::scale(m_identity, m_data.m_scl);
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