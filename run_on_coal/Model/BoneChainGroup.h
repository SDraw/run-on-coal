#pragma once

namespace ROC
{

class BoneChainData;
class BoneChainGroup
{
protected:
    std::vector<BoneChainData*> m_boneChainDataVector;
    BoneChainGroup();
    ~BoneChainGroup();
    void AddChainData(BoneChainData *f_cd);
    friend class Geometry;
    friend class Skeleton;
};

}
