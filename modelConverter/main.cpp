#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <bitset>
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "sajson.h"
#include "zlib.h"

struct Face
{
    int m_materialIndices[9];
};

int CompressData(void *f_src, int f_srcLen, void *f_dest, int f_destLen)
{
    z_stream zInfo = { 0 };
    zInfo.total_in = zInfo.avail_in = f_srcLen;
    zInfo.total_out = zInfo.avail_out = f_destLen;
    zInfo.next_in = static_cast<unsigned char*>(f_src);
    zInfo.next_out = static_cast<unsigned char*>(f_dest);

    int l_error, l_ret = -1;
    l_error = deflateInit(&zInfo, Z_DEFAULT_COMPRESSION);
    if(l_error == Z_OK)
    {
        l_error = deflate(&zInfo, Z_FINISH);
        if(l_error == Z_STREAM_END) l_ret = zInfo.total_out;
    }
    deflateEnd(&zInfo);
    return l_ret;
}
int UncompressData(void *f_src, int f_srcLen, void *f_dest, int f_destLen)
{
    z_stream zInfo = { 0 };
    zInfo.total_in = zInfo.avail_in = f_srcLen;
    zInfo.total_out = zInfo.avail_out = f_destLen;
    zInfo.next_in = static_cast<unsigned char*>(f_src);
    zInfo.next_out = static_cast<unsigned char*>(f_dest);

    int l_error, l_ret = -1;
    l_error = inflateInit(&zInfo);
    if(l_error == Z_OK)
    {
        l_error = inflate(&zInfo, Z_FINISH);
        if(l_error == Z_STREAM_END) l_ret = zInfo.total_out;
    }
    inflateEnd(&zInfo);
    return l_ret;
}
int GetMaxCompressedLen(int nLenSrc)
{
    int n16kBlocks = (nLenSrc + 16383) / 16384;
    return (nLenSrc + 6 + (n16kBlocks * 5));
}

#define Error(T) { std::cout << "Error: " << T << std::endl; return; }
#define Info(T) std::cout << "Info: " << T << std::endl

void ConvertJSON(std::string &f_path, std::string &f_out)
{
    std::ifstream l_inputFile;
    l_inputFile.open(f_path);
    if(l_inputFile.fail()) Error("Unable to open file");
    std::string l_data((std::istreambuf_iterator<char>(l_inputFile)), std::istreambuf_iterator<char>());
    l_inputFile.close();

    sajson::document l_document = sajson::parse(sajson::dynamic_allocation(), sajson::mutable_string_view(l_data.size(), const_cast<char*>(l_data.data())));
    size_t l_errorLine = l_document.get_error_line();
    if(l_errorLine) Error("JSON parsing error, line " << l_errorLine << ": " << l_document.get_error_message_as_string());
    sajson::value l_documentRoot = l_document.get_root();
    if(l_documentRoot.get_type() != sajson::TYPE_OBJECT) Error("Root node isn't an object");
    if(l_documentRoot.get_length() == 0U) Error("Root node is empty");

    std::ofstream l_file(f_out, std::ios::out | std::ios::binary);
    if(l_file.fail()) Error("Unable to create output file");
    l_file.write("ROC", 3U);
    unsigned char l_setter = 0x2U;
    l_file.write(reinterpret_cast<char*>(&l_setter), sizeof(unsigned char));

    int l_origSize, l_maxSize, l_compressedSized;
    std::vector<unsigned char> l_compressedData;

    size_t l_nodeIndex = l_documentRoot.find_object_key(sajson::literal("vertices"));
    if(l_nodeIndex == l_documentRoot.get_length()) Error("No vertex node");
    sajson::value l_vertexNode = l_documentRoot.get_object_value(l_nodeIndex);
    if(l_vertexNode.get_type() != sajson::TYPE_ARRAY) Error("Vertex node isn't an array");
    size_t l_verticesValue = l_vertexNode.get_length();
    if(l_verticesValue == 0U) Error("Vertex node is empty");
    Info(l_verticesValue / 3U << " vertices");

    std::vector<glm::vec3> l_vertexVector;
    for(size_t i = 0U; i < l_verticesValue; i += 3U)
    {
        glm::vec3 l_vertexPos;
        for(size_t j = 0U; j < 3U; j++)
        {
            sajson::value l_node = l_vertexNode.get_array_element(i + j);
            sajson::type l_nodeType = l_node.get_type();
            if(l_nodeType != sajson::TYPE_INTEGER && l_nodeType != sajson::TYPE_DOUBLE) Error(i + j << " vertex value isn't a number");
            l_vertexPos[j] = static_cast<float>((l_nodeType == sajson::TYPE_INTEGER) ? l_node.get_integer_value() : l_node.get_double_value());
        }
        l_vertexVector.push_back(l_vertexPos);
    }
    l_origSize = l_vertexVector.size()*sizeof(glm::vec3);
    l_maxSize = GetMaxCompressedLen(l_origSize);
    l_compressedData.resize(l_maxSize);
    l_compressedSized = CompressData(l_vertexVector.data(), l_origSize, l_compressedData.data(), l_maxSize);
    if(l_compressedSized == -1) Error("Unable to compress vertices");
    l_file.write(reinterpret_cast<char*>(&l_compressedSized), sizeof(int));
    l_file.write(reinterpret_cast<char*>(&l_origSize), sizeof(int));
    l_file.write(reinterpret_cast<char*>(l_compressedData.data()), l_compressedSized);

    l_nodeIndex = l_documentRoot.find_object_key(sajson::literal("uvs"));
    if(l_nodeIndex == l_documentRoot.get_length()) Error("No UVs node");
    sajson::value l_uvNodeArray = l_documentRoot.get_object_value(l_nodeIndex);
    if(l_uvNodeArray.get_type() != sajson::TYPE_ARRAY) Error("UVs node isn't an array");
    if(l_uvNodeArray.get_length() == 0U) Error("UVs node is empty");
    sajson::value l_uvNode = l_uvNodeArray.get_array_element(0U);
    size_t l_uvsValue = l_uvNode.get_length();
    if(l_uvsValue == 0U) Error("UVs subnode 0 is empty");
    Info(l_uvsValue / 2U << " UVs");

    std::vector<glm::vec2> l_uvVector;
    for(size_t i = 0U; i < l_uvsValue; i += 2U)
    {
        glm::vec2 l_uvPos;
        for(size_t j = 0U; j < 2U; j++)
        {
            sajson::value l_node = l_uvNode.get_array_element(i + j);
            sajson::type l_nodeType = l_node.get_type();
            if(l_nodeType != sajson::TYPE_INTEGER && l_nodeType != sajson::TYPE_DOUBLE) Error(i + j << " UV value isn't a number");
            l_uvPos[j] = static_cast<float>((l_nodeType == sajson::TYPE_INTEGER) ? l_node.get_integer_value() : l_node.get_double_value());
            if(j == 1U) l_uvPos[j] = 1.f - l_uvPos[j]; // Inverse V value
        }
        l_uvVector.push_back(l_uvPos);
    }
    l_origSize = l_uvVector.size()*sizeof(glm::vec2);
    l_maxSize = GetMaxCompressedLen(l_origSize);
    l_compressedData.resize(l_maxSize);
    l_compressedSized = CompressData(l_uvVector.data(), l_origSize, l_compressedData.data(), l_maxSize);
    if(l_compressedSized == -1) Error("Unable to compress UVs");
    l_file.write(reinterpret_cast<char*>(&l_compressedSized), sizeof(int));
    l_file.write(reinterpret_cast<char*>(&l_origSize), sizeof(int));
    l_file.write(reinterpret_cast<char*>(l_compressedData.data()), l_compressedSized);

    l_nodeIndex = l_documentRoot.find_object_key(sajson::literal("normals"));
    if(l_nodeIndex == l_documentRoot.get_length()) Error("No normal node");
    sajson::value l_normalNode = l_documentRoot.get_object_value(l_nodeIndex);
    if(l_normalNode.get_type() != sajson::TYPE_ARRAY) Error("Normal node isn't an array");
    size_t l_normalsValue = l_normalNode.get_length();
    if(l_normalsValue == 0U) Error("Normal node is empty");
    Info(l_normalsValue / 3U << " normals");

    std::vector<glm::vec3> l_normalVector;
    for(size_t i = 0U; i < l_normalsValue; i += 3U)
    {
        glm::vec3 l_normalDir;
        for(size_t j = 0U; j < 3U; j++)
        {
            sajson::value l_node = l_normalNode.get_array_element(i + j);
            sajson::type l_nodeType = l_node.get_type();
            if(l_nodeType != sajson::TYPE_INTEGER && l_nodeType != sajson::TYPE_DOUBLE) Error(i + j << " normal value isn't number");
            l_normalDir[j] = static_cast<float>((l_nodeType == sajson::TYPE_INTEGER) ? l_node.get_integer_value() : l_node.get_double_value());
        }
        l_normalVector.push_back(l_normalDir);
    }
    l_origSize = l_normalVector.size()*sizeof(glm::vec3);
    l_maxSize = GetMaxCompressedLen(l_origSize);
    l_compressedData.resize(l_maxSize);
    l_compressedSized = CompressData(l_normalVector.data(), l_origSize, l_compressedData.data(), l_maxSize);
    if(l_compressedSized == -1) Error("Unable to compress normals");
    l_file.write(reinterpret_cast<char*>(&l_compressedSized), sizeof(int));
    l_file.write(reinterpret_cast<char*>(&l_origSize), sizeof(int));
    l_file.write(reinterpret_cast<char*>(l_compressedData.data()), l_compressedSized);

    l_nodeIndex = l_documentRoot.find_object_key(sajson::literal("skinWeights"));
    if(l_nodeIndex == l_documentRoot.get_length()) Error("No weights node");
    sajson::value l_weightNode = l_documentRoot.get_object_value(l_nodeIndex);
    if(l_weightNode.get_type() != sajson::TYPE_ARRAY) Error("Weights node isn't an array");
    size_t l_weightsValue = l_weightNode.get_length();
    if(l_weightsValue == 0U) Error("Weights node is empty");
    Info(l_weightsValue / 4U << " weights");

    std::vector<glm::vec4> l_weightVector;
    for(size_t i = 0U; i < l_weightsValue; i += 4U)
    {
        glm::vec4 l_weightVal;
        for(size_t j = 0U; j < 4U; j++)
        {
            sajson::value l_node = l_weightNode.get_array_element(i + j);
            sajson::type l_nodeType = l_node.get_type();
            if(l_nodeType != sajson::TYPE_INTEGER && l_nodeType != sajson::TYPE_DOUBLE) Error(i + j << " weight value isn't number");
            l_weightVal[j] = static_cast<float>((l_nodeType == sajson::TYPE_INTEGER) ? l_node.get_integer_value() : l_node.get_double_value());
        }
        l_weightVector.push_back(l_weightVal);
    }
    l_origSize = l_weightVector.size()*sizeof(glm::vec4);
    l_maxSize = GetMaxCompressedLen(l_origSize);
    l_compressedData.resize(l_maxSize);
    l_compressedSized = CompressData(l_weightVector.data(), l_origSize, l_compressedData.data(), l_maxSize);
    if(l_compressedSized == -1) Error("Unable to compress weights");
    l_file.write(reinterpret_cast<char*>(&l_compressedSized), sizeof(int));
    l_file.write(reinterpret_cast<char*>(&l_origSize), sizeof(int));
    l_file.write(reinterpret_cast<char*>(l_compressedData.data()), l_compressedSized);

    l_nodeIndex = l_documentRoot.find_object_key(sajson::literal("skinIndices"));
    if(l_nodeIndex == l_documentRoot.get_length()) Error("No indices node");
    sajson::value l_indexNode = l_documentRoot.get_object_value(l_nodeIndex);
    if(l_indexNode.get_type() != sajson::TYPE_ARRAY) Error("Indices node isn't an array");
    size_t l_indicesValue = l_indexNode.get_length();
    if(l_indicesValue == 0U) Error("Indices node is empty");
    Info(l_indicesValue / 4U << " indices");

    std::vector<glm::ivec4> l_indexVector;
    for(size_t i = 0U; i < l_weightsValue; i += 4U)
    {
        glm::ivec4 l_indexVal;
        for(size_t j = 0U; j < 4U; j++)
        {
            sajson::value l_node = l_indexNode.get_array_element(i + j);
            if(l_node.get_type() != sajson::TYPE_INTEGER) Error(i + j << " index value isn't number");
            l_indexVal[j] = l_node.get_integer_value();
        }
        l_indexVector.push_back(l_indexVal);
    }
    l_origSize = l_indexVector.size()*sizeof(glm::ivec4);
    l_maxSize = GetMaxCompressedLen(l_origSize);
    l_compressedData.resize(l_maxSize);
    l_compressedSized = CompressData(l_indexVector.data(), l_origSize, l_compressedData.data(), l_maxSize);
    if(l_compressedSized == -1) Error("Unable to compress indices");
    l_file.write(reinterpret_cast<char*>(&l_compressedSized), sizeof(int));
    l_file.write(reinterpret_cast<char*>(&l_origSize), sizeof(int));
    l_file.write(reinterpret_cast<char*>(l_compressedData.data()), l_compressedSized);

    l_nodeIndex = l_documentRoot.find_object_key(sajson::literal("faces"));
    if(l_nodeIndex == l_documentRoot.get_length()) Error("No faces node");
    sajson::value l_faceNode = l_documentRoot.get_object_value(l_nodeIndex);
    if(l_faceNode.get_type() != sajson::TYPE_ARRAY) Error("Faces node isn't an array");
    size_t l_facesValue = l_faceNode.get_length();
    if(l_facesValue == 0U) Error("Faces node is empty");
    if(l_facesValue % 11U != 0U) Error("Wrong faces type. Be sure, that it's with normals, UVs, weights and indices");
    Info(l_facesValue / 11U << " faces");

    l_nodeIndex = l_documentRoot.find_object_key(sajson::literal("materials"));
    if(l_nodeIndex == l_documentRoot.get_length()) Error("No materials node");
    sajson::value l_materialsNode = l_documentRoot.get_object_value(l_nodeIndex);
    if(l_materialsNode.get_type() != sajson::TYPE_ARRAY) Error("Materials node isn't an array");
    size_t l_materialCount = l_materialsNode.get_length();
    if(l_materialCount == 0U) Error("Materials node is empty");
    Info(l_materialCount << " materials");

    int l_materialCountInt = static_cast<int>(l_materialCount);
    l_file.write(reinterpret_cast<char*>(&l_materialCountInt), sizeof(int));

    // Parse materials
    // THREE.js faces structure:
    //  42, [vertex_index, vertex_index, vertex_index],
    //  [material_index],
    //  [vertex_uv, vertex_uv, vertex_uv],
    //  [vertex_normal, vertex_normal, vertex_normal]
    for(size_t i = 0U; i < l_materialCount; i++)
    {
        std::vector<Face> l_facesVector;
        for(size_t j = 0U, jj = l_facesValue / 11U; j < jj; j++)
        {
            if(l_faceNode.get_array_element(j * 11U + 4U).get_integer_value() == i)
            {
                Face l_face;
                for(size_t k = 0; k < 3U; k++)
                {
                    sajson::value l_faceValueNode = l_faceNode.get_array_element(j * 11U + k + 1U);
                    if(l_faceValueNode.get_type() != sajson::TYPE_INTEGER) Error("Material " << i << ", face " << j * 11U + k + 1U << ", value " << k << " isn't number");
                    l_face.m_materialIndices[k] = l_faceValueNode.get_integer_value();
                }
                for(size_t k = 3U; k < 9U; k++)
                {
                    sajson::value l_faceValueNode = l_faceNode.get_array_element(j * 11U + k + 2U);
                    if(l_faceValueNode.get_type() != sajson::TYPE_INTEGER) Error("Material " << i << ", face " << j * 11U + k + 2U << ", value " << k << " isn't number");
                    l_face.m_materialIndices[k] = l_faceValueNode.get_integer_value();
                }
                l_facesVector.push_back(l_face);
            }
        }
        sajson::value m_node = l_materialsNode.get_array_element(i);
        if(m_node.get_type() != sajson::TYPE_OBJECT) Error("Material " << i << " node isn't an object");

        std::bitset<8U> l_materialBit;
        l_materialBit.set(0U, true);
        l_nodeIndex = m_node.find_object_key(sajson::literal("transparent"));
        if(l_nodeIndex == m_node.get_length()) l_materialBit.set(1U, true);
        else
        {
            sajson::value l_transparencyData = m_node.get_object_value(l_nodeIndex);
            size_t l_transparencyType = l_transparencyData.get_type();
            if(l_transparencyType != sajson::TYPE_FALSE && l_transparencyType != sajson::TYPE_TRUE) l_materialBit.set(1U, true);
            else
            {
                if(l_transparencyType == sajson::TYPE_FALSE) l_materialBit.set(1U, true);
                else l_materialBit.set(2U, true);
            }
        }
        l_nodeIndex = m_node.find_object_key(sajson::literal("doubleSided"));
        if(l_nodeIndex != m_node.get_length())
        {
            sajson::value l_dsData = m_node.get_object_value(l_nodeIndex);
            l_materialBit.set(3U, l_dsData.get_type() == sajson::TYPE_TRUE);
        }
        l_nodeIndex = m_node.find_object_key(sajson::literal("filtering"));
        if(l_nodeIndex != m_node.get_length())
        {
            sajson::value l_filterData = m_node.get_object_value(l_nodeIndex);
            if(l_filterData.get_type() == sajson::TYPE_INTEGER) l_materialBit.set(4U, l_filterData.get_integer_value() == 1);
        }
        l_nodeIndex = m_node.find_object_key(sajson::literal("compression"));
        if(l_nodeIndex != m_node.get_length())
        {
            sajson::value l_compData = m_node.get_object_value(l_nodeIndex);
            l_materialBit.set(5U, l_compData.get_type() == sajson::TYPE_TRUE);
        }

        std::string l_difMap;
        l_nodeIndex = m_node.find_object_key(sajson::literal("mapDiffuse"));
        if(l_nodeIndex != m_node.get_length())
        {
            sajson::value l_textureData = m_node.get_object_value(l_nodeIndex);
            if(l_textureData.get_type() == sajson::TYPE_STRING) l_difMap += ("textures/" + l_textureData.as_string());
        }

        l_setter = static_cast<unsigned char>(l_materialBit.to_ulong());
        l_file.write(reinterpret_cast<char*>(&l_setter), sizeof(unsigned char));
        glm::vec4 l_params(1.f);
        l_file.write(reinterpret_cast<char*>(&l_params), sizeof(glm::vec4));
        l_setter = l_difMap.length();
        l_file.write(reinterpret_cast<char*>(&l_setter), sizeof(unsigned char));
        if(l_setter) l_file.write(l_difMap.data(), l_setter);

        l_origSize = l_facesVector.size()*sizeof(Face);
        l_maxSize = GetMaxCompressedLen(l_origSize);
        l_compressedData.resize(l_maxSize);
        l_compressedSized = CompressData(l_facesVector.data(), l_origSize, l_compressedData.data(), l_maxSize);
        if(l_compressedSized == -1) Error("Unable to compress material " << i << " faces");
        l_file.write(reinterpret_cast<char*>(&l_compressedSized), sizeof(int));
        l_file.write(reinterpret_cast<char*>(&l_origSize), sizeof(int));
        l_file.write(reinterpret_cast<char*>(l_compressedData.data()), l_compressedSized);
        Info("Material " << i << ", " << l_facesVector.size() << " faces");
        l_facesVector.clear();
    }

    // Parse skeleton
    l_nodeIndex = l_documentRoot.find_object_key(sajson::literal("bones"));
    if(l_nodeIndex == l_documentRoot.get_length()) Error("No bones node");
    sajson::value l_bonesNode = l_documentRoot.get_object_value(l_nodeIndex);
    if(l_bonesNode.get_type() != sajson::TYPE_ARRAY) Error("Bones node isn't an array");
    size_t l_bonesCount = l_bonesNode.get_length();
    if(l_bonesCount == 0U) Error("Bones node is empty");
    Info(l_bonesCount << " bones");

    int l_bonesCountInt = static_cast<int>(l_bonesCount);
    l_file.write(reinterpret_cast<char*>(&l_bonesCountInt), sizeof(int));

    for(size_t i = 0U; i < l_bonesCount; i++)
    {
        sajson::value b_node = l_bonesNode.get_array_element(i);
        if(b_node.get_type() != sajson::TYPE_OBJECT) Error("Bone " << i << " node isn't an object");
        std::string l_name;
        l_nodeIndex = b_node.find_object_key(sajson::literal("name"));
        if(l_nodeIndex == b_node.get_length()) l_name += ("Bone_" + std::to_string(i));
        else
        {
            sajson::value l_boneNameData = b_node.get_object_value(l_nodeIndex);
            if(l_boneNameData.get_type() != sajson::TYPE_STRING) l_name += ("Bone_" + std::to_string(i));
            else l_name += l_boneNameData.as_string();
        }

        l_setter = l_name.length();
        l_file.write(reinterpret_cast<char*>(&l_setter), sizeof(unsigned char));
        if(l_setter) l_file.write(l_name.data(), l_setter);

        l_nodeIndex = b_node.find_object_key(sajson::literal("parent"));
        if(l_nodeIndex == b_node.get_length()) Error("No parent node for bone " << i);
        sajson::value l_node4 = b_node.get_object_value(l_nodeIndex);
        if(l_node4.get_type() != sajson::TYPE_INTEGER) Error("Parent node for bone " << i << " isn't an integer value");
        int l_parentBoneValue = l_node4.get_integer_value();
        if(l_parentBoneValue >= l_bonesCountInt || l_parentBoneValue < -1) Error("Wrong value of parent for bone " << i);
        l_file.write(reinterpret_cast<char*>(&l_parentBoneValue), sizeof(int));

        glm::vec3 l_pos;
        l_nodeIndex = b_node.find_object_key(sajson::literal("pos"));
        if(l_nodeIndex == b_node.get_length()) Error("No position node for bone " << i);
        sajson::value l_node2 = b_node.get_object_value(l_nodeIndex);
        if(l_node2.get_type() != sajson::TYPE_ARRAY) Error("Position node for bone " << i << " isn't an array");
        if(l_node2.get_length() != 3U) Error("Position node array size for bone " << i << " isn't equal 3");
        for(size_t j = 0U; j < 3U; j++)
        {
            sajson::value l_nodeData = l_node2.get_array_element(j);
            size_t l_nodeType = l_nodeData.get_type();
            if(l_nodeType != sajson::TYPE_INTEGER && l_nodeType != sajson::TYPE_DOUBLE) Error("Position value " << j << " for bone " << i << " isn't an number");
            l_pos[j] = static_cast<float>(l_nodeType == sajson::TYPE_INTEGER ? l_nodeData.get_integer_value() : l_nodeData.get_double_value());
        }
        l_file.write(reinterpret_cast<char*>(&l_pos), sizeof(glm::vec3));

        glm::quat l_rot;
        l_nodeIndex = b_node.find_object_key(sajson::literal("rotq"));
        if(l_nodeIndex == b_node.get_length()) Error("No rotation node for bone " << i);
        sajson::value l_node1 = b_node.get_object_value(l_nodeIndex);
        if(l_node1.get_type() != sajson::TYPE_ARRAY) Error("Rotation node for bone " << i << " isn't an array");
        if(l_node1.get_length() != 4U) Error("Rotation node array size for bone " << i << " isn't equal 4");
        for(size_t j = 0U; j < 4U; j++)
        {
            sajson::value l_nodeData = l_node1.get_array_element(j);
            size_t l_nodeType = l_nodeData.get_type();
            if(l_nodeType != sajson::TYPE_INTEGER && l_nodeType != sajson::TYPE_DOUBLE) Error("Rotation value " << j << " for bone " << i << " isn't an number");
            l_rot[j] = static_cast<float>(l_nodeType == sajson::TYPE_INTEGER ? l_nodeData.get_integer_value() : l_nodeData.get_double_value());
        }
        l_file.write(reinterpret_cast<char*>(&l_rot), sizeof(glm::quat));

        glm::vec3 l_scl;
        l_nodeIndex = b_node.find_object_key(sajson::literal("scl"));
        if(l_nodeIndex == b_node.get_length())
        {
            Info("No scale node for bone " << i << ". Set to default 1.0");
            l_scl = glm::vec3(1.f);
        }
        else
        {
            sajson::value l_node3 = b_node.get_object_value(l_nodeIndex);
            if(l_node3.get_type() != sajson::TYPE_ARRAY) Error("Scale node for bone " << i << " isn't an array");
            if(l_node3.get_length() != 3U) Error("Scale node array size for bone " << i << " isn't equal 3");
            for(size_t j = 0U; j < 3U; j++)
            {
                sajson::value l_nodeData = l_node3.get_array_element(j);
                size_t l_nodeType = l_nodeData.get_type();
                if(l_nodeType != sajson::TYPE_INTEGER && l_nodeType != sajson::TYPE_DOUBLE) Error("Scale value " << j << " for bone " << i << " isn't an number");
                l_scl[j] = static_cast<float>(l_nodeType == sajson::TYPE_INTEGER ? l_nodeData.get_integer_value() : l_nodeData.get_double_value());
            }
        }
        l_file.write(reinterpret_cast<char*>(&l_scl), sizeof(glm::vec3));
    }
    l_file.flush();
    l_file.close();
    Info("Model has been converted to " << f_out);
}

void ConvertOBJ(std::string &f_path, std::string &f_out)
{
    std::vector<glm::vec3> temp_vertex;
    std::vector<glm::vec2> temp_uv;
    std::vector<glm::vec3> temp_normal;

    std::vector<std::string> l_materialNames;
    std::vector<std::string> l_materialTextureNames;
    std::vector<unsigned char> l_materialTypes;
    int l_currentMaterial = -1;
    bool l_defaultMaterial = true;

    std::ifstream l_objectFile;
    l_objectFile.open(f_path + ".obj", std::ios::in);
    if(l_objectFile.fail()) Error("Unable to load input file");

    std::string l_buffer;

    std::ifstream l_materialFile;
    l_materialFile.open(f_path + ".mtl", std::ios::in);
    if(!l_materialFile.fail())
    {
        l_defaultMaterial = false;
        //Materials parsing
        while(std::getline(l_materialFile, l_buffer))
        {
            if(l_buffer.empty()) continue;
            if(l_buffer.at(0U) == 'N' || l_buffer.at(0U) == 'K' || l_buffer.at(0U) == 'd' || l_buffer.at(0U) == 'i') continue;
            if(l_buffer.find("newmtl ") != std::string::npos)
            {
                std::string l_materialName = l_buffer.substr(7U);
                l_materialNames.push_back(l_materialName);
                continue;
            }
            if(l_buffer.find("type ") != std::string::npos) // this is custom line, you have to insert it in file with value based on material bits
            {
                std::string l_typeString = l_buffer.substr(5U);
                l_materialTypes.push_back(static_cast<unsigned char>(std::stoi(l_typeString)));
                continue;
            }
            if(l_buffer.find("map_Kd ") != std::string::npos || l_buffer.find("map_kd ") != std::string::npos)
            {
                std::string l_textureName = l_buffer.substr(7U);
                l_textureName.insert(0U, "textures/");
                l_materialTextureNames.push_back(l_textureName);
                continue;
            }
        }
        l_materialFile.close();
    }
    else Info("No .mtl file, assumed that all materials are default");

    if(l_materialNames.size() == 0U)
    {
        l_materialNames.push_back("DefaultMaterial");
        l_currentMaterial = 0;
    }

    if(l_materialTextureNames.size() < l_materialNames.size())
    {
        for(size_t i = 0U, j = l_materialNames.size() - l_materialTextureNames.size(); i < j; i++)
        {
            l_materialTextureNames.push_back("");
        }
    }

    if(l_materialTypes.size() < l_materialNames.size())
    {
        for(size_t i = 0U, j = l_materialNames.size() - l_materialTypes.size(); i < j; i++)
        {
            l_materialTypes.push_back(3U);
        }
    }

    std::ofstream l_outputFile(f_out, std::ios::out | std::ios::binary);
    if(l_outputFile.fail()) Error("Unable to create output file");

    l_outputFile.write("ROC", 3U);
    unsigned char l_setter = 0x1U;
    l_outputFile.write(reinterpret_cast<char*>(&l_setter), sizeof(unsigned char));

    bool l_dataParsed = false;
    bool l_materialsSizeParsed = false;
    std::vector<Face> l_faceVector;
    std::vector<unsigned char> l_compressedData;

    while(std::getline(l_objectFile, l_buffer))
    {
        if(l_buffer.empty()) continue;
        if(l_buffer.at(0U) == '#' || l_buffer.at(0U) == 's' || l_buffer.at(0U) == 'm' || l_buffer.at(0U) == 'o') continue;
        if(l_buffer.find("v ") != std::string::npos)
        {
            std::stringstream l_vertexStream(l_buffer.substr(2U));
            glm::vec3 l_vert;
            l_vertexStream >> l_vert.x >> l_vert.y >> l_vert.z;
            temp_vertex.push_back(l_vert);
            continue;
        }
        if(l_buffer.find("vn ") != std::string::npos)
        {
            std::stringstream l_normalStream(l_buffer.substr(3U));
            glm::vec3 l_norm;
            l_normalStream >> l_norm.x >> l_norm.y >> l_norm.z;
            temp_normal.push_back(l_norm);
            continue;
        }
        if(l_buffer.find("vt ") != std::string::npos)
        {
            std::stringstream l_uvStream(l_buffer.substr(3U));
            glm::vec2 l_uv;
            l_uvStream >> l_uv.x >> l_uv.y;
            l_uv.y = 1.f - l_uv.y;
            temp_uv.push_back(l_uv);
            continue;
        }
        if(!l_dataParsed)
        {
            l_dataParsed = true;

            int l_origSize = temp_vertex.size()*sizeof(glm::vec3);
            int l_maxSize = GetMaxCompressedLen(l_origSize);
            l_compressedData.resize(l_maxSize);
            int l_compressedSize = CompressData(temp_vertex.data(), l_origSize, l_compressedData.data(), l_maxSize);
            if(l_compressedSize == -1) Error("Unable to compress vertices");
            l_outputFile.write(reinterpret_cast<char*>(&l_compressedSize), sizeof(int));
            l_outputFile.write(reinterpret_cast<char*>(&l_origSize), sizeof(int));
            l_outputFile.write(reinterpret_cast<char*>(l_compressedData.data()), l_compressedSize);
            Info(temp_vertex.size() << " vertices");

            l_origSize = temp_uv.size()*sizeof(glm::vec2);
            l_maxSize = GetMaxCompressedLen(l_origSize);
            l_compressedData.resize(l_maxSize);
            l_compressedSize = CompressData(temp_uv.data(), l_origSize, l_compressedData.data(), l_maxSize);
            if(l_compressedSize == -1) Error("Unable to compress UVs");
            l_outputFile.write(reinterpret_cast<char*>(&l_compressedSize), sizeof(int));
            l_outputFile.write(reinterpret_cast<char*>(&l_origSize), sizeof(int));
            l_outputFile.write(reinterpret_cast<char*>(l_compressedData.data()), l_compressedSize);
            Info(temp_vertex.size() << " UVs");

            l_origSize = temp_normal.size()*sizeof(glm::vec3);
            l_maxSize = GetMaxCompressedLen(l_origSize);
            l_compressedData.resize(l_maxSize);
            l_compressedSize = CompressData(temp_normal.data(), l_origSize, l_compressedData.data(), l_maxSize);
            if(l_compressedSize == -1) Error("Unable to compress normals");
            l_outputFile.write(reinterpret_cast<char*>(&l_compressedSize), sizeof(int));
            l_outputFile.write(reinterpret_cast<char*>(&l_origSize), sizeof(int));
            l_outputFile.write(reinterpret_cast<char*>(l_compressedData.data()), l_compressedSize);
            Info(temp_vertex.size() << " normals");
        }
        if(l_buffer.find("usemtl ") != std::string::npos)
        {
            if(!l_materialsSizeParsed)
            {
                int l_materialsSizeI = l_materialNames.size();
                l_outputFile.write(reinterpret_cast<char*>(&l_materialsSizeI), sizeof(int));
                l_materialsSizeParsed = true;
                Info(l_materialsSizeI << " material(s)");
            }
            if(l_defaultMaterial) continue;
            if(l_currentMaterial != -1)
            {
                l_setter = static_cast<unsigned char>(l_materialTypes[l_currentMaterial]);
                l_outputFile.write(reinterpret_cast<char*>(&l_setter), sizeof(unsigned char));
                glm::vec4 l_params(1.f);
                l_outputFile.write(reinterpret_cast<char*>(&l_params), sizeof(glm::vec4));
                l_setter = l_materialTextureNames[l_currentMaterial].size();
                l_outputFile.write(reinterpret_cast<char*>(&l_setter), sizeof(unsigned char));
                if(l_setter) l_outputFile.write(l_materialTextureNames[l_currentMaterial].data(), l_setter);

                int l_origSize = l_faceVector.size()*sizeof(Face);
                int l_maxSize = GetMaxCompressedLen(l_origSize);
                l_compressedData.resize(l_maxSize);
                int l_compressedSize = CompressData(l_faceVector.data(), l_origSize, l_compressedData.data(), l_maxSize);
                if(l_compressedSize == -1) Error("Unable to compress faces for material " << l_currentMaterial);
                l_outputFile.write(reinterpret_cast<char*>(&l_compressedSize), sizeof(int));
                l_outputFile.write(reinterpret_cast<char*>(&l_origSize), sizeof(int));
                l_outputFile.write(reinterpret_cast<char*>(l_compressedData.data()), l_compressedSize);
                Info("Material " << l_currentMaterial << ", " << l_faceVector.size() << " faces");
                l_faceVector.clear();
            }
            std::string l_textureName = l_buffer.substr(7U);
            l_currentMaterial = std::distance(l_materialNames.begin(), std::find(l_materialNames.begin(), l_materialNames.end(), l_textureName));
            if(l_currentMaterial >= int(l_materialNames.size()) || l_currentMaterial < 0) Error("Unable to parse materials");
            continue;
        }
        if(l_buffer.find("f ") != std::string::npos)
        {
            if(!l_materialsSizeParsed)
            {
                int l_materialsSizeI = l_materialNames.size();
                l_outputFile.write(reinterpret_cast<char*>(&l_materialsSizeI), sizeof(int));
                l_materialsSizeParsed = true;
                Info(l_materialsSizeI << " material(s)");
            }
            std::string l_faceString = l_buffer.substr(2U);
            std::replace(l_faceString.begin(), l_faceString.end(), '/', ' ');
            std::stringstream l_faceStream(l_faceString);
            Face l_face;
            l_faceStream >>
                l_face.m_materialIndices[0] >> l_face.m_materialIndices[3] >> l_face.m_materialIndices[6] >>
                l_face.m_materialIndices[1] >> l_face.m_materialIndices[4] >> l_face.m_materialIndices[7] >>
                l_face.m_materialIndices[2] >> l_face.m_materialIndices[5] >> l_face.m_materialIndices[8];

            for(int i = 0; i < 9; i++) l_face.m_materialIndices[i]--;
            l_faceVector.push_back(l_face);
            continue;
        }
    }
    if(l_currentMaterial != -1)
    {
        l_setter = static_cast<unsigned char>(l_materialTypes[l_currentMaterial]);
        l_outputFile.write(reinterpret_cast<char*>(&l_setter), sizeof(unsigned char));
        glm::vec4 l_params(1.f);
        l_outputFile.write(reinterpret_cast<char*>(&l_params), sizeof(glm::vec4));
        l_setter = l_materialTextureNames[l_currentMaterial].size();
        l_outputFile.write(reinterpret_cast<char*>(&l_setter), sizeof(unsigned char));
        if(l_setter) l_outputFile.write(l_materialTextureNames[l_currentMaterial].data(), l_setter);

        int l_origSize = l_faceVector.size()*sizeof(Face);
        int l_maxSize = GetMaxCompressedLen(l_origSize);
        l_compressedData.resize(l_maxSize);
        int l_compressedSize = CompressData(l_faceVector.data(), l_origSize, l_compressedData.data(), l_maxSize);
        if(l_compressedSize == -1) Error("Unable to compress faces for material " << l_currentMaterial);
        l_outputFile.write(reinterpret_cast<char*>(&l_compressedSize), sizeof(int));
        l_outputFile.write(reinterpret_cast<char*>(&l_origSize), sizeof(int));
        l_outputFile.write(reinterpret_cast<char*>(l_compressedData.data()), l_compressedSize);
        Info("Material " << l_currentMaterial << ", " << l_faceVector.size() << " faces");
        l_faceVector.clear();
    }
    l_objectFile.close();
    l_outputFile.flush();
    l_outputFile.close();

    l_compressedData.clear();

    Info("Model has been converted to " << f_out);
}

int main(int argc, char *argv[])
{
    if(argc > 1)
    {
        std::string l_inputFile(argv[1]);
        std::string l_outputFile;

        size_t l_searchResult = l_inputFile.find(std::string(".obj"));
        if(l_searchResult != std::string::npos)
        {
            Info("Converting OBJ ...");
            l_inputFile = l_inputFile.substr(0U, l_searchResult);
            l_outputFile.assign(l_inputFile);
            l_outputFile.append(".rmf");
            ConvertOBJ(l_inputFile, l_outputFile);
        }
        else
        {
            l_searchResult = l_inputFile.find(std::string(".json"));
            if(l_searchResult != std::string::npos)
            {
                Info("Converting THREE.js JSON (animated)...");
                l_outputFile = l_inputFile.substr(0U, l_searchResult);
                l_outputFile.append(".rmf");
                ConvertJSON(l_inputFile, l_outputFile);
            }
            else Info("Unknown format. Avaliable formats: obj, json (THREE.js animated)");
        }
    }
    else std::cout << "Usage: [input_file]" << std::endl;
    Info("Press any key to exit");
    std::getchar();
    return EXIT_SUCCESS;
}
