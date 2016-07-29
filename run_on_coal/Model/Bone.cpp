#include "stdafx.h"
#include "Model/Bone.h"

glm::mat4 ROC::Bone::m_identity = glm::mat4(1.f);

ROC::Bone::Bone(std::string &f_name, glm::quat &f_rot, glm::vec3 &f_pos, glm::vec3 &f_scale)
{
    m_name = f_name;
    std::memcpy(&m_data.m_rot,&f_rot,sizeof(glm::quat));
    std::memcpy(&m_data.m_pos,&f_pos,sizeof(glm::vec3));
    std::memcpy(&m_data.m_scl,&f_scale,sizeof(glm::vec3));
    m_parent = NULL;
    m_rebuildMatrix = false;
}
ROC::Bone::~Bone()
{
    m_childBoneVector.clear();
    m_parent = NULL;
}

void ROC::Bone::GenerateBindPose()
{
    m_localMatrix = glm::translate(m_identity,m_data.m_pos)*glm::mat4_cast(m_data.m_rot)*glm::scale(m_identity,m_data.m_scl);
    if(m_parent == NULL) m_matrix = m_localMatrix;
    else m_matrix = m_parent->m_matrix*m_localMatrix;
    m_bindMatrix = glm::inverse(m_matrix);
    m_offsetMatrix = m_matrix*m_bindMatrix;
    for(auto iter : m_childBoneVector) iter->GenerateBindPose();
}
void ROC::Bone::GenerateFastTree(std::vector<Bone*> &f_vec)
{
    f_vec.push_back(this);
    for(auto iter : m_childBoneVector) iter->GenerateFastTree(f_vec);
}

void ROC::Bone::SetData(void *f_data)
{
    if(!std::memcmp(&m_data,f_data,sizeof(bnStoring))) return;
    std::memcpy(&m_data,f_data,sizeof(bnStoring));
    m_rebuildMatrix = true;
}

void ROC::Bone::UpdateMatrix()
{
    if(m_rebuildMatrix)
    {
        m_localMatrix = glm::translate(m_identity,m_data.m_pos)*glm::mat4_cast(m_data.m_rot)*glm::scale(m_identity,m_data.m_scl);
        if(!m_parent) std::memcpy(&m_matrix,&m_localMatrix,sizeof(glm::mat4));
        else m_matrix = m_parent->m_matrix*m_localMatrix;
        m_offsetMatrix = m_matrix*m_bindMatrix;
    }
    else
    {
        if(m_parent)
        {
            if(m_parent->m_rebuildMatrix)
            {
                m_matrix = m_parent->m_matrix*m_localMatrix;
                m_offsetMatrix = m_matrix*m_bindMatrix;
                m_rebuildMatrix = true;
            }
        }
    }
}
