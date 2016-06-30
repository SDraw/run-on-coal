#include "stdafx.h"
#include "Model/Bone.h"

ROC::Bone::Bone(std::string &f_name, glm::quat &f_rot, glm::vec3 &f_pos, glm::vec3 &f_scale)
{
    m_name = f_name;
    std::memcpy(&m_rotation,&f_rot,sizeof(glm::quat));
    std::memcpy(&m_position,&f_pos,sizeof(glm::vec3));
    std::memcpy(&m_scale,&f_scale,sizeof(glm::vec3));
    m_parent = NULL;
    m_rebuildMatrix = false;
    m_forcedPosition = false;
    m_forcedRotation = false;
    m_forcedScale = false;
}
ROC::Bone::~Bone()
{
    m_childBoneVector.clear();
    m_parent = NULL;
}

void ROC::Bone::SetParent(Bone *f_bone)
{
    m_parent = f_bone;
}
void ROC::Bone::AddChild(Bone *f_bone)
{
    m_childBoneVector.push_back(f_bone);
}
void ROC::Bone::GenerateBindPose()
{
    glm::mat4 l_identity(1.f);
    m_localMatrix = glm::translate(l_identity,m_position)*glm::mat4_cast(m_rotation)*glm::scale(l_identity,m_scale);
    if(m_parent == NULL) m_matrix = m_localMatrix;
    else m_matrix = m_parent->m_matrix*m_localMatrix;
    m_bindMatrix = glm::inverse(m_matrix);
    m_offsetMatrix = m_matrix*m_bindMatrix;
    for(auto iter : m_childBoneVector) iter->GenerateBindPose();
}
void ROC::Bone::UpdateMatrix()
{
    if(m_rebuildMatrix)
    {
        glm::mat4 l_indent(1.f);
        m_localMatrix = glm::translate(l_indent,m_position)*glm::toMat4(m_rotation)*glm::scale(l_indent,m_scale);
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

    for(auto iter : m_childBoneVector) iter->UpdateMatrix();
    m_rebuildMatrix = false;
    m_forcedPosition = false;
    m_forcedRotation = false;
    m_forcedScale = false;
}

void ROC::Bone::SetPosition(glm::vec3 &f_pos, bool f_forced)
{
    if(m_forcedPosition && !f_forced) return;
    m_forcedPosition = f_forced;
    if(!std::memcmp(&m_position,&f_pos,sizeof(glm::vec3))) return;
    std::memcpy(&m_position,&f_pos,sizeof(glm::vec3));
    m_rebuildMatrix = true;
}
void ROC::Bone::SetRotation(glm::quat &f_rot, bool f_forced)
{
    if(m_forcedRotation && !f_forced) return;
    m_forcedRotation = f_forced;
    if(!std::memcmp(&m_rotation,&f_rot,sizeof(glm::quat))) return;
    std::memcpy(&m_rotation,&f_rot,sizeof(glm::quat));
    m_rebuildMatrix = true;
}
void ROC::Bone::SetScale(glm::vec3 &f_scale, bool f_forced)
{
    if(m_forcedScale && !f_forced) return;
    m_forcedScale = f_forced;
    if(!std::memcmp(&m_scale,&f_scale,sizeof(glm::vec3))) return;
    std::memcpy(&m_scale,&f_scale,sizeof(glm::vec3));
    m_rebuildMatrix = true;
}
