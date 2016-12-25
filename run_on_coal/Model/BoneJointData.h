#pragma once

namespace ROC
{
class BoneJointData
{
protected:
    unsigned int m_boneID;

    struct bjdJointPart
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
    std::vector<bjdJointPart*> m_jointPartVector;
    BoneJointData();
    ~BoneJointData();
    void AddPart(unsigned int f_bone, unsigned char f_type, glm::vec3 &f_size, glm::vec3 &f_pos, glm::quat &f_rot, float f_mass, float f_restutition, float f_friction, glm::vec2 &f_damping, glm::vec3 &f_lAL, glm::vec3 &f_uAL, glm::vec3 &f_aStiffness, glm::vec3 &f_lLL, glm::vec3 &f_uLL, glm::vec3 &f_lStiffness);
    friend class Geometry;
    friend class Skeleton;
};
}
