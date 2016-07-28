#pragma once

namespace ROC
{

class BoneChainData
{
protected:
    unsigned char m_type;
    float m_mass;
    glm::vec3 m_size;
    int m_boneID;
    BoneChainData();
    ~BoneChainData();
    friend class Geometry;
    friend class Skeleton;
};

}
