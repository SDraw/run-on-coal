#pragma once

namespace ROC
{
class BoneChainGroup
{
protected:
    struct BoneChainData
    {
        unsigned char m_type = 0U;
        float m_mass = -1.f;
        glm::vec3 m_size;
        int m_boneID = -1;
    };
    std::vector<BoneChainData*> m_boneChainDataVector;
    BoneChainGroup();
    ~BoneChainGroup();
    void AddChain(unsigned char f_type, float f_mass, glm::vec3 &f_size, int f_boneID);
    friend class Geometry;
    friend class Skeleton;
};
}
