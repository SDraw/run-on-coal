#include "stdafx.h"
#include "Model/BoneJointData.h"

ROC::BoneJointData::BoneJointData()
{
    m_boneID = 0U;
}
ROC::BoneJointData::~BoneJointData()
{
    for(auto iter : m_jointPartVector) delete iter;
    m_jointPartVector.clear();
}

void ROC::BoneJointData::AddPart(unsigned int f_bone, unsigned char f_type, glm::vec3 &f_size, glm::vec3 &f_pos, glm::quat &f_rot, float f_mass, glm::vec3 &f_lAL, glm::vec3 &f_uAL, glm::vec3 &f_aStiffness, glm::vec3 &f_lLL, glm::vec3 &f_uLL, glm::vec3 &f_lStiffness)
{
    bjdJointPart *l_part = new bjdJointPart();
    l_part->m_boneID = f_bone;
    l_part->m_type = f_type;
    std::memcpy(&l_part->m_size, &f_size, sizeof(glm::vec3));
    std::memcpy(&l_part->m_offset, &f_pos, sizeof(glm::vec3));
    std::memcpy(&l_part->m_rotation, &f_rot, sizeof(glm::quat));
    l_part->m_mass = f_mass;
    std::memcpy(&l_part->m_lowerAngularLimit, &f_lAL, sizeof(glm::vec3));
    std::memcpy(&l_part->m_upperAngularLimit, &f_uAL, sizeof(glm::vec3));
    std::memcpy(&l_part->m_angularStiffness, &f_aStiffness, sizeof(glm::vec3));
    std::memcpy(&l_part->m_lowerLinearLimit, &f_lLL, sizeof(glm::vec3));
    std::memcpy(&l_part->m_upperLinearLimit, &f_uLL, sizeof(glm::vec3));
    std::memcpy(&l_part->m_linearStiffness, &f_lStiffness, sizeof(glm::vec3));
    m_jointPartVector.push_back(l_part);
}