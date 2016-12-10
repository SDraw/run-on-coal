#pragma once

namespace ROC
{

class BoneData;
class BoneChainGroup;
class Material;
class Geometry
{
    std::vector<Material*> m_materialVector;
    unsigned int m_materialCount;

    std::vector<BoneData*> m_bonesData;
    std::vector<BoneChainGroup*> m_chainsData;

    bool m_loaded;
    void Clear();
public:
    bool HasBonesData();
    bool HasChainsData();
protected:
    Geometry();
    ~Geometry();
    inline std::vector<Material*>& GetMaterialVectorRef() { return m_materialVector; }
    bool Load(std::string &f_path);
    void GetBonesData(std::vector<BoneData*> &f_vec);
    void GetChainsData(std::vector<BoneChainGroup*> &f_vec);
    friend class ElementManager;
    friend class RenderManager;
    friend class Model;
    friend class Skeleton;
};

}
