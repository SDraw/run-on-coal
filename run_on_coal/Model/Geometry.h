#pragma once

namespace ROC
{

class BoneData;
class BoneChainGroup;
class Material;
class Geometry
{
    std::vector<BoneData*> m_bonesData;
    std::vector<BoneChainGroup*> m_chainsData;

    bool m_loaded;
    void Clear();
    void SortMaterials();
public:
    bool HasBonesData();
    bool HasChainsData();
protected:
    std::vector<Material*> m_materialVector;
    unsigned int m_materialCount;
    Geometry();
    ~Geometry();
    bool Load(std::string &f_path, bool f_compressed);
    void GetBonesData(std::vector<BoneData*> &f_vec);
    void GetChainsData(std::vector<BoneChainGroup*> &f_vec);
    friend class ElementManager;
    friend class RenderManager;
    friend class Model;
    friend class Skeleton;
};

}
