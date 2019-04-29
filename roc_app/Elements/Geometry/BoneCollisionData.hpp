#pragma once

namespace ROC
{

struct BoneCollisionData
{
    unsigned char m_type;
    glm::vec3 m_size;
    glm::vec3 m_offset;
    glm::quat m_offsetRotation;
    unsigned int m_boneID;
};

}
