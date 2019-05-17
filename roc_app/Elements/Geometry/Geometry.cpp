#include "stdafx.h"

#include "Elements/Geometry/Geometry.h"
#include "Elements/Geometry/BoneCollisionData.hpp"
#include "Elements/Geometry/BoneData.hpp"
#include "Elements/Geometry/BoneJointData.hpp"
#include "Elements/Geometry/Material.h"

#include "Utils/GLBinder.h"
#include "Utils/zlibUtils.h"

ROC::Geometry::Geometry()
{
    m_elementType = ET_Geometry;
    m_elementTypeName.assign("Geometry");

    m_materialCount = 0U;
    m_boundSphereRaduis = 0.f;
    m_loaded = false;
}
ROC::Geometry::~Geometry()
{
    Clear();
}

bool ROC::Geometry::Load(const std::string &f_path)
{
    if(!m_loaded)
    {
        GLint l_lastArrayBuffer = GLBinder::GetBindedArrayBuffer();
        GLint l_lastVertexArray = GLBinder::GetBindedVertexArray();

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
                l_file.read(reinterpret_cast<char*>(&m_boundSphereRaduis), sizeof(float));

                unsigned int l_materialCount = 0U;
                l_file.read(reinterpret_cast<char*>(&l_materialCount), sizeof(unsigned int));
                m_materialCount = l_materialCount;

                // Materials
                for(size_t i = 0U; i < m_materialCount; i++)
                {
                    Material *l_material = new Material;
                    m_materialVector.push_back(l_material);

                    unsigned char l_materialType = 0U;
                    l_file.read(reinterpret_cast<char*>(&l_materialType), sizeof(unsigned char));
                    l_material->SetType(l_materialType);

                    glm::vec4 l_materialParams(1.f);
                    l_file.read(reinterpret_cast<char*>(glm::value_ptr(l_materialParams)), sizeof(glm::vec4));
                    l_material->SetParams(l_materialParams);

                    unsigned char l_diffuseTextureNameLength = 0U;
                    l_file.read(reinterpret_cast<char*>(&l_diffuseTextureNameLength), sizeof(unsigned char));
                    if(l_diffuseTextureNameLength > 0U)
                    {
                        std::string l_diffuseTexture(l_diffuseTextureNameLength, '\0');
                        l_file.read(&l_diffuseTexture[0], l_diffuseTextureNameLength);
                        l_material->LoadTexture(l_diffuseTexture);
                    }

                    unsigned int l_compressedSize = 0U;
                    unsigned int l_sourceSize = 0U;
                    std::vector<char> l_compressedData;

                    // Vertex data
                    l_file.read(reinterpret_cast<char*>(&l_compressedSize), sizeof(unsigned int));
                    l_file.read(reinterpret_cast<char*>(&l_sourceSize), sizeof(unsigned int));

                    std::vector<glm::vec3> l_vertexData(l_sourceSize / sizeof(glm::vec3));
                    l_compressedData.resize(l_compressedSize, '\0');
                    l_file.read(l_compressedData.data(), l_compressedSize);
                    if(zlibUtils::UncompressData(l_compressedData.data(), l_compressedSize, l_vertexData.data(), l_sourceSize) != l_sourceSize) throw std::exception();
                    l_material->LoadVertices(l_vertexData);
                    l_vertexData.clear();

                    // Normal data
                    l_file.read(reinterpret_cast<char*>(&l_compressedSize), sizeof(unsigned int));
                    l_file.read(reinterpret_cast<char*>(&l_sourceSize), sizeof(unsigned int));

                    std::vector<glm::vec3> l_normalData(l_sourceSize / sizeof(glm::vec3));
                    l_compressedData.resize(l_compressedSize, '\0');
                    l_file.read(l_compressedData.data(), l_compressedSize);
                    if(zlibUtils::UncompressData(l_compressedData.data(), l_compressedSize, l_normalData.data(), l_sourceSize) != l_sourceSize) throw std::exception();
                    l_material->LoadNormals(l_normalData);
                    l_normalData.clear();

                    // UV data
                    l_file.read(reinterpret_cast<char*>(&l_compressedSize), sizeof(unsigned int));
                    l_file.read(reinterpret_cast<char*>(&l_sourceSize), sizeof(unsigned int));

                    std::vector<glm::vec2> l_uvData(l_sourceSize / sizeof(glm::vec2));
                    l_compressedData.resize(l_compressedSize, '\0');
                    l_file.read(l_compressedData.data(), l_compressedSize);
                    if(zlibUtils::UncompressData(l_compressedData.data(), l_compressedSize, l_uvData.data(), l_sourceSize) != l_sourceSize) throw std::exception();
                    l_material->LoadUVs(l_uvData);
                    l_uvData.clear();

                    if(l_type == GSB_Animated)
                    {
                        // Weight data
                        l_file.read(reinterpret_cast<char*>(&l_compressedSize), sizeof(unsigned int));
                        l_file.read(reinterpret_cast<char*>(&l_sourceSize), sizeof(unsigned int));

                        std::vector<glm::vec4> l_weightData(l_sourceSize / sizeof(glm::vec4));
                        l_compressedData.resize(l_compressedSize, '\0');
                        l_file.read(l_compressedData.data(), l_compressedSize);
                        if(zlibUtils::UncompressData(l_compressedData.data(), l_compressedSize, l_weightData.data(), l_sourceSize) != l_sourceSize) throw std::exception();
                        l_material->LoadWeights(l_weightData);
                        l_weightData.clear();

                        // Index data
                        l_file.read(reinterpret_cast<char*>(&l_compressedSize), sizeof(unsigned int));
                        l_file.read(reinterpret_cast<char*>(&l_sourceSize), sizeof(unsigned int));

                        std::vector<glm::ivec4> l_indexData(l_sourceSize / sizeof(glm::ivec4));
                        l_compressedData.resize(l_compressedSize, '\0');
                        l_file.read(l_compressedData.data(), l_compressedSize);
                        if(zlibUtils::UncompressData(l_compressedData.data(), l_compressedSize, l_indexData.data(), l_sourceSize) != l_sourceSize) throw std::exception();
                        l_material->LoadIndices(l_indexData);
                        l_indexData.clear();
                    }
                }

                // Skeleton
                if(l_type == GSB_Animated)
                {
                    unsigned int l_bonesCount;
                    l_file.read(reinterpret_cast<char*>(&l_bonesCount), sizeof(unsigned int));

                    for(size_t i = 0; i < l_bonesCount; i++)
                    {
                        BoneData *l_boneData = new BoneData();
                        m_bonesData.push_back(l_boneData);

                        unsigned char l_boneNameLength;
                        l_file.read(reinterpret_cast<char*>(&l_boneNameLength), sizeof(unsigned char));
                        if(l_boneNameLength > 0U)
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


                // Sort materials
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
            }
            m_loaded = true;
        }
        catch(const std::exception&)
        {
            Clear();
        }

        // Static and dynamic collision, only for animated geometry
        if(m_loaded)
        {
            if(l_type == GSB_Animated)
            {
                try
                {
                    unsigned char l_physicsBlock = 0U;
                    l_file.read(reinterpret_cast<char*>(&l_physicsBlock), sizeof(unsigned char));
                    if(l_physicsBlock == GSB_Collision)
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

                                    l_joint->m_jointParts.push_back(l_jointPart);
                                }
                                l_joint->m_jointParts.shrink_to_fit();
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

        GLBinder::BindArrayBuffer(l_lastArrayBuffer);
        GLBinder::BindVertexArray(l_lastVertexArray);
    }
    return m_loaded;
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

    m_loaded = false;
}

void ROC::Geometry::GenerateVAOs()
{
    if(m_loaded)
    {
        GLint l_lastArrayBuffer = GLBinder::GetBindedArrayBuffer();
        GLint l_lastVertexArray = GLBinder::GetBindedVertexArray();

        for(auto iter : m_materialVector) iter->GenerateVAO();

        GLBinder::BindArrayBuffer(l_lastArrayBuffer);
        GLBinder::BindVertexArray(l_lastVertexArray);
    }
}

float ROC::Geometry::GetBoundSphereRadius() const
{
    return m_boundSphereRaduis;
}
size_t ROC::Geometry::GetMaterialsCount() const
{
    return m_materialCount;
}
