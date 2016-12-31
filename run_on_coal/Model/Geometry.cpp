#include "stdafx.h"
#include "Model/BoneCollisionData.h"
#include "Model/BoneData.h"
#include "Model/BoneJointData.h"
#include "Model/Geometry.h"
#include "Model/Material.h"
#include "Utils/Utils.h"

ROC::Geometry::Geometry()
{
    m_loaded = false;
    m_materialCount = 0U;
}
ROC::Geometry::~Geometry()
{
    Clear();
}

bool ROC::Geometry::Load(std::string &f_path)
{
    if(m_loaded) return false;
    bool l_result = true;
    unsigned char l_type;
    std::ifstream l_file;
    l_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        l_file.open(f_path, std::ios::binary);
        std::string l_header(3, '0');
        l_file.read(const_cast<char*>(l_header.data()), 3);
        if(!l_header.compare("ROC"))
        {
            l_file.read(reinterpret_cast<char*>(&l_type), sizeof(unsigned char));

            int l_compressedSize, l_uncompressedSize;
            std::vector<unsigned char> l_tempData;
            std::vector<glm::vec3> l_vertexData;
            std::vector<glm::vec2> l_uvData;
            std::vector<glm::vec3> l_normalData;
            std::vector<glm::vec4> l_weightData;
            std::vector<glm::ivec4> l_indexData;

            //Vertices
            l_file.read(reinterpret_cast<char*>(&l_compressedSize), sizeof(int));
            l_file.read(reinterpret_cast<char*>(&l_uncompressedSize), sizeof(int));
            l_tempData.resize(l_compressedSize);
            l_file.read(reinterpret_cast<char*>(l_tempData.data()), l_compressedSize);
            l_vertexData.resize(l_uncompressedSize / sizeof(glm::vec3));
            Utils::UncompressData(l_tempData.data(), l_compressedSize, l_vertexData.data(), l_uncompressedSize);

            //UVs
            l_file.read(reinterpret_cast<char*>(&l_compressedSize), sizeof(int));
            l_file.read(reinterpret_cast<char*>(&l_uncompressedSize), sizeof(int));
            l_tempData.resize(l_compressedSize);
            l_file.read(reinterpret_cast<char*>(l_tempData.data()), l_compressedSize);
            l_uvData.resize(l_uncompressedSize / sizeof(glm::vec2));
            Utils::UncompressData(l_tempData.data(), l_compressedSize, l_uvData.data(), l_uncompressedSize);

            //Normals
            l_file.read(reinterpret_cast<char*>(&l_compressedSize), sizeof(int));
            l_file.read(reinterpret_cast<char*>(&l_uncompressedSize), sizeof(int));
            l_tempData.resize(l_compressedSize);
            l_file.read(reinterpret_cast<char*>(l_tempData.data()), l_compressedSize);
            l_normalData.resize(l_uncompressedSize / sizeof(glm::vec3));
            Utils::UncompressData(l_tempData.data(), l_compressedSize, l_normalData.data(), l_uncompressedSize);

            if(l_type == 0x2U) // If has skeletal animation
            {
                // Weights
                l_file.read(reinterpret_cast<char*>(&l_compressedSize), sizeof(int));
                l_file.read(reinterpret_cast<char*>(&l_uncompressedSize), sizeof(int));
                l_tempData.resize(l_compressedSize);
                l_file.read(reinterpret_cast<char*>(l_tempData.data()), l_compressedSize);
                l_weightData.resize(l_uncompressedSize / sizeof(glm::vec4));
                Utils::UncompressData(l_tempData.data(), l_compressedSize, l_weightData.data(), l_uncompressedSize);

                //Indices
                l_file.read(reinterpret_cast<char*>(&l_compressedSize), sizeof(int));
                l_file.read(reinterpret_cast<char*>(&l_uncompressedSize), sizeof(int));
                l_tempData.resize(l_compressedSize);
                l_file.read(reinterpret_cast<char*>(l_tempData.data()), l_compressedSize);
                l_indexData.resize(l_uncompressedSize / sizeof(glm::vec4));
                Utils::UncompressData(l_tempData.data(), l_compressedSize, l_indexData.data(), l_uncompressedSize);
            }

            //Materials
            int l_materialSize;
            l_file.read(reinterpret_cast<char*>(&l_materialSize), sizeof(int));
            for(int i = 0; i < l_materialSize; i++)
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

                l_file.read(reinterpret_cast<char*>(&l_materialType), sizeof(unsigned char));
                l_file.read(reinterpret_cast<char*>(&l_materialParam), sizeof(glm::vec4));
                l_file.read(reinterpret_cast<char*>(&l_difTextureLength), sizeof(unsigned char));
                if(l_difTextureLength)
                {
                    l_difTexture.resize(l_difTextureLength);
                    l_file.read(const_cast<char*>(l_difTexture.data()), l_difTextureLength);
                }
                l_file.read(reinterpret_cast<char*>(&l_compressedSize), sizeof(int));
                l_file.read(reinterpret_cast<char*>(&l_uncompressedSize), sizeof(int));
                l_tempData.resize(l_compressedSize);
                l_file.read(reinterpret_cast<char*>(l_tempData.data()), l_compressedSize);
                l_faceIndex.resize(l_uncompressedSize / sizeof(int));
                Utils::UncompressData(l_tempData.data(), l_compressedSize, l_faceIndex.data(), l_uncompressedSize);

                for(int j = 0, k = static_cast<int>(l_faceIndex.size()); j < k; j += 9)
                {
                    l_tempVertex.push_back(l_vertexData[l_faceIndex[j]]);
                    l_tempVertex.push_back(l_vertexData[l_faceIndex[j + 1]]);
                    l_tempVertex.push_back(l_vertexData[l_faceIndex[j + 2]]);
                    l_tempUV.push_back(l_uvData[l_faceIndex[j + 3]]);
                    l_tempUV.push_back(l_uvData[l_faceIndex[j + 4]]);
                    l_tempUV.push_back(l_uvData[l_faceIndex[j + 5]]);
                    l_tempNormal.push_back(l_normalData[l_faceIndex[j + 6]]);
                    l_tempNormal.push_back(l_normalData[l_faceIndex[j + 7]]);
                    l_tempNormal.push_back(l_normalData[l_faceIndex[j + 8]]);
                    if(l_type == 0x2)
                    {
                        l_tempWeight.push_back(l_weightData[l_faceIndex[j]]);
                        l_tempWeight.push_back(l_weightData[l_faceIndex[j + 1]]);
                        l_tempWeight.push_back(l_weightData[l_faceIndex[j + 2]]);
                        l_tempIndex.push_back(l_indexData[l_faceIndex[j]]);
                        l_tempIndex.push_back(l_indexData[l_faceIndex[j + 1]]);
                        l_tempIndex.push_back(l_indexData[l_faceIndex[j + 2]]);
                    }
                }

                Material *l_material = new Material();
                m_materialVector.push_back(l_material);
                l_material->SetType(l_materialType);
                l_material->SetParams(l_materialParam);
                l_material->LoadVertices(l_tempVertex);
                l_material->LoadUVs(l_tempUV);
                l_material->LoadNormals(l_tempNormal);
                if(l_type == 0x2U)
                {
                    l_material->LoadWeights(l_tempWeight);
                    l_material->LoadIndices(l_tempIndex);
                }
                l_material->GenerateVAO();
                l_material->LoadTexture(l_difTexture);
            }
            m_materialCount = static_cast<unsigned int>(m_materialVector.size());
            if(m_materialCount)
            {
                std::vector<Material*> l_matVecDef, l_matVecDefDouble, l_matVecDefTransp;
                for(auto iter : m_materialVector)
                {
                    if(iter->IsTransparent() || !iter->IsDepthable()) l_matVecDefTransp.push_back(iter);
                    else iter->IsDoubleSided() ? l_matVecDefDouble.push_back(iter) : l_matVecDef.push_back(iter);
                }
                m_materialVector.clear();
                m_materialVector.insert(m_materialVector.end(), l_matVecDefDouble.begin(), l_matVecDefDouble.end());
                m_materialVector.insert(m_materialVector.end(), l_matVecDef.begin(), l_matVecDef.end());
                m_materialVector.insert(m_materialVector.end(), l_matVecDefTransp.begin(), l_matVecDefTransp.end());
            }

            if(l_type == 0x2U)
            {
                int l_bonesSize;
                l_file.read(reinterpret_cast<char*>(&l_bonesSize), sizeof(int));

                for(int i = 0; i < l_bonesSize; i++)
                {
                    BoneData *l_boneData = new BoneData();
                    m_bonesData.push_back(l_boneData);
                    unsigned char l_boneNameLength;

                    l_file.read(reinterpret_cast<char*>(&l_boneNameLength), sizeof(unsigned char));
                    if(l_boneNameLength)
                    {
                        l_boneData->m_name.resize(l_boneNameLength);
                        l_file.read(const_cast<char*>(l_boneData->m_name.data()), l_boneNameLength);
                    }
                    l_file.read(reinterpret_cast<char*>(&l_boneData->m_parent), sizeof(int));
                    l_file.read(reinterpret_cast<char*>(&l_boneData->m_position), sizeof(glm::vec3));
                    l_file.read(reinterpret_cast<char*>(&l_boneData->m_rotation), sizeof(glm::quat));
                    l_file.read(reinterpret_cast<char*>(&l_boneData->m_scale), sizeof(glm::vec3));
                }
            }
        }
        else l_result = false;
    }
    catch(const std::ifstream::failure &e)
    {
        Clear();
        l_result = false;
    }

    if(l_result && (l_type == 0x2U))
    {
        try
        {
            unsigned char l_physicBlock = 0U;
            l_file.read(reinterpret_cast<char*>(&l_physicBlock), sizeof(unsigned char));
            if(l_physicBlock == 0xCB)
            {
                unsigned int l_scbCount = 0U;
                l_file.read(reinterpret_cast<char*>(&l_scbCount), sizeof(unsigned int));
                for(unsigned int i = 0U; i < l_scbCount; i++)
                {
                    BoneCollisionData *l_colData = new BoneCollisionData();
                    m_collisionData.push_back(l_colData);
                    l_file.read(reinterpret_cast<char*>(&l_colData->m_type), sizeof(unsigned char));
                    l_file.read(reinterpret_cast<char*>(&l_colData->m_size), sizeof(glm::vec3));
                    l_file.read(reinterpret_cast<char*>(&l_colData->m_offset), sizeof(glm::vec3));
                    l_file.read(reinterpret_cast<char*>(&l_colData->m_offsetRotation), sizeof(glm::quat));
                    l_file.read(reinterpret_cast<char*>(&l_colData->m_boneID), sizeof(unsigned int));
                }

                unsigned int l_jointsCount = 0U;
                l_file.read(reinterpret_cast<char*>(&l_jointsCount), sizeof(unsigned int));
                for(unsigned int i = 0U; i < l_jointsCount; i++)
                {
                    unsigned int l_jointParts = 0U;
                    l_file.read(reinterpret_cast<char*>(&l_jointParts), sizeof(unsigned int));

                    if(l_jointParts > 0U)
                    {
                        BoneJointData *l_joint = new BoneJointData();
                        m_jointData.push_back(l_joint);
                        l_file.read(reinterpret_cast<char*>(&l_joint->m_boneID), sizeof(unsigned int));
                        for(unsigned int j = 0; j < l_jointParts; j++)
                        {
                            unsigned int l_boneID;
                            unsigned char l_type;
                            glm::vec3 l_size, l_offset;
                            glm::quat l_rotation;
                            float l_mass, l_restutition, l_friction;
                            glm::vec2 l_damping;
                            glm::vec3 l_lowerAngularLimit, l_upperAngularLimit, l_angularStiffness;
                            glm::vec3 l_lowerLinearLimit, l_upperLinearLimit, l_linearStiffness;

                            l_file.read(reinterpret_cast<char*>(&l_boneID), sizeof(unsigned int));
                            l_file.read(reinterpret_cast<char*>(&l_type), sizeof(unsigned char));
                            l_file.read(reinterpret_cast<char*>(&l_size), sizeof(glm::vec3));
                            l_file.read(reinterpret_cast<char*>(&l_offset), sizeof(glm::vec3));
                            l_file.read(reinterpret_cast<char*>(&l_rotation), sizeof(glm::quat));

                            l_file.read(reinterpret_cast<char*>(&l_mass), sizeof(float));
                            l_file.read(reinterpret_cast<char*>(&l_restutition), sizeof(float));
                            l_file.read(reinterpret_cast<char*>(&l_friction), sizeof(float));
                            l_file.read(reinterpret_cast<char*>(&l_damping), sizeof(glm::vec2));

                            l_file.read(reinterpret_cast<char*>(&l_lowerAngularLimit), sizeof(glm::vec3));
                            l_file.read(reinterpret_cast<char*>(&l_upperAngularLimit), sizeof(glm::vec3));
                            l_file.read(reinterpret_cast<char*>(&l_angularStiffness), sizeof(glm::vec3));

                            l_file.read(reinterpret_cast<char*>(&l_lowerLinearLimit), sizeof(glm::vec3));
                            l_file.read(reinterpret_cast<char*>(&l_upperLinearLimit), sizeof(glm::vec3));
                            l_file.read(reinterpret_cast<char*>(&l_linearStiffness), sizeof(glm::vec3));

                            l_joint->AddPart(l_boneID, l_type, l_size, l_offset, l_rotation, l_mass, l_restutition, l_friction, l_damping, l_lowerAngularLimit, l_upperAngularLimit, l_angularStiffness, l_lowerLinearLimit, l_upperLinearLimit, l_linearStiffness);
                        }
                    }
                }
            }
        }
        catch(const std::ifstream::failure &e)
        {
            for(auto iter : m_collisionData) delete iter;
            m_collisionData.clear();

            for(auto iter : m_jointData) delete iter;
            m_jointData.clear();
        }
    }
    m_loaded = l_result;
    return l_result;
}

void ROC::Geometry::Clear()
{
    for(auto iter : m_materialVector) delete iter;
    m_materialVector.clear();
    m_materialCount = 0U;
    for(auto iter : m_bonesData) delete iter;
    m_bonesData.clear();
    for(auto iter : m_collisionData) delete iter;
    m_collisionData.clear();
    for(auto iter : m_jointData) delete iter;
    m_jointData.clear();
    m_loaded = false;
}