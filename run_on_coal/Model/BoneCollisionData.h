#pragma once

namespace ROC
{

class BoneCollisionData
{
protected:
    unsigned char m_type;
    glm::vec3 m_size;
    glm::vec3 m_offset;
    glm::quat m_offsetRotation;
    unsigned int m_boneID;
    BoneCollisionData();
    ~BoneCollisionData();
    friend class Geometry;
    friend class Skeleton;
};

}
