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
    void SortMaterials();
public:
    bool HasBonesData();
    bool HasChainsData();
    unsigned int GetMaterialCount();
    unsigned char GetMaterialType(unsigned int f_material);
    void GetMaterialParam(unsigned int f_material,glm::vec4 &f_vec);
protected:
    Geometry();
    ~Geometry();
    bool Load(std::string &f_path, bool f_compressed);
    void GetBonesData(std::vector<BoneData*> &f_vec);
    void GetChainsData(std::vector<BoneChainGroup*> &f_vec);
    void DrawMaterial(unsigned int f_material, bool f_texturize, bool f_binding);
    GLuint GetMaterialVAO(unsigned int f_material);
    GLuint GetMaterialTexture(unsigned int f_material);
    friend class ElementManager;
    friend class Model;
    friend class Skeleton;
};

}
