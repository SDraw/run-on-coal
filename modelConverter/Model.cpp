#include "stdafx.h"

#include "Model.h"
#include "Utils.h"

Model::Model()
{
    m_type = MT_Static;
    m_loaded = false;
}

Model::~Model()
{
    Clean();
}

void Model::Clean()
{
    m_vertexData.clear();
    m_uvData.clear();
    m_normalData.clear();
    m_weightData.clear();
    m_indexData.clear();
    m_materials.clear();
    m_bones.clear();

    m_type = MT_Static;
    m_loaded = false;
}

bool Model::LoadJSON(const std::string &f_path)
{
    if(!m_loaded)
    {
        std::cout << '\'' << f_path << "' parsing ..." << std::endl;
        try
        {
            m_type = MT_Animated;

            std::ifstream l_inputFile;
            l_inputFile.open(f_path);
            if(l_inputFile.fail()) throw std::exception("Unable to open file");
            std::string l_parseData((std::istreambuf_iterator<char>(l_inputFile)), std::istreambuf_iterator<char>());
            l_inputFile.close();

            sajson::document l_document = sajson::parse(sajson::dynamic_allocation(), sajson::mutable_string_view(l_parseData.size(), const_cast<char*>(l_parseData.data())));
            if(l_document.get_error_line()) throw std::exception("JSON parsing error");
            sajson::value l_documentRoot = l_document.get_root();
            if(l_documentRoot.get_type() != sajson::TYPE_OBJECT) throw std::exception("Root node isn't an object");
            if(l_documentRoot.get_length() == 0U) throw std::exception("Root node is empty");

            size_t l_nodeIndex;

            // Vertices
            l_nodeIndex = l_documentRoot.find_object_key(sajson::literal("vertices"));
            if(l_nodeIndex == l_documentRoot.get_length()) throw std::exception("No vertex node");
            sajson::value l_vertexNode = l_documentRoot.get_object_value(l_nodeIndex);
            if(l_vertexNode.get_type() != sajson::TYPE_ARRAY) throw std::exception("Vertex node isn't an array");
            size_t l_verticesValue = l_vertexNode.get_length();
            if(l_verticesValue == 0U) throw std::exception("Vertex node is empty");
            std::cout << l_verticesValue / 3U << " vertices" << std::endl;

            for(size_t i = 0U; i < l_verticesValue; i += 3U)
            {
                glm::vec3 l_vertexPos;
                for(size_t j = 0U; j < 3U; j++)
                {
                    sajson::value l_node = l_vertexNode.get_array_element(i + j);
                    sajson::type l_nodeType = l_node.get_type();
                    if(l_nodeType != sajson::TYPE_INTEGER && l_nodeType != sajson::TYPE_DOUBLE) throw std::exception("One vertex value isn't a number");
                    l_vertexPos[static_cast<int>(j)] = static_cast<float>((l_nodeType == sajson::TYPE_INTEGER) ? l_node.get_integer_value() : l_node.get_double_value());
                }
                m_vertexData.push_back(l_vertexPos);
            }

            // UVs
            l_nodeIndex = l_documentRoot.find_object_key(sajson::literal("uvs"));
            if(l_nodeIndex == l_documentRoot.get_length()) throw std::exception("No UVs node");
            sajson::value l_uvNodeArray = l_documentRoot.get_object_value(l_nodeIndex);
            if(l_uvNodeArray.get_type() != sajson::TYPE_ARRAY) throw std::exception("UVs node isn't an array");
            if(l_uvNodeArray.get_length() == 0U) throw std::exception("UVs node is empty");
            sajson::value l_uvNode = l_uvNodeArray.get_array_element(0U); // Only one UV map is supported currently
            size_t l_uvsValue = l_uvNode.get_length();
            if(l_uvsValue == 0U) throw std::exception("UVs subnode 0 is empty");
            std::cout << l_uvsValue / 2U << " UVs" << std::endl;

            for(size_t i = 0U; i < l_uvsValue; i += 2U)
            {
                glm::vec2 l_uvPos;
                for(size_t j = 0U; j < 2U; j++)
                {
                    sajson::value l_node = l_uvNode.get_array_element(i + j);
                    sajson::type l_nodeType = l_node.get_type();
                    if(l_nodeType != sajson::TYPE_INTEGER && l_nodeType != sajson::TYPE_DOUBLE) throw std::exception("One UV value isn't a number");
                    l_uvPos[static_cast<int>(j)] = static_cast<float>((l_nodeType == sajson::TYPE_INTEGER) ? l_node.get_integer_value() : l_node.get_double_value());
                    if(j == 1U) l_uvPos[static_cast<int>(j)] = 1.f - l_uvPos[static_cast<int>(j)]; // Inverse V value
                }
                m_uvData.push_back(l_uvPos);
            }

            // Normals
            l_nodeIndex = l_documentRoot.find_object_key(sajson::literal("normals"));
            if(l_nodeIndex == l_documentRoot.get_length()) throw std::exception("No normal node");
            sajson::value l_normalNode = l_documentRoot.get_object_value(l_nodeIndex);
            if(l_normalNode.get_type() != sajson::TYPE_ARRAY) throw std::exception("Normal node isn't an array");
            size_t l_normalsValue = l_normalNode.get_length();
            if(l_normalsValue == 0U) throw std::exception("Normal node is empty");
            std::cout << l_normalsValue / 3U << " normals" << std::endl;

            for(size_t i = 0U; i < l_normalsValue; i += 3U)
            {
                glm::vec3 l_normal;
                for(size_t j = 0U; j < 3U; j++)
                {
                    sajson::value l_node = l_normalNode.get_array_element(i + j);
                    sajson::type l_nodeType = l_node.get_type();
                    if(l_nodeType != sajson::TYPE_INTEGER && l_nodeType != sajson::TYPE_DOUBLE) throw std::exception("One normal value isn't a number");
                    l_normal[static_cast<int>(j)] = static_cast<float>((l_nodeType == sajson::TYPE_INTEGER) ? l_node.get_integer_value() : l_node.get_double_value());
                }
                m_normalData.push_back(l_normal);
            }

            // Weights
            l_nodeIndex = l_documentRoot.find_object_key(sajson::literal("skinWeights"));
            if(l_nodeIndex == l_documentRoot.get_length()) throw std::exception("No weights node");
            sajson::value l_weightNode = l_documentRoot.get_object_value(l_nodeIndex);
            if(l_weightNode.get_type() != sajson::TYPE_ARRAY) throw std::exception("Weights node isn't an array");
            size_t l_weightsValue = l_weightNode.get_length();
            if(l_weightsValue == 0U) throw std::exception("Weights node is empty");
            std::cout << l_weightsValue / 4U << " weights" << std::endl;

            for(size_t i = 0U; i < l_weightsValue; i += 4U)
            {
                glm::vec4 l_weight;
                for(size_t j = 0U; j < 4U; j++)
                {
                    sajson::value l_node = l_weightNode.get_array_element(i + j);
                    sajson::type l_nodeType = l_node.get_type();
                    if(l_nodeType != sajson::TYPE_INTEGER && l_nodeType != sajson::TYPE_DOUBLE) throw std::exception("One weight value isn't a number");
                    l_weight[static_cast<int>(j)] = static_cast<float>((l_nodeType == sajson::TYPE_INTEGER) ? l_node.get_integer_value() : l_node.get_double_value());
                }
                m_weightData.push_back(l_weight);
            }

            // Weight indices
            l_nodeIndex = l_documentRoot.find_object_key(sajson::literal("skinIndices"));
            if(l_nodeIndex == l_documentRoot.get_length()) throw std::exception("No indices node");
            sajson::value l_indexNode = l_documentRoot.get_object_value(l_nodeIndex);
            if(l_indexNode.get_type() != sajson::TYPE_ARRAY) throw std::exception("Indices node isn't an array");
            size_t l_indicesValue = l_indexNode.get_length();
            if(l_indicesValue == 0U) throw std::exception("Indices node is empty");
            std::cout << l_indicesValue / 4U << " indices" << std::endl;

            for(size_t i = 0U; i < l_indicesValue; i += 4U)
            {
                glm::ivec4 l_index;
                for(size_t j = 0U; j < 4U; j++)
                {
                    sajson::value l_node = l_indexNode.get_array_element(i + j);
                    if(l_node.get_type() != sajson::TYPE_INTEGER) throw std::exception("One weight index value isn't an integer number");
                    l_index[static_cast<int>(j)] = l_node.get_integer_value();
                }
                m_indexData.push_back(l_index);
            }

            l_nodeIndex = l_documentRoot.find_object_key(sajson::literal("faces"));
            if(l_nodeIndex == l_documentRoot.get_length()) throw std::exception("No faces node");
            sajson::value l_faceNode = l_documentRoot.get_object_value(l_nodeIndex);
            if(l_faceNode.get_type() != sajson::TYPE_ARRAY) throw std::exception("Faces node isn't an array");
            size_t l_facesValue = l_faceNode.get_length();
            if(l_facesValue == 0U) throw std::exception("Faces node is empty");
            if(l_facesValue % 11U != 0U) throw std::exception("Wrong faces type. Be sure, that faces are only triangulated, have normals, UVs, weights and indices");
            std::cout << l_facesValue / 11U << " faces" << std::endl;

            l_nodeIndex = l_documentRoot.find_object_key(sajson::literal("materials"));
            if(l_nodeIndex == l_documentRoot.get_length()) throw std::exception("No materials node");
            sajson::value l_materialsNode = l_documentRoot.get_object_value(l_nodeIndex);
            if(l_materialsNode.get_type() != sajson::TYPE_ARRAY) throw std::exception("Materials node isn't an array");
            size_t l_materialCount = l_materialsNode.get_length();
            if(l_materialCount == 0U) throw std::exception("Materials node is empty");
            std::cout << l_materialCount << " materials" << std::endl;

            std::cout << "Parsing materials ..." << std::endl;
            for(size_t i = 0U; i < l_materialCount; i++)
            {
                Material l_material;

                // Material faces
                for(size_t j = 0U; j < l_facesValue / 11U; j++)
                {
                    sajson::value l_faceMaterial = l_faceNode.get_array_element(j * 11U + 4U);
                    if(l_faceMaterial.get_type() != sajson::TYPE_INTEGER) throw std::exception("One face material index isn't an integer");
                    if(l_faceMaterial.get_integer_value() == i)
                    {
                        Face l_face;
                        for(size_t k = 0; k < 3U; k++)
                        {
                            sajson::value l_node = l_faceNode.get_array_element(j * 11U + k + 1U);
                            if(l_node.get_type() != sajson::TYPE_INTEGER) throw std::exception("One face value isn't a number");
                            l_face.m_materialIndex[k] = l_node.get_integer_value();
                        }
                        for(size_t k = 3U; k < 9U; k++)
                        {
                            sajson::value l_node = l_faceNode.get_array_element(j * 11U + k + 2U);
                            if(l_node.get_type() != sajson::TYPE_INTEGER) throw std::exception("One face value isn't a number");
                            l_face.m_materialIndex[k] = l_node.get_integer_value();
                        }
                        l_material.m_faces.push_back(l_face);
                    }
                }

                // Material properties
                sajson::value l_materialNode = l_materialsNode.get_array_element(i);
                if(l_materialNode.get_type() != sajson::TYPE_OBJECT) throw std::exception("Material node isn't an object");

                l_material.m_type = 0U;
                l_material.m_type.set(MTB_Shading, true);
                l_nodeIndex = l_materialNode.find_object_key(sajson::literal("transparent"));
                if(l_nodeIndex != l_materialNode.get_length())
                {
                    sajson::value l_node = l_materialNode.get_object_value(l_nodeIndex);
                    size_t l_transparencyType = l_node.get_type();
                    if(l_transparencyType != sajson::TYPE_FALSE && l_transparencyType != sajson::TYPE_TRUE) l_material.m_type.set(MTB_Depth, true);
                    else
                    {
                        if(l_transparencyType == sajson::TYPE_FALSE) l_material.m_type.set(MTB_Depth, true);
                        else l_material.m_type.set(MTB_Transparency, true);
                    }
                }
                else l_material.m_type.set(MTB_Depth, true);

                l_nodeIndex = l_materialNode.find_object_key(sajson::literal("doubleSided"));
                if(l_nodeIndex != l_materialNode.get_length())
                {
                    sajson::value l_node = l_materialNode.get_object_value(l_nodeIndex);
                    l_material.m_type.set(MTB_Doubleside, l_node.get_type() == sajson::TYPE_TRUE);
                }

                l_nodeIndex = l_materialNode.find_object_key(sajson::literal("filtering"));
                if(l_nodeIndex != l_materialNode.get_length())
                {
                    sajson::value l_node = l_materialNode.get_object_value(l_nodeIndex);
                    if(l_node.get_type() == sajson::TYPE_INTEGER) l_material.m_type.set(MTB_Filtering, l_node.get_integer_value() == 1);
                }

                l_nodeIndex = l_materialNode.find_object_key(sajson::literal("compression"));
                if(l_nodeIndex != l_materialNode.get_length())
                {
                    sajson::value l_node = l_materialNode.get_object_value(l_nodeIndex);
                    l_material.m_type.set(MTB_Compression, l_node.get_type() == sajson::TYPE_TRUE);
                }

                l_nodeIndex = l_materialNode.find_object_key(sajson::literal("mapDiffuse"));
                if(l_nodeIndex != l_materialNode.get_length())
                {
                    sajson::value l_node = l_materialNode.get_object_value(l_nodeIndex);
                    if(l_node.get_type() == sajson::TYPE_STRING) l_material.m_diffuseTexture.assign("textures/" + l_node.as_string());
                }

                l_material.m_params = glm::vec4(1.f);

                m_materials.push_back(l_material);
            }

            // Skeleton
            l_nodeIndex = l_documentRoot.find_object_key(sajson::literal("bones"));
            if(l_nodeIndex == l_documentRoot.get_length()) throw std::exception("No bones node");
            sajson::value l_bonesNode = l_documentRoot.get_object_value(l_nodeIndex);
            if(l_bonesNode.get_type() != sajson::TYPE_ARRAY) throw std::exception("Bones node isn't an array");
            size_t l_bonesCount = l_bonesNode.get_length();
            if(l_bonesCount == 0U) throw std::exception("Bones node is empty");
            std::cout << l_bonesCount << " bones" << std::endl;

            std::cout << "Parsing bones ..." << std::endl;
            for(size_t i = 0U; i < l_bonesCount; i++)
            {
                Bone l_bone;

                // Name
                sajson::value b_node = l_bonesNode.get_array_element(i);
                if(b_node.get_type() != sajson::TYPE_OBJECT) throw std::exception("Bone node isn't an object");
                l_nodeIndex = b_node.find_object_key(sajson::literal("name"));
                if(l_nodeIndex == b_node.get_length()) l_bone.m_name = ("Bone_" + std::to_string(i));
                else
                {
                    sajson::value l_node = b_node.get_object_value(l_nodeIndex);
                    if(l_node.get_type() != sajson::TYPE_STRING) l_bone.m_name = ("Bone_" + std::to_string(i));
                    else l_bone.m_name = l_node.as_string();
                }

                // Parent
                l_nodeIndex = b_node.find_object_key(sajson::literal("parent"));
                if(l_nodeIndex == b_node.get_length()) throw std::exception("No parent node");
                sajson::value l_parentNode = b_node.get_object_value(l_nodeIndex);
                if(l_parentNode.get_type() != sajson::TYPE_INTEGER) throw std::exception("Parent node for bone isn't an integer value");
                int l_parentBone = l_parentNode.get_integer_value();
                if(l_parentBone >= static_cast<int>(l_bonesCount) || l_parentBone < -1) throw std::exception("Wrong value of parent bone");
                l_bone.m_parent = l_parentBone;

                // Position
                l_nodeIndex = b_node.find_object_key(sajson::literal("pos"));
                if(l_nodeIndex == b_node.get_length()) throw std::exception("No position node");
                sajson::value l_posNode = b_node.get_object_value(l_nodeIndex);
                if(l_posNode.get_type() != sajson::TYPE_ARRAY) throw std::exception("Position node isn't an array");
                if(l_posNode.get_length() != 3U) throw std::exception("Position node array size isn't equal 3");
                for(size_t j = 0U; j < 3U; j++)
                {
                    sajson::value l_node = l_posNode.get_array_element(j);
                    size_t l_nodeType = l_node.get_type();
                    if(l_nodeType != sajson::TYPE_INTEGER && l_nodeType != sajson::TYPE_DOUBLE) throw std::exception("One bone position value isn't a number");
                    l_bone.m_position[static_cast<int>(j)] = static_cast<float>(l_nodeType == sajson::TYPE_INTEGER ? l_node.get_integer_value() : l_node.get_double_value());
                }

                // Rotation
                l_nodeIndex = b_node.find_object_key(sajson::literal("rotq"));
                if(l_nodeIndex == b_node.get_length()) throw std::exception("No rotation node");
                sajson::value l_rotNode = b_node.get_object_value(l_nodeIndex);
                if(l_rotNode.get_type() != sajson::TYPE_ARRAY) throw std::exception("Rotation node isn't an array");
                if(l_rotNode.get_length() != 4U) throw std::exception("Rotation node array isn't equal 4");
                for(size_t j = 0U; j < 4U; j++)
                {
                    sajson::value l_node = l_rotNode.get_array_element(j);
                    size_t l_nodeType = l_node.get_type();
                    if(l_nodeType != sajson::TYPE_INTEGER && l_nodeType != sajson::TYPE_DOUBLE) throw std::exception("One bone rotation value isn't a number");
                    l_bone.m_rotation[static_cast<int>(j)] = static_cast<float>(l_nodeType == sajson::TYPE_INTEGER ? l_node.get_integer_value() : l_node.get_double_value());
                }

                // Scale
                l_nodeIndex = b_node.find_object_key(sajson::literal("scl"));
                if(l_nodeIndex != b_node.get_length())
                {
                    sajson::value l_sclNode = b_node.get_object_value(l_nodeIndex);
                    if(l_sclNode.get_type() != sajson::TYPE_ARRAY) throw std::exception("Scale node isn't an array");
                    if(l_sclNode.get_length() != 3U) throw std::exception("Scale node array size isn't equal 3");
                    for(size_t j = 0U; j < 3U; j++)
                    {
                        sajson::value l_node = l_sclNode.get_array_element(j);
                        size_t l_nodeType = l_node.get_type();
                        if(l_nodeType != sajson::TYPE_INTEGER && l_nodeType != sajson::TYPE_DOUBLE) throw std::exception("One scale value isn't a number");
                        l_bone.m_scale[static_cast<int>(j)] = static_cast<float>(l_nodeType == sajson::TYPE_INTEGER ? l_node.get_integer_value() : l_node.get_double_value());
                    }
                }
                else l_bone.m_scale = glm::vec3(1.f);

                m_bones.push_back(l_bone);
            }

            m_loaded = true;
        }
        catch(const std::exception &e)
        {
            std::cout << "Error: " << e.what() << std::endl;
            Clean();
        }
    }
    return m_loaded;
}

bool Model::LoadOBJ(const std::string &f_path)
{
    if(!m_loaded)
    {
        std::cout << '\'' << f_path << "' parsing ..." << std::endl;
        try
        {
            m_type = MT_Static;

            std::ifstream l_objFile;
            l_objFile.open(f_path, std::ios::in);
            if(l_objFile.fail()) throw std::exception("Unable to load input file");

            std::string l_buffer;

            // Find MTL file
            std::string l_mtlPath;
            while(std::getline(l_objFile, l_buffer))
            {
                if(l_buffer.empty()) continue;
                if(l_buffer.find("mtllib ") == 0U)
                {
                    size_t l_pos = f_path.find_last_of("\\/");
                    if(l_pos != std::string::npos)
                    {
                        l_mtlPath = f_path.substr(0U, l_pos);
                        l_mtlPath.push_back('/');
                    }
                    l_mtlPath.append(l_buffer.substr(7U));
                    break;
                }
            }

            std::map<std::string, std::string> l_mtlMaterialTexture; // Material name <-> Texture path
            if(!l_mtlPath.empty())
            {
                std::cout << "Parsing '" << l_mtlPath << '\'' << std::endl;

                std::ifstream l_mtlFile;
                l_mtlFile.open(l_mtlPath, std::ios::in);
                if(l_mtlFile.fail()) throw std::exception("Unable to load MTL library file");

                std::pair<std::string, std::string> l_mtlMaterial;
                while(std::getline(l_mtlFile, l_buffer))
                {
                    if(l_buffer.empty()) continue;
                    if(l_buffer.find("newmtl ") == 0U)
                    {
                        l_mtlMaterial.first.assign(l_buffer.substr(7U));
                        continue;
                    }
                    if(l_buffer.find("map_Kd ") == 0U)
                    {
                        l_mtlMaterial.second.assign(l_buffer.substr(7U));

                        l_mtlMaterialTexture.insert(l_mtlMaterial);
                        l_mtlMaterial.first.clear();
                        l_mtlMaterial.second.clear();
                        continue;
                    }
                }

                std::cout << l_mtlMaterialTexture.size() << " material(s) in MTL library" << std::endl;
            }
            else std::cout << "Warning: No MTL file, all materials are assumed to be default" << std::endl;

            // Fill data
            l_objFile.clear();
            l_objFile.seekg(0U, std::ios::beg);
            while(std::getline(l_objFile, l_buffer))
            {
                if(l_buffer.empty()) continue;
                if(l_buffer.find("v ") == 0U)
                {
                    std::stringstream l_stream(l_buffer.substr(2U));
                    glm::vec3 l_vert;
                    l_stream >> l_vert.x >> l_vert.y >> l_vert.z;
                    if(l_stream.fail()) throw std::exception("Invalid vertex line");
                    m_vertexData.push_back(l_vert);
                    continue;
                }
                if(l_buffer.find("vn ") == 0U)
                {
                    std::stringstream l_stream(l_buffer.substr(3U));
                    glm::vec3 l_norm;
                    l_stream >> l_norm.x >> l_norm.y >> l_norm.z;
                    if(l_stream.fail()) throw std::exception("Invalid normal line");
                    m_normalData.push_back(l_norm);
                    continue;
                }
                if(l_buffer.find("vt ") == 0U)
                {
                    std::stringstream l_stream(l_buffer.substr(3U));
                    glm::vec2 l_uv;
                    l_stream >> l_uv.x >> l_uv.y;
                    if(l_stream.fail()) throw std::exception("Invalid UV line");
                    l_uv.y = 1.f - l_uv.y;
                    m_uvData.push_back(l_uv);
                    continue;
                }
            }

            // Parse material faces
            l_objFile.clear();
            l_objFile.seekg(0U, std::ios::beg);
            Material *l_material = nullptr;
            while(std::getline(l_objFile, l_buffer))
            {
                if(l_buffer.empty()) continue;
                if(l_buffer.find("usemtl ") == 0U)
                {
                    if(l_material)
                    {
                        m_materials.push_back(*l_material);
                        delete l_material;
                    }
                    l_material = new Material();
                    l_material->m_type.set(MTB_Shading, true);
                    l_material->m_type.set(MTB_Depth, true);

                    auto iter = l_mtlMaterialTexture.find(l_buffer.substr(7U));
                    if(iter != l_mtlMaterialTexture.end())
                    {
                        l_material->m_diffuseTexture.assign("textures/");
                        l_material->m_diffuseTexture.append(iter->second);
                    }
                    else std::cout << "Warning: Unknown material, no texture will be applied" << std::endl;

                    continue;
                }
                if(l_buffer.find("f ") == 0U)
                {
                    if(!l_material) throw std::exception("No material corresponds to faces");
                    std::string l_faceString = l_buffer.substr(2U);
                    std::replace(l_faceString.begin(), l_faceString.end(), '/', ' ');
                    std::stringstream l_stream(l_faceString);

                    Face l_face;
                    l_stream >>
                        l_face.m_materialIndex[0] >> l_face.m_materialIndex[3] >> l_face.m_materialIndex[6] >>
                        l_face.m_materialIndex[1] >> l_face.m_materialIndex[4] >> l_face.m_materialIndex[7] >>
                        l_face.m_materialIndex[2] >> l_face.m_materialIndex[5] >> l_face.m_materialIndex[8];
                    if(l_stream.fail()) throw std::exception("Invalid face line");
                    for(size_t i = 0U; i < 9U; i++) l_face.m_materialIndex[i]--;
                    l_material->m_faces.push_back(l_face);
                    continue;
                }
            }
            if(l_material)
            {
                m_materials.push_back(*l_material);
                delete l_material;
            }

            m_loaded = true;

            std::cout << m_vertexData.size() << " vertices" << std::endl;
            std::cout << m_normalData.size() << " normals" << std::endl;
            std::cout << m_uvData.size() << " UVs" << std::endl;
            std::cout << m_materials.size() << " material(s)" << std::endl;
        }
        catch(const std::exception &e)
        {
            std::cout << "Error: " << e.what() << std::endl;
            Clean();
        }
    }
    return m_loaded;
}

bool Model::Generate(const std::string &f_path)
{
    bool l_result = false;
    if(m_loaded)
    {
        try
        {
            std::ofstream l_file(f_path, std::ios::out | std::ios::binary);
            if(l_file.fail()) throw std::exception("Unable to create output file");

            l_file.write("ROC", 3U); // Header
            l_file.write(reinterpret_cast<char*>(&m_type), sizeof(unsigned char)); // Type

            glm::vec3 l_farthestPoint(0.f);
            for(const auto &iter : m_vertexData) l_farthestPoint = glm::max(l_farthestPoint, iter);
            float l_geometryRadius = glm::length(l_farthestPoint);
            l_file.write(reinterpret_cast<char*>(&l_geometryRadius), sizeof(float)); // Geometry sphere radius

            unsigned int l_materialCount = static_cast<unsigned int>(m_materials.size());
            l_file.write(reinterpret_cast<char*>(&l_materialCount), sizeof(unsigned int)); // Materials count

            for(size_t i = 0, j = m_materials.size(); i < j; i++)
            {
                const Material &l_material = m_materials[i];
                unsigned char l_materialType = static_cast<unsigned char>(l_material.m_type.to_ulong());
                l_file.write(reinterpret_cast<char*>(&l_materialType), sizeof(unsigned char)); // Material type
                l_file.write(reinterpret_cast<const char*>(&l_material.m_params), sizeof(glm::vec4)); // Material params

                unsigned char l_textureNameLength = static_cast<unsigned char>(m_materials[i].m_diffuseTexture.length());
                l_file.write(reinterpret_cast<char*>(&l_textureNameLength), sizeof(unsigned char)); // Diffuse texture path length
                if(l_textureNameLength > 0U) l_file.write(l_material.m_diffuseTexture.c_str(), l_textureNameLength); // Diffuse texture path

                // Materials
                std::vector<glm::vec3> l_materialVertices;
                std::vector<glm::vec3> l_materialNormals;
                std::vector<glm::vec2> l_materialUVs;
                std::vector<glm::vec4> l_materialWeights; // Only for MT_Animated
                std::vector<glm::ivec4> l_materialIndices; // Only for MT_Animated

                for(const auto &iter : l_material.m_faces)
                {
                    l_materialVertices.push_back(m_vertexData[iter.m_materialIndex[0U]]);
                    l_materialVertices.push_back(m_vertexData[iter.m_materialIndex[1U]]);
                    l_materialVertices.push_back(m_vertexData[iter.m_materialIndex[2U]]);
                    l_materialNormals.push_back(m_normalData[iter.m_materialIndex[6U]]);
                    l_materialNormals.push_back(m_normalData[iter.m_materialIndex[7U]]);
                    l_materialNormals.push_back(m_normalData[iter.m_materialIndex[8U]]);
                    l_materialUVs.push_back(m_uvData[iter.m_materialIndex[3U]]);
                    l_materialUVs.push_back(m_uvData[iter.m_materialIndex[4U]]);
                    l_materialUVs.push_back(m_uvData[iter.m_materialIndex[5U]]);

                    if(m_type == MT_Animated)
                    {
                        l_materialWeights.push_back(m_weightData[iter.m_materialIndex[0U]]);
                        l_materialWeights.push_back(m_weightData[iter.m_materialIndex[1U]]);
                        l_materialWeights.push_back(m_weightData[iter.m_materialIndex[2U]]);
                        l_materialIndices.push_back(m_indexData[iter.m_materialIndex[0U]]);
                        l_materialIndices.push_back(m_indexData[iter.m_materialIndex[1U]]);
                        l_materialIndices.push_back(m_indexData[iter.m_materialIndex[2U]]);
                    }
                }

                unsigned int l_sourceSize, l_possibleCompressionSize, l_compressedSize;
                std::vector<char> l_compressedData;

                // Vertices
                l_sourceSize = static_cast<unsigned int>(l_materialVertices.size()*sizeof(glm::vec3));
                l_possibleCompressionSize = GetMaxCompressedLen(l_sourceSize);
                l_compressedData.resize(l_possibleCompressionSize, '\0');
                l_compressedSize = CompressData(l_materialVertices.data(), l_sourceSize, l_compressedData.data(), l_possibleCompressionSize);
                if(l_compressedSize == 0U) throw std::exception("Unable to compress material vertex data");
                l_file.write(reinterpret_cast<char*>(&l_compressedSize), sizeof(unsigned int)); // Compressed size
                l_file.write(reinterpret_cast<char*>(&l_sourceSize), sizeof(unsigned int)); // Original size
                l_file.write(l_compressedData.data(), l_compressedSize); // Compressed data

                // Normals
                l_sourceSize = static_cast<unsigned int>(l_materialNormals.size()*sizeof(glm::vec3));
                l_possibleCompressionSize = GetMaxCompressedLen(l_sourceSize);
                l_compressedData.resize(l_possibleCompressionSize, '\0');
                l_compressedSize = CompressData(l_materialNormals.data(), l_sourceSize, l_compressedData.data(), l_possibleCompressionSize);
                if(l_compressedSize == 0U) throw std::exception("Unable to compress material normal data");
                l_file.write(reinterpret_cast<char*>(&l_compressedSize), sizeof(unsigned int)); // Compressed size
                l_file.write(reinterpret_cast<char*>(&l_sourceSize), sizeof(unsigned int)); // Original size
                l_file.write(l_compressedData.data(), l_compressedSize); // Compressed data

                // UVs
                l_sourceSize = static_cast<unsigned int>(l_materialUVs.size()*sizeof(glm::vec2));
                l_possibleCompressionSize = GetMaxCompressedLen(l_sourceSize);
                l_compressedData.resize(l_possibleCompressionSize, '\0');
                l_compressedSize = CompressData(l_materialUVs.data(), l_sourceSize, l_compressedData.data(), l_possibleCompressionSize);
                if(l_compressedSize == 0U) throw std::exception("Unable to compress material UV data");
                l_file.write(reinterpret_cast<char*>(&l_compressedSize), sizeof(unsigned int)); // Compressed size
                l_file.write(reinterpret_cast<char*>(&l_sourceSize), sizeof(unsigned int)); // Original size
                l_file.write(l_compressedData.data(), l_compressedSize); // Compressed data

                if(m_type == MT_Animated)
                {
                    // Weights
                    l_sourceSize = static_cast<unsigned int>(l_materialWeights.size()*sizeof(glm::vec4));
                    l_possibleCompressionSize = GetMaxCompressedLen(l_sourceSize);
                    l_compressedData.resize(l_possibleCompressionSize, '\0');
                    l_compressedSize = CompressData(l_materialWeights.data(), l_sourceSize, l_compressedData.data(), l_possibleCompressionSize);
                    if(l_compressedSize == 0U) throw std::exception("Unable to compress material weight data");
                    l_file.write(reinterpret_cast<char*>(&l_compressedSize), sizeof(unsigned int)); // Compressed size
                    l_file.write(reinterpret_cast<char*>(&l_sourceSize), sizeof(unsigned int)); // Original size
                    l_file.write(l_compressedData.data(), l_compressedSize); // Compressed data

                    // Weight indices
                    l_sourceSize = static_cast<unsigned int>(l_materialIndices.size()*sizeof(glm::ivec4));
                    l_possibleCompressionSize = GetMaxCompressedLen(l_sourceSize);
                    l_compressedData.resize(l_possibleCompressionSize, '\0');
                    l_compressedSize = CompressData(l_materialIndices.data(), l_sourceSize, l_compressedData.data(), l_possibleCompressionSize);
                    if(l_compressedSize == 0U) throw std::exception("Unable to compress material weight index data");
                    l_file.write(reinterpret_cast<char*>(&l_compressedSize), sizeof(unsigned int)); // Compressed size
                    l_file.write(reinterpret_cast<char*>(&l_sourceSize), sizeof(unsigned int)); // Original size
                    l_file.write(l_compressedData.data(), l_compressedSize); // Compressed data
                }
            }

            // Skeleton
            if(m_type == MT_Animated)
            {
                unsigned int l_bonesCount = static_cast<unsigned int>(m_bones.size());
                l_file.write(reinterpret_cast<char*>(&l_bonesCount), sizeof(unsigned int)); // Bones count

                for(size_t i = 0U, j = m_bones.size(); i < j; i++)
                {
                    const Bone &l_bone = m_bones[i];

                    unsigned char l_boneNameLength = static_cast<unsigned char>(l_bone.m_name.length());
                    l_file.write(reinterpret_cast<char*>(&l_boneNameLength), sizeof(unsigned char)); // Bone name length
                    if(l_boneNameLength > 0U) l_file.write(l_bone.m_name.c_str(), l_boneNameLength); // Bone name

                    l_file.write(reinterpret_cast<const char*>(&l_bone.m_parent), sizeof(int)); // Parent
                    l_file.write(reinterpret_cast<const char*>(&l_bone.m_position), sizeof(glm::vec3)); // Position
                    l_file.write(reinterpret_cast<const char*>(&l_bone.m_rotation), sizeof(glm::quat)); // Rotation
                    l_file.write(reinterpret_cast<const char*>(&l_bone.m_scale), sizeof(glm::vec3)); // Scale
                }
            }

            l_result = true;
        }
        catch(const std::exception &e)
        {
            std::cout << "Error: " << e.what() << std::endl;
            std::remove(f_path.c_str());
        }

        std::cout << "Generation result: " << (l_result ? "Success" : "Fail") << std::endl;
    }

    return l_result;
}
