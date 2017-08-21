#include "stdafx.h"

#include "Elements/Geometry/Geometry.h"
#include "Elements/Geometry/BoneCollisionData.hpp"
#include "Elements/Geometry/BoneData.hpp"
#include "Elements/Geometry/BoneJointData.hpp"
#include "Elements/Geometry/Material.h"

#include "Utils/zlibUtils.h"

#define ROC_GEOMETRY_TYPE_STATIC 1U
#define ROC_GEOMETRY_TYPE_ANIMATED 2U
#define ROC_GEOMETRY_COLLISION_SETTER 0xCBU

ROC::Geometry::Geometry(bool f_async)
{
    m_elementType = ElementType::GeometryElement;
    m_elementTypeName.assign("Geometry");

    m_loadState = gmLoadState::NotLoaded;
    m_async = f_async;
    m_released = !m_async;
    m_materialCount = 0U;
    m_boundSphereRaduis = 0.f;
}
ROC::Geometry::~Geometry()
{
    Clear();
}

bool ROC::Geometry::Load(const std::string &f_path)
{
    bool l_result = false;
    if(m_loadState == gmLoadState::NotLoaded)
    {
        m_loadState = gmLoadState::Loading;

        unsigned char l_type;
        std::ifstream l_file;
        l_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            l_file.open(f_path, std::ios::binary);
            std::string l_header(3, '\0');
            l_file.read(&l_header[0], 3);
            if(!l_header.compare("ROC"))
            {
                l_file.read(reinterpret_cast<char*>(&l_type), sizeof(unsigned char));

                int l_compressedSize, l_uncompressedSize;
                std::vector<unsigned char> l_tempData;

                //Vertices
                std::vector<glm::vec3> l_vertexData;
                l_file.read(reinterpret_cast<char*>(&l_compressedSize), sizeof(int));
                l_file.read(reinterpret_cast<char*>(&l_uncompressedSize), sizeof(int));
                l_tempData.resize(l_compressedSize);
                l_file.read(reinterpret_cast<char*>(l_tempData.data()), l_compressedSize);
                l_vertexData.resize(l_uncompressedSize / sizeof(glm::vec3));
                zlibUtils::UncompressData(l_tempData.data(), l_compressedSize, l_vertexData.data(), l_uncompressedSize);

                //UVs
                std::vector<glm::vec2> l_uvData;
                l_file.read(reinterpret_cast<char*>(&l_compressedSize), sizeof(int));
                l_file.read(reinterpret_cast<char*>(&l_uncompressedSize), sizeof(int));
                l_tempData.resize(l_compressedSize);
                l_file.read(reinterpret_cast<char*>(l_tempData.data()), l_compressedSize);
                l_uvData.resize(l_uncompressedSize / sizeof(glm::vec2));
                zlibUtils::UncompressData(l_tempData.data(), l_compressedSize, l_uvData.data(), l_uncompressedSize);

                //Normals
                std::vector<glm::vec3> l_normalData;
                l_file.read(reinterpret_cast<char*>(&l_compressedSize), sizeof(int));
                l_file.read(reinterpret_cast<char*>(&l_uncompressedSize), sizeof(int));
                l_tempData.resize(l_compressedSize);
                l_file.read(reinterpret_cast<char*>(l_tempData.data()), l_compressedSize);
                l_normalData.resize(l_uncompressedSize / sizeof(glm::vec3));
                zlibUtils::UncompressData(l_tempData.data(), l_compressedSize, l_normalData.data(), l_uncompressedSize);

                std::vector<glm::vec4> l_weightData;
                std::vector<glm::ivec4> l_indexData;
                if(l_type == ROC_GEOMETRY_TYPE_ANIMATED)
                {
                    // Weights
                    l_file.read(reinterpret_cast<char*>(&l_compressedSize), sizeof(int));
                    l_file.read(reinterpret_cast<char*>(&l_uncompressedSize), sizeof(int));
                    l_tempData.resize(l_compressedSize);
                    l_file.read(reinterpret_cast<char*>(l_tempData.data()), l_compressedSize);
                    l_weightData.resize(l_uncompressedSize / sizeof(glm::vec4));
                    zlibUtils::UncompressData(l_tempData.data(), l_compressedSize, l_weightData.data(), l_uncompressedSize);

                    //Indices
                    l_file.read(reinterpret_cast<char*>(&l_compressedSize), sizeof(int));
                    l_file.read(reinterpret_cast<char*>(&l_uncompressedSize), sizeof(int));
                    l_tempData.resize(l_compressedSize);
                    l_file.read(reinterpret_cast<char*>(l_tempData.data()), l_compressedSize);
                    l_indexData.resize(l_uncompressedSize / sizeof(glm::vec4));
                    zlibUtils::UncompressData(l_tempData.data(), l_compressedSize, l_indexData.data(), l_uncompressedSize);
                }

                //Materials
                int l_materialCount;
                glm::vec3 l_farthestPoint(0.f);
                l_file.read(reinterpret_cast<char*>(&l_materialCount), sizeof(int));
                for(int i = 0; i < l_materialCount; i++)
                {
                    unsigned char l_materialType;
                    glm::vec4 l_materialParam;
                    unsigned char l_difTextureLength;
                    std::string l_difTexture;

                    l_file.read(reinterpret_cast<char*>(&l_materialType), sizeof(unsigned char));
                    l_file.read(reinterpret_cast<char*>(&l_materialParam), sizeof(glm::vec4));
                    l_file.read(reinterpret_cast<char*>(&l_difTextureLength), sizeof(unsigned char));
                    if(l_difTextureLength)
                    {
                        l_difTexture.resize(l_difTextureLength);
                        l_file.read(&l_difTexture[0], l_difTextureLength);
                    }

                    std::vector<int> l_faceIndex;
                    l_file.read(reinterpret_cast<char*>(&l_compressedSize), sizeof(int));
                    l_file.read(reinterpret_cast<char*>(&l_uncompressedSize), sizeof(int));
                    l_tempData.resize(l_compressedSize);
                    l_file.read(reinterpret_cast<char*>(l_tempData.data()), l_compressedSize);
                    l_faceIndex.resize(l_uncompressedSize / sizeof(int));
                    zlibUtils::UncompressData(l_tempData.data(), l_compressedSize, l_faceIndex.data(), l_uncompressedSize);

                    std::vector<glm::vec3> l_tempVertex;
                    std::vector<glm::vec2> l_tempUV;
                    std::vector<glm::vec3> l_tempNormal;
                    std::vector<glm::vec4> l_tempWeight;
                    std::vector<glm::ivec4> l_tempIndex;
                    for(int j = 0, k = static_cast<int>(l_faceIndex.size()); j < k; j += 9)
                    {
                        l_tempVertex.push_back(l_vertexData[l_faceIndex[j]]);
                        l_farthestPoint = glm::max(l_farthestPoint, l_tempVertex.back());
                        l_tempVertex.push_back(l_vertexData[l_faceIndex[j + 1]]);
                        l_farthestPoint = glm::max(l_farthestPoint, l_tempVertex.back());
                        l_tempVertex.push_back(l_vertexData[l_faceIndex[j + 2]]);
                        l_farthestPoint = glm::max(l_farthestPoint, l_tempVertex.back());
                        l_tempUV.push_back(l_uvData[l_faceIndex[j + 3]]);
                        l_tempUV.push_back(l_uvData[l_faceIndex[j + 4]]);
                        l_tempUV.push_back(l_uvData[l_faceIndex[j + 5]]);
                        l_tempNormal.push_back(l_normalData[l_faceIndex[j + 6]]);
                        l_tempNormal.push_back(l_normalData[l_faceIndex[j + 7]]);
                        l_tempNormal.push_back(l_normalData[l_faceIndex[j + 8]]);
                        if(l_type == ROC_GEOMETRY_TYPE_ANIMATED)
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
                    if(l_type == ROC_GEOMETRY_TYPE_ANIMATED)
                    {
                        l_material->LoadWeights(l_tempWeight);
                        l_material->LoadIndices(l_tempIndex);
                    }
                    if(!m_async) l_material->GenerateVAO();
                    l_material->LoadTexture(l_difTexture);
                }
                m_materialCount = static_cast<unsigned int>(l_materialCount);
                if(m_materialCount > 0U)
                {
                    std::vector<Material*> l_matVecDef, l_matVecDefDouble, l_matVecDefTransp;
                    for(auto iter : m_materialVector)
                    {
                        if(iter->IsTransparent() || !iter->HasDepth()) l_matVecDefTransp.push_back(iter);
                        else iter->IsDoubleSided() ? l_matVecDefDouble.push_back(iter) : l_matVecDef.push_back(iter);
                    }
                    m_materialVector.clear();
                    m_materialVector.insert(m_materialVector.end(), l_matVecDefDouble.begin(), l_matVecDefDouble.end());
                    m_materialVector.insert(m_materialVector.end(), l_matVecDef.begin(), l_matVecDef.end());
                    m_materialVector.insert(m_materialVector.end(), l_matVecDefTransp.begin(), l_matVecDefTransp.end());
                    m_materialVector.shrink_to_fit();
                }
                m_boundSphereRaduis = glm::length(l_farthestPoint);

                if(l_type == ROC_GEOMETRY_TYPE_ANIMATED)
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
                            l_file.read(&l_boneData->m_name[0], l_boneNameLength);
                        }
                        l_file.read(reinterpret_cast<char*>(&l_boneData->m_parent), sizeof(int));
                        l_file.read(reinterpret_cast<char*>(&l_boneData->m_position), sizeof(glm::vec3));
                        l_file.read(reinterpret_cast<char*>(&l_boneData->m_rotation), sizeof(glm::quat));
                        l_file.read(reinterpret_cast<char*>(&l_boneData->m_scale), sizeof(glm::vec3));
                    }
                    m_bonesData.shrink_to_fit();
                }
                l_result = true;
            }
        }
        catch(const std::exception&)
        {
            Clear();
        }
        if(l_result && !m_async) m_loadState = gmLoadState::Loaded;

        if(l_result && (l_type == ROC_GEOMETRY_TYPE_ANIMATED))
        {
            try
            {
                unsigned char l_physicBlock = 0U;
                l_file.read(reinterpret_cast<char*>(&l_physicBlock), sizeof(unsigned char));
                if(l_physicBlock == ROC_GEOMETRY_COLLISION_SETTER)
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
                    m_collisionData.shrink_to_fit();

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
                                BoneJointPartData l_jointPart;

                                l_file.read(reinterpret_cast<char*>(&l_jointPart.m_boneID), sizeof(unsigned int));
                                l_file.read(reinterpret_cast<char*>(&l_jointPart.m_type), sizeof(unsigned char));
                                l_file.read(reinterpret_cast<char*>(&l_jointPart.m_size), sizeof(glm::vec3));
                                l_file.read(reinterpret_cast<char*>(&l_jointPart.m_offset), sizeof(glm::vec3));
                                l_file.read(reinterpret_cast<char*>(&l_jointPart.m_rotation), sizeof(glm::quat));

                                l_file.read(reinterpret_cast<char*>(&l_jointPart.m_mass), sizeof(float));
                                l_file.read(reinterpret_cast<char*>(&l_jointPart.m_restutition), sizeof(float));
                                l_file.read(reinterpret_cast<char*>(&l_jointPart.m_friction), sizeof(float));
                                l_file.read(reinterpret_cast<char*>(&l_jointPart.m_damping), sizeof(glm::vec2));

                                l_file.read(reinterpret_cast<char*>(&l_jointPart.m_lowerAngularLimit), sizeof(glm::vec3));
                                l_file.read(reinterpret_cast<char*>(&l_jointPart.m_upperAngularLimit), sizeof(glm::vec3));
                                l_file.read(reinterpret_cast<char*>(&l_jointPart.m_angularStiffness), sizeof(glm::vec3));

                                l_file.read(reinterpret_cast<char*>(&l_jointPart.m_lowerLinearLimit), sizeof(glm::vec3));
                                l_file.read(reinterpret_cast<char*>(&l_jointPart.m_upperLinearLimit), sizeof(glm::vec3));
                                l_file.read(reinterpret_cast<char*>(&l_jointPart.m_linearStiffness), sizeof(glm::vec3));

                                l_joint->m_jointPartVector.push_back(l_jointPart);
                            }
                            l_joint->m_jointPartVector.shrink_to_fit();
                        }
                    }
                    m_jointData.shrink_to_fit();
                }
            }
            catch(const std::exception&)
            {
                for(auto iter : m_collisionData) delete iter;
                m_collisionData.clear();

                for(auto iter : m_jointData) delete iter;
                m_jointData.clear();
            }
        }
    }
    return l_result;
}

void ROC::Geometry::Clear()
{
    for(auto iter : m_materialVector) delete iter;
    m_materialVector.clear();

    m_materialCount = 0U;
    m_boundSphereRaduis = 0.f;

    for(auto iter : m_bonesData) delete iter;
    m_bonesData.clear();

    for(auto iter : m_collisionData) delete iter;
    m_collisionData.clear();

    for(auto iter : m_jointData) delete iter;
    m_jointData.clear();

    m_loadState = gmLoadState::NotLoaded;

    if(m_async) m_released = true;
}

void ROC::Geometry::GenerateVAOs()
{
    if(m_async && (m_loadState == gmLoadState::Loading))
    {
        for(auto iter : m_materialVector) iter->GenerateVAO();
        m_loadState = gmLoadState::Loaded;
    }
}
