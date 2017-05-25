#pragma once

namespace ROC
{

struct BoneJointPartData
{
    unsigned int m_boneID;
    unsigned char m_type;
    glm::vec3 m_size;
    glm::vec3 m_offset;
    glm::quat m_rotation;
    float m_mass, m_restutition, m_friction;
    glm::vec2 m_damping;
    glm::vec3 m_lowerAngularLimit;
    glm::vec3 m_upperAngularLimit;
    glm::vec3 m_angularStiffness;
    glm::vec3 m_lowerLinearLimit;
    glm::vec3 m_upperLinearLimit;
    glm::vec3 m_linearStiffness;
};

struct BoneJointData
{
    unsigned int m_boneID;
    std::vector<BoneJointPartData*> m_jointPartVector;
};

}
