#include "stdafx.h"
#include "Model/Geometry.h"
#include "Model/Material.h"
#include "Utils/Utils.h"

ROC::Geometry::Geometry()
{
    m_loaded = false;
}
ROC::Geometry::~Geometry()
{
    for(auto iter : m_materialVector) delete iter;
    m_materialVector.clear();
    m_bonesData.clear();
}
void ROC::Geometry::Clear()
{
    for(auto iter : m_materialVector) delete iter;
    m_materialVector.clear();
    m_materialCount = 0U;
    m_bonesData.clear();
    m_loaded = false;
}

void ROC::Geometry::SortMaterials()
{
    std::vector<Material*> l_matVecDef,l_matVecDefDouble,l_matVecDefTransp;
    for(auto iter : m_materialVector)
    {
        if(iter->IsTransparent() || !iter->IsDepthable()) l_matVecDefTransp.push_back(iter);
        else
        {
            if(iter->IsDoubleSided()) l_matVecDefDouble.push_back(iter);
            else l_matVecDef.push_back(iter);
        }
    }
    m_materialVector.clear();
    m_materialVector.insert(m_materialVector.end(),l_matVecDefDouble.begin(),l_matVecDefDouble.end());
    m_materialVector.insert(m_materialVector.end(),l_matVecDef.begin(),l_matVecDef.end());
    m_materialVector.insert(m_materialVector.end(),l_matVecDefTransp.begin(),l_matVecDefTransp.end());
}

bool ROC::Geometry::Load(std::string &f_path, bool f_compressed)
{
    if(m_loaded) return false;
    std::ifstream l_file;
    l_file.open(f_path,std::ios::binary);
    if(l_file.fail()) return false;

    std::string l_header;
    l_header.resize(3);
    l_file.read((char*)l_header.data(),3);
    if(l_file.fail() || l_header.compare("ROC")) return false;

    unsigned char l_type;
    l_file.read((char*)&l_type,sizeof(unsigned char));
    if(l_file.fail()) return false;

    int l_compressedSize, l_uncompressedSize;

    std::vector<unsigned char> l_tempData;
    std::vector<glm::vec3> l_vertexData;
    std::vector<glm::vec2> l_uvData;
    std::vector<glm::vec3> l_normalData;
    std::vector<glm::vec4> l_weightData;
    std::vector<glm::ivec4> l_indexData;

    //Vertices
    l_file.read((char*)&l_compressedSize,sizeof(int));
    if(l_file.fail()) return false;
    l_file.read((char*)&l_uncompressedSize,sizeof(int));
    if(l_file.fail()) return false;
    l_tempData.resize(l_compressedSize);
    l_file.read((char*)l_tempData.data(),l_compressedSize);
    if(l_file.fail()) return false;
    l_vertexData.resize(l_uncompressedSize/sizeof(glm::vec3));
    if(Utils::UncompressData(l_tempData.data(),l_compressedSize,l_vertexData.data(),l_uncompressedSize) == -1) return false;

    //UVs
    l_file.read((char*)&l_compressedSize,sizeof(int));
    if(l_file.fail()) return false;
    l_file.read((char*)&l_uncompressedSize,sizeof(int));
    if(l_file.fail()) return false;
    l_tempData.resize(l_compressedSize);
    l_file.read((char*)l_tempData.data(),l_compressedSize);
    if(l_file.fail()) return false;
    l_uvData.resize(l_uncompressedSize/sizeof(glm::vec2));
    if(Utils::UncompressData(l_tempData.data(),l_compressedSize,l_uvData.data(),l_uncompressedSize) == -1) return false;

    //Normals
    l_file.read((char*)&l_compressedSize,sizeof(int));
    if(l_file.fail()) return false;
    l_file.read((char*)&l_uncompressedSize,sizeof(int));
    if(l_file.fail()) return false;
    l_tempData.resize(l_compressedSize);
    l_file.read((char*)l_tempData.data(),l_compressedSize);
    if(l_file.fail()) return false;
    l_normalData.resize(l_uncompressedSize/sizeof(glm::vec3));
    if(Utils::UncompressData(l_tempData.data(),l_compressedSize,l_normalData.data(),l_uncompressedSize) == -1) return false;

    if(l_type == 0x2) // If has skeletal animation
    {
        // Weights
        l_file.read((char*)&l_compressedSize,sizeof(int));
        if(l_file.fail()) return false;
        l_file.read((char*)&l_uncompressedSize,sizeof(int));
        if(l_file.fail()) return false;
        l_tempData.resize(l_compressedSize);
        l_file.read((char*)l_tempData.data(),l_compressedSize);
        if(l_file.fail()) return false;
        l_weightData.resize(l_uncompressedSize/sizeof(glm::vec4));
        if(Utils::UncompressData(l_tempData.data(),l_compressedSize,l_weightData.data(),l_uncompressedSize) == -1) return false;

        //Indices
        l_file.read((char*)&l_compressedSize,sizeof(int));
        if(l_file.fail()) return false;
        l_file.read((char*)&l_uncompressedSize,sizeof(int));
        if(l_file.fail()) return false;
        l_tempData.resize(l_compressedSize);
        l_file.read((char*)l_tempData.data(),l_compressedSize);
        if(l_file.fail()) return false;
        l_indexData.resize(l_uncompressedSize/sizeof(glm::vec4));
        if(Utils::UncompressData(l_tempData.data(),l_compressedSize,l_indexData.data(),l_uncompressedSize) == -1) return false;
    }

    //Materials
    int l_materialSize;
    l_file.read((char*)&l_materialSize,sizeof(int));
    if(l_file.fail()) return false;
    if(l_materialSize < 1) return false;
    for(int i=0; i < l_materialSize; i++)
    {
        unsigned char l_materialType;
        glm::vec4 l_materialParam;
        unsigned char l_difTextureLength;
        std::string l_difTexture;
        std::vector<int> l_faceIndex;
        std::vector<glm::vec3> l_tempVertex;
        std::vector<glm::vec2> l_tempUV;
        std::vector<glm::vec3> l_tempNormal;
        std::vector<glm::vec4> l_tempWeight;
        std::vector<glm::ivec4> l_tempIndex;

        l_file.read((char*)&l_materialType,sizeof(unsigned char));
        if(l_file.fail()) 
        {
            Clear();
            return false;
        }
        l_file.read((char*)&l_materialParam,sizeof(glm::vec4));
        if(l_file.fail())
        {
            Clear();
            return false;
        }
        l_file.read((char*)&l_difTextureLength,sizeof(unsigned char));
        if(l_file.fail())
        {
            Clear();
            return false;
        }
        if(l_difTextureLength)
        {
            l_difTexture.resize(l_difTextureLength);
            l_file.read((char*)l_difTexture.data(),l_difTextureLength);
            if(l_file.fail())
            {
                Clear();
                return false;
            }
        }
        l_file.read((char*)&l_compressedSize,sizeof(int));
        if(l_file.fail())
        {
            Clear();
            return false;
        }
        l_file.read((char*)&l_uncompressedSize,sizeof(int));
        if(l_file.fail())
        {
            Clear();
            return false;
        }
        l_tempData.resize(l_compressedSize);
        l_file.read((char*)l_tempData.data(),l_compressedSize);
        if(l_file.fail())
        {
            Clear();
            return false;
        }
        l_faceIndex.resize(l_uncompressedSize/sizeof(int));
        if(Utils::UncompressData(l_tempData.data(),l_compressedSize,l_faceIndex.data(),l_uncompressedSize) == -1) return false;

        for(int j=0, k=int(l_faceIndex.size()); j < k; j += 9)
        {
            l_tempVertex.push_back(l_vertexData[l_faceIndex[j]]);
            l_tempVertex.push_back(l_vertexData[l_faceIndex[j+1]]);
            l_tempVertex.push_back(l_vertexData[l_faceIndex[j+2]]);
            l_tempUV.push_back(l_uvData[l_faceIndex[j+3]]);
            l_tempUV.push_back(l_uvData[l_faceIndex[j+4]]);
            l_tempUV.push_back(l_uvData[l_faceIndex[j+5]]);
            l_tempNormal.push_back(l_normalData[l_faceIndex[j+6]]);
            l_tempNormal.push_back(l_normalData[l_faceIndex[j+7]]);
            l_tempNormal.push_back(l_normalData[l_faceIndex[j+8]]);
            if(l_type == 0x2)
            {
                l_tempWeight.push_back(l_weightData[l_faceIndex[j]]);
                l_tempWeight.push_back(l_weightData[l_faceIndex[j+1]]);
                l_tempWeight.push_back(l_weightData[l_faceIndex[j+2]]);
                l_tempIndex.push_back(l_indexData[l_faceIndex[j]]);
                l_tempIndex.push_back(l_indexData[l_faceIndex[j+1]]);
                l_tempIndex.push_back(l_indexData[l_faceIndex[j+2]]);
            }
        }

        Material *l_material = new Material();
        l_material->SetType(l_materialType);
        l_material->SetParams(l_materialParam);
        l_material->LoadVertices(l_tempVertex);
        l_material->LoadUVs(l_tempUV);
        l_material->LoadNormals(l_tempNormal);
        if(l_type == 0x2)
        {
            l_material->LoadWeights(l_tempWeight);
            l_material->LoadIndices(l_tempIndex);
        }
        l_material->GenerateVAO();
        l_material->LoadTexture(l_difTexture,f_compressed);
        m_materialVector.push_back(l_material);
    }
    SortMaterials();
    m_materialCount = static_cast<unsigned int>(m_materialVector.size());
    if(l_type == 0x2)
    {
        int l_bonesSize;
        l_file.read((char*)&l_bonesSize,sizeof(int));
        if(l_file.fail())
        {
            Clear();
            return false;
        }
        if(l_bonesSize < 1) 
        {
            Clear();
            return false;
        }
        m_bonesData.resize(l_bonesSize);

        for(int i=0; i < l_bonesSize; i++)
        {
            unsigned char l_boneNameLength;

            l_file.read((char*)&l_boneNameLength,sizeof(unsigned char));
            if(l_file.fail())
            {
                Clear();
                return false;
            }
            if(l_boneNameLength)
            {
                m_bonesData[i].m_name.resize(l_boneNameLength);
                l_file.read((char*)m_bonesData[i].m_name.data(),l_boneNameLength);
                if(l_file.fail())
                {
                    Clear();
                    return false;
                }
            }
            l_file.read((char*)&m_bonesData[i].m_parent,sizeof(int));
            if(l_file.fail())
            {
                Clear();
                return false;
            }
            l_file.read((char*)&m_bonesData[i].m_position,sizeof(glm::vec3));
            if(l_file.fail())
            {
                Clear();
                return false;
            }
            l_file.read((char*)&m_bonesData[i].m_rotation,sizeof(glm::quat));
            if(l_file.fail())
            {
                Clear();
                return false;
            }
            l_file.read((char*)&m_bonesData[i].m_scale,sizeof(glm::vec3));
            if(l_file.fail())
            {
                Clear();
                return false;
            }
        }
    }
    unsigned char l_physicBlock = 0U;
    l_file.read((char*)&l_physicBlock,sizeof(unsigned char));
    if(l_physicBlock == 0xCB)
    {
        unsigned int l_chainsCount = 0U;
        l_file.read((char*)&l_chainsCount,sizeof(unsigned int));
        m_chainsData.resize(l_chainsCount);
        for(size_t i=0; i < l_chainsCount; i++)
        {
            unsigned int l_chainParts = 0U;
            l_file.read((char*)&l_chainParts,sizeof(unsigned int));
            for(size_t j=0; j < l_chainParts; j++)
            {
                geometryChainData l_chain;
                l_file.read((char*)&l_chain.m_type,sizeof(unsigned char));
                l_file.read((char*)&l_chain.m_mass,sizeof(float));
                l_file.read((char*)&l_chain.m_size,sizeof(glm::vec3));
                l_file.read((char*)&l_chain.m_boneID,sizeof(int));
                m_chainsData[i].push_back(l_chain);
            }
        }
    }
    m_loaded = true;
    return true;
}

bool ROC::Geometry::HasBonesData()
{
    return (m_bonesData.size() > 0U);
}
void ROC::Geometry::GetBonesData(std::vector<geometryBoneData> &f_vec)
{
    f_vec.insert(f_vec.begin(),m_bonesData.begin(),m_bonesData.end());
}

bool ROC::Geometry::HasChainsData()
{
    return (m_chainsData.size() > 0U);
}
void ROC::Geometry::GetChainsData(std::vector<std::vector<geometryChainData>> &f_vec)
{
    f_vec.insert(f_vec.begin(),m_chainsData.begin(),m_chainsData.end());
}

unsigned int ROC::Geometry::GetMaterialCount()
{
    return m_materialCount;
}
unsigned char ROC::Geometry::GetMaterialType(unsigned int f_material)
{
    if(f_material >= m_materialCount) return 0U;
    return m_materialVector[f_material]->GetType();
}
void ROC::Geometry::GetMaterialParam(unsigned int f_material,glm::vec4 &f_vec)
{
    if(f_material >= m_materialCount) return;
    m_materialVector[f_material]->GetParams(f_vec);
}
void ROC::Geometry::DrawMaterial(unsigned int f_material, bool f_texturize, bool f_binding)
{
    if(f_material >= m_materialCount) return;
    m_materialVector[f_material]->Draw(f_texturize,f_binding);
}
GLuint ROC::Geometry::GetMaterialVAO(unsigned int f_material)
{
    return ((f_material < m_materialCount) ? m_materialVector[f_material]->GetVAO() : 0);
}
GLuint ROC::Geometry::GetMaterialTexture(unsigned int f_material)
{
    return ((f_material < m_materialCount) ? m_materialVector[f_material]->GetTexture() : 0);
}
