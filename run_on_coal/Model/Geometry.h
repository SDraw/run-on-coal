#pragma once

namespace ROC
{

class Material;
class Geometry
{
    std::vector<Material*> m_materialVector;
    unsigned int m_materialCount;

    struct geometryBoneData
    {
        std::string m_name;
        int m_parent;
        glm::vec3 m_position;
        glm::quat m_rotation;
        glm::vec3 m_scale;
    };
    std::vector<geometryBoneData> m_bonesData;

    struct geometryChainData
    {
        unsigned char m_type;
        float m_mass;
        glm::vec3 m_size;
        int m_boneID;
    };
    std::vector<std::vector<geometryChainData>> m_chainsData;

    bool m_loaded;
    void Clear();
    void SortMaterials();
public:
    bool HasBonesData();
    bool HasChainsData();
    unsigned int GetMaterialCount();
    void GetMaterialType(unsigned int f_material, std::bitset<8U> &f_type);
    void GetMaterialParam(unsigned int f_material,glm::vec4 &f_vec);
protected:
    Geometry();
    ~Geometry();
    bool Load(std::string &f_path, bool f_compressed);
    void GetBonesData(std::vector<geometryBoneData> &f_vec);
    void GetChainsData(std::vector<std::vector<geometryChainData>> &f_vec);
    void DrawMaterial(unsigned int f_material, bool f_texturize, bool f_binding);
    GLuint GetMaterialVAO(unsigned int f_material);
    GLuint GetMaterialTexture(unsigned int f_material);
    friend class ElementManager;
    friend class Model;
    friend class Skeleton;
};

}
