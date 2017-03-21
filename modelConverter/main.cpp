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

int ReadEnumString(std::string &f_val, const std::string &f_enum)
{
    size_t first = f_enum.find(f_val);
    if(first == std::string::npos) return -1;
    return std::count(f_enum.begin(), f_enum.begin() + first, ',');
}

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
bool ReadFile(std::string &path, std::string &f_cont)
{
    std::ifstream l_file;
    l_file.open(path, std::ios::in);
    if(l_file.fail()) return false;

    std::ostringstream l_data;
    l_data << l_file.rdbuf();
    l_file.close();
    f_cont.append(l_data.str());
    return true;
}

#define Error(T) { std::cout << "Error: " << T << std::endl; std::getchar(); return; }
#define Info(T) std::cout << "Info: " << T << std::endl

void ConvertJSON(std::string &f_path, std::string &f_out)
{
    std::string l_data;
    if(!ReadFile(f_path, l_data)) Error("Unable to read " << f_path);
    sajson::document l_document = sajson::parse(sajson::literal(l_data.c_str()));
    if(l_document.get_error_line()) Error("JSON parsing error: " << l_document.get_error_message());
    sajson::value l_documentRoot = l_document.get_root();
    if(l_documentRoot.get_type() != sajson::TYPE_OBJECT) Error("Root node isn't an object");
    if(l_documentRoot.get_length() == 0) Error("Root node is empty");

    std::ofstream l_file(f_out, std::ios::out | std::ios::binary);
    if(l_file.fail()) Error("Unable to create output file");
    l_file.write("ROC", 3);
    unsigned char l_setter = 0x2;
    l_file.write(reinterpret_cast<char*>(&l_setter), sizeof(unsigned char));

    size_t l_facesValue = 0;

    size_t l_nodeIndex = l_documentRoot.find_object_key(sajson::literal("vertices"));
    if(l_nodeIndex == l_documentRoot.get_length()) Error("No vertex node");
    sajson::value l_vertexNode = l_documentRoot.get_object_value(l_nodeIndex);
    if(l_vertexNode.get_type() != sajson::TYPE_ARRAY) Error("Vertex node isn't an array");
    size_t l_verticesValue = l_vertexNode.get_length();
    if(l_verticesValue == 0) Error("Vertex node is empty");
    Info(l_verticesValue / 3 << " vertices");

    std::vector<glm::vec3> l_vertexVector;
    for(size_t i = 0; i < l_verticesValue; i += 3)
    {
        sajson::value l_node1 = l_vertexNode.get_array_element(i);
        sajson::type l_node1Type = l_node1.get_type();
        if(l_node1Type != sajson::TYPE_INTEGER && l_node1Type != sajson::TYPE_DOUBLE) Error(i << " vertex value isn't a number");
        sajson::value l_node2 = l_vertexNode.get_array_element(i + 1);
        sajson::type l_node2Type = l_node2.get_type();
        if(l_node2Type != sajson::TYPE_INTEGER && l_node2Type != sajson::TYPE_DOUBLE) Error(i + 1 << " vertex value isn't a number");
        sajson::value l_node3 = l_vertexNode.get_array_element(i + 2);
        sajson::type l_node3Type = l_node3.get_type();
        if(l_node3Type != sajson::TYPE_INTEGER && l_node3Type != sajson::TYPE_DOUBLE) Error(i + 2 << " vertex value isn't a number");
        l_vertexVector.push_back(glm::vec3(
            (l_node1Type == sajson::TYPE_INTEGER) ? float(l_node1.get_integer_value()) : float(l_node1.get_double_value()),
            (l_node2Type == sajson::TYPE_INTEGER) ? float(l_node2.get_integer_value()) : float(l_node2.get_double_value()),
            (l_node3Type == sajson::TYPE_INTEGER) ? float(l_node3.get_integer_value()) : float(l_node3.get_double_value())
            ));
    }
    int l_origSize = l_vertexVector.size()*sizeof(glm::vec3);
    int l_maxSize = GetMaxCompressedLen(l_origSize);
    unsigned char *l_compressedData = new unsigned char[l_maxSize];
    int l_compressedSized = CompressData(l_vertexVector.data(), l_origSize, l_compressedData, l_maxSize);
    if(l_compressedSized == -1) Error("Unable to compress vertices");
    l_file.write(reinterpret_cast<char*>(&l_compressedSized), sizeof(int));
    l_file.write(reinterpret_cast<char*>(&l_origSize), sizeof(int));
    l_file.write(reinterpret_cast<char*>(l_compressedData), l_compressedSized);
    delete[]l_compressedData;
    Info("Vertices data compressed from " << l_origSize << " to " << l_compressedSized << " bytes");

    l_nodeIndex = l_documentRoot.find_object_key(sajson::literal("uvs"));
    if(l_nodeIndex == l_documentRoot.get_length()) Error("No UVs node");
    if(l_documentRoot.get_object_value(l_nodeIndex).get_type() != sajson::TYPE_ARRAY) Error("UVs node isn't an array");
    if(l_documentRoot.get_object_value(l_nodeIndex).get_length() == 0) Error("UVs node is empty");
    sajson::value l_uvNode = l_documentRoot.get_object_value(l_nodeIndex).get_array_element(0);
    size_t l_uvsValue = l_uvNode.get_length();
    if(l_uvsValue == 0) Error("UVs subnode 0 is empty");
    Info(l_uvsValue / 2 << " UVs");

    std::vector<glm::vec2> l_uvVector;
    for(size_t i = 0; i < l_uvsValue; i += 2)
    {
        sajson::value l_node1 = l_uvNode.get_array_element(i);
        sajson::type l_node1Type = l_node1.get_type();
        if(l_node1Type != sajson::TYPE_INTEGER && l_node1Type != sajson::TYPE_DOUBLE) Error(i << " UV value isn't a number");
        sajson::value l_node2 = l_uvNode.get_array_element(i + 1);
        sajson::type l_node2Type = l_node2.get_type();
        if(l_node2Type != sajson::TYPE_INTEGER && l_node2Type != sajson::TYPE_DOUBLE) Error(i + 1 << "UV value isn't a number")
            l_uvVector.push_back(glm::vec2(
            (l_node1Type == sajson::TYPE_INTEGER) ? float(l_node1.get_integer_value()) : float(l_node1.get_double_value()),
            (l_node2Type == sajson::TYPE_INTEGER) ? 1.0f - float(l_node2.get_integer_value()) : 1.0f - float(l_node2.get_double_value())
            ));
    }

    l_origSize = l_uvVector.size()*sizeof(glm::vec2);
    l_maxSize = GetMaxCompressedLen(l_origSize);
    l_compressedData = new unsigned char[l_maxSize];
    l_compressedSized = CompressData(l_uvVector.data(), l_origSize, l_compressedData, l_maxSize);
    if(l_compressedSized == -1) Error("Unable to compress UVs");
    l_file.write(reinterpret_cast<char*>(&l_compressedSized), sizeof(int));
    l_file.write(reinterpret_cast<char*>(&l_origSize), sizeof(int));
    l_file.write(reinterpret_cast<char*>(l_compressedData), l_compressedSized);
    delete[]l_compressedData;

    l_nodeIndex = l_documentRoot.find_object_key(sajson::literal("normals"));
    if(l_nodeIndex == l_documentRoot.get_length()) Error("No normal node");
    sajson::value l_normalNode = l_documentRoot.get_object_value(l_nodeIndex);
    if(l_normalNode.get_type() != sajson::TYPE_ARRAY) Error("Normal node isn't an array");
    size_t l_normalsValue = l_normalNode.get_length();
    if(l_normalsValue == 0) Error("Normal node is empty");
    Info(l_normalsValue / 3 << " normals");

    std::vector<glm::vec3> l_normalVector;
    for(size_t i = 0; i < l_normalsValue; i += 3)
    {
        sajson::value l_node1 = l_normalNode.get_array_element(i);
        sajson::type l_node1Type = l_node1.get_type();
        if(l_node1Type != sajson::TYPE_INTEGER && l_node1Type != sajson::TYPE_DOUBLE) Error(i << " normal value isn't number");
        sajson::value l_node2 = l_normalNode.get_array_element(i + 1);
        sajson::type l_node2Type = l_node2.get_type();
        if(l_node2Type != sajson::TYPE_INTEGER && l_node2Type != sajson::TYPE_DOUBLE) Error(i + 1 << " normal value isn't number");
        sajson::value l_node3 = l_normalNode.get_array_element(i + 2);
        sajson::type l_node3Type = l_node3.get_type();
        if(l_node3Type != sajson::TYPE_INTEGER && l_node3Type != sajson::TYPE_DOUBLE) Error(i + 2 << " normal value isn't number");
        l_normalVector.push_back(glm::vec3(
            (l_node1Type == sajson::TYPE_INTEGER) ? float(l_node1.get_integer_value()) : float(l_node1.get_double_value()),
            (l_node2Type == sajson::TYPE_INTEGER) ? float(l_node2.get_integer_value()) : float(l_node2.get_double_value()),
            (l_node3Type == sajson::TYPE_INTEGER) ? float(l_node3.get_integer_value()) : float(l_node3.get_double_value())
            ));
    }

    l_origSize = l_normalVector.size()*sizeof(glm::vec3);
    l_maxSize = GetMaxCompressedLen(l_origSize);
    l_compressedData = new unsigned char[l_maxSize];
    l_compressedSized = CompressData(l_normalVector.data(), l_origSize, l_compressedData, l_maxSize);
    if(l_compressedSized == -1) Error("Unable to compress normals");
    l_file.write(reinterpret_cast<char*>(&l_compressedSized), sizeof(int));
    l_file.write(reinterpret_cast<char*>(&l_origSize), sizeof(int));
    l_file.write(reinterpret_cast<char*>(l_compressedData), l_compressedSized);
    delete[]l_compressedData;

    l_nodeIndex = l_documentRoot.find_object_key(sajson::literal("skinWeights"));
    if(l_nodeIndex == l_documentRoot.get_length()) Error("No weights node");
    sajson::value l_weightNode = l_documentRoot.get_object_value(l_nodeIndex);
    if(l_weightNode.get_type() != sajson::TYPE_ARRAY) Error("Weights node isn't an array");
    size_t l_weightsValue = l_weightNode.get_length();
    if(l_weightsValue == 0) Error("Weights node is empty");
    Info(l_weightsValue / 4 << " weights");

    std::vector<glm::vec4> l_weightVector;
    for(size_t i = 0; i < l_weightsValue; i += 4)
    {
        sajson::value l_node1 = l_weightNode.get_array_element(i);
        sajson::type l_node1Type = l_node1.get_type();
        if(l_node1Type != sajson::TYPE_INTEGER && l_node1Type != sajson::TYPE_DOUBLE) Error(i << " weight value isn't number");
        sajson::value l_node2 = l_weightNode.get_array_element(i + 1);
        sajson::type l_node2Type = l_node2.get_type();
        if(l_node2Type != sajson::TYPE_INTEGER && l_node2Type != sajson::TYPE_DOUBLE) Error(i + 1 << " weight value isn't number");
        sajson::value l_node3 = l_weightNode.get_array_element(i + 2);
        sajson::type l_node3Type = l_node3.get_type();
        if(l_node3Type != sajson::TYPE_INTEGER && l_node3Type != sajson::TYPE_DOUBLE) Error(i + 2 << " weight value isn't number");
        sajson::value l_node4 = l_weightNode.get_array_element(i + 3);
        sajson::type l_node4Type = l_node4.get_type();
        if(l_node4Type != sajson::TYPE_INTEGER && l_node4Type != sajson::TYPE_DOUBLE) Error(i + 3 << " weight value isn't number");
        l_weightVector.push_back(glm::vec4(
            (l_node1Type == sajson::TYPE_INTEGER) ? float(l_node1.get_integer_value()) : float(l_node1.get_double_value()),
            (l_node2Type == sajson::TYPE_INTEGER) ? float(l_node2.get_integer_value()) : float(l_node2.get_double_value()),
            (l_node3Type == sajson::TYPE_INTEGER) ? float(l_node3.get_integer_value()) : float(l_node3.get_double_value()),
            (l_node4Type == sajson::TYPE_INTEGER) ? float(l_node4.get_integer_value()) : float(l_node4.get_double_value())
            ));
    }

    l_origSize = l_weightVector.size()*sizeof(glm::vec4);
    l_maxSize = GetMaxCompressedLen(l_origSize);
    l_compressedData = new unsigned char[l_maxSize];
    l_compressedSized = CompressData(l_weightVector.data(), l_origSize, l_compressedData, l_maxSize);
    if(l_compressedSized == -1) Error("Unable to compress weights");
    l_file.write(reinterpret_cast<char*>(&l_compressedSized), sizeof(int));
    l_file.write(reinterpret_cast<char*>(&l_origSize), sizeof(int));
    l_file.write(reinterpret_cast<char*>(l_compressedData), l_compressedSized);
    delete[]l_compressedData;

    l_nodeIndex = l_documentRoot.find_object_key(sajson::literal("skinIndices"));
    if(l_nodeIndex == l_documentRoot.get_length()) Error("No indices node");
    sajson::value l_indexNode = l_documentRoot.get_object_value(l_nodeIndex);
    if(l_indexNode.get_type() != sajson::TYPE_ARRAY) Error("Indices node isn't an array");
    size_t l_indicesValue = l_indexNode.get_length();
    if(l_indicesValue == 0) Error("Indices node is empty");
    Info(l_indicesValue / 4 << " indices");

    std::vector<glm::ivec4> l_indexVector;
    for(size_t i = 0; i < l_weightsValue; i += 4)
    {
        sajson::value l_node1 = l_indexNode.get_array_element(i);
        sajson::type l_node1Type = l_node1.get_type();
        if(l_node1Type != sajson::TYPE_INTEGER) Error(i << " index value isn't number");
        sajson::value l_node2 = l_indexNode.get_array_element(i + 1);
        sajson::type l_node2Type = l_node2.get_type();
        if(l_node2Type != sajson::TYPE_INTEGER) Error(i + 1 << " index value isn't number");
        sajson::value l_node3 = l_indexNode.get_array_element(i + 2);
        sajson::type l_node3Type = l_node3.get_type();
        if(l_node3Type != sajson::TYPE_INTEGER) Error(i + 2 << " index value isn't number");
        sajson::value l_node4 = l_indexNode.get_array_element(i + 3);
        sajson::type l_node4Type = l_node4.get_type();
        if(l_node4Type != sajson::TYPE_INTEGER) Error(i + 3 << " index value isn't number");
        l_indexVector.push_back(glm::ivec4(
            l_node1.get_integer_value(), l_node2.get_integer_value(),
            l_node3.get_integer_value(), l_node4.get_integer_value()
            ));
    }

    l_origSize = l_indexVector.size()*sizeof(glm::ivec4);
    l_maxSize = GetMaxCompressedLen(l_origSize);
    l_compressedData = new unsigned char[l_maxSize];
    l_compressedSized = CompressData(l_indexVector.data(), l_origSize, l_compressedData, l_maxSize);
    if(l_compressedSized == -1) Error("Unable to compress indices");
    l_file.write(reinterpret_cast<char*>(&l_compressedSized), sizeof(int));
    l_file.write(reinterpret_cast<char*>(&l_origSize), sizeof(int));
    l_file.write(reinterpret_cast<char*>(l_compressedData), l_compressedSized);
    delete[]l_compressedData;

    l_nodeIndex = l_documentRoot.find_object_key(sajson::literal("faces"));
    if(l_nodeIndex == l_documentRoot.get_length()) Error("No faces node");
    sajson::value l_faceNode = l_documentRoot.get_object_value(l_nodeIndex);
    if(l_faceNode.get_type() != sajson::TYPE_ARRAY) Error("Faces node isn't an array");
    l_facesValue = l_faceNode.get_length();
    if(l_facesValue == 0) Error("Faces node is empty");
    if(l_facesValue % 11 != 0) Error("Wrong faces type. Be sure, that it's with normals, UVs, weights and indices");
    Info(l_facesValue / 11 << " faces");

    l_nodeIndex = l_documentRoot.find_object_key(sajson::literal("materials"));
    if(l_nodeIndex == l_documentRoot.get_length()) Error("No materials node");
    sajson::value l_materialsNode = l_documentRoot.get_object_value(l_nodeIndex);
    if(l_materialsNode.get_type() != sajson::TYPE_ARRAY) Error("Materials node isn't an array");
    size_t l_materialValue = l_materialsNode.get_length();
    if(l_materialValue == 0) Error("Materials node is empty");
    Info(l_materialValue << " materials");

    int l_iMaterialValue = l_materialValue;
    l_file.write(reinterpret_cast<char*>(&l_iMaterialValue), sizeof(int));

    // Parse materials
    // THREE.js faces structure:
    //  42, [vertex_index, vertex_index, vertex_index],
    //  [material_index],
    //  [vertex_uv, vertex_uv, vertex_uv],
    //  [vertex_normal, vertex_normal, vertex_normal]
    for(size_t i = 0; i < l_materialValue; i++)
    {
        std::vector<Face> l_facesVector;
        for(size_t j = 0, jj = l_facesValue / 11; j < jj; j++)
        {
            if(l_faceNode.get_array_element(j * 11 + 4).get_integer_value() == i)
            {
                Face l_face;
                sajson::value l_node_v1 = l_faceNode.get_array_element(j * 11 + 1);
                if(l_node_v1.get_type() != sajson::TYPE_INTEGER) Error("Material " << i << ", face " << j << ", value 0 isn't number");
                l_face.m_materialIndices[0] = l_node_v1.get_integer_value();
                sajson::value l_node_v2 = l_faceNode.get_array_element(j * 11 + 2);
                if(l_node_v2.get_type() != sajson::TYPE_INTEGER) Error("Material " << i << ", face " << j << ", value 1 isn't number");
                l_face.m_materialIndices[1] = l_node_v2.get_integer_value();
                sajson::value l_node_v3 = l_faceNode.get_array_element(j * 11 + 3);
                if(l_node_v3.get_type() != sajson::TYPE_INTEGER) Error("Material " << i << ", face " << j << ", value 2 isn't number");
                l_face.m_materialIndices[2] = l_node_v3.get_integer_value();

                sajson::value l_node_uv1 = l_faceNode.get_array_element(j * 11 + 5);
                if(l_node_uv1.get_type() != sajson::TYPE_INTEGER) Error("Material " << i << ", face " << j << ", value 3 isn't number");
                l_face.m_materialIndices[3] = l_node_uv1.get_integer_value();
                sajson::value l_node_uv2 = l_faceNode.get_array_element(j * 11 + 6);
                if(l_node_uv2.get_type() != sajson::TYPE_INTEGER) Error("Material " << i << ", face " << j << ", value 4 isn't number");
                l_face.m_materialIndices[4] = l_node_uv2.get_integer_value();
                sajson::value l_node_uv3 = l_faceNode.get_array_element(j * 11 + 7);
                if(l_node_uv3.get_type() != sajson::TYPE_INTEGER) Error("Material " << i << ", face " << j << ", value 5 isn't number");
                l_face.m_materialIndices[5] = l_node_uv3.get_integer_value();

                sajson::value l_node_n1 = l_faceNode.get_array_element(j * 11 + 8);
                if(l_node_n1.get_type() != sajson::TYPE_INTEGER) Error("Material " << i << ", face " << j << ", value 6 isn't number");
                l_face.m_materialIndices[6] = l_node_n1.get_integer_value();
                sajson::value l_node_n2 = l_faceNode.get_array_element(j * 11 + 9);
                if(l_node_n2.get_type() != sajson::TYPE_INTEGER) Error("Material " << i << ", face " << j << ", value 7 isn't number");
                l_face.m_materialIndices[7] = l_node_n2.get_integer_value();
                sajson::value l_node_n3 = l_faceNode.get_array_element(j * 11 + 10);
                if(l_node_n3.get_type() != sajson::TYPE_INTEGER) Error("Material " << i << ", face " << j << ", value 8 isn't number");
                l_face.m_materialIndices[8] = l_node_n3.get_integer_value();

                l_facesVector.push_back(l_face);
            }
        }
        sajson::value m_node = l_materialsNode.get_array_element(i);
        if(m_node.get_type() != sajson::TYPE_OBJECT) Error("Material " << i << " node isn't an object");

        std::bitset<8U> l_materialBit;
        l_materialBit.set(0, true);
        l_nodeIndex = m_node.find_object_key(sajson::literal("transparent"));
        if(l_nodeIndex == m_node.get_length()) l_materialBit.set(1, true);
        else
        {
            sajson::value l_transparencyData = m_node.get_object_value(l_nodeIndex);
            size_t l_transparencyType = l_transparencyData.get_type();
            if(l_transparencyType != sajson::TYPE_FALSE && l_transparencyType != sajson::TYPE_TRUE) l_materialBit.set(1, true);
            else
            {
                if(l_transparencyType == sajson::TYPE_FALSE) l_materialBit.set(1, true);
                else l_materialBit.set(2, true);
            }
        }
        l_nodeIndex = m_node.find_object_key(sajson::literal("doubleSided"));
        if(l_nodeIndex != m_node.get_length())
        {
            sajson::value l_dsData = m_node.get_object_value(l_nodeIndex);
            l_materialBit.set(3, l_dsData.get_type() == sajson::TYPE_TRUE);
        }
        l_nodeIndex = m_node.find_object_key(sajson::literal("filtering"));
        if(l_nodeIndex != m_node.get_length())
        {
            sajson::value l_filterData = m_node.get_object_value(l_nodeIndex);
            if(l_filterData.get_type() == sajson::TYPE_INTEGER) l_materialBit.set(4, l_filterData.get_integer_value() == 1);
        }
        l_nodeIndex = m_node.find_object_key(sajson::literal("compression"));
        if(l_nodeIndex != m_node.get_length())
        {
            sajson::value l_compData = m_node.get_object_value(l_nodeIndex);
            l_materialBit.set(5, l_compData.get_type() == sajson::TYPE_TRUE);
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
        l_compressedData = new unsigned char[l_maxSize];
        l_compressedSized = CompressData(l_facesVector.data(), l_origSize, l_compressedData, l_maxSize);
        if(l_compressedSized == -1) Error("Unable to compress material " << i << " faces");
        l_file.write(reinterpret_cast<char*>(&l_compressedSized), sizeof(int));
        l_file.write(reinterpret_cast<char*>(&l_origSize), sizeof(int));
        l_file.write(reinterpret_cast<char*>(l_compressedData), l_compressedSized);
        delete[]l_compressedData;
        Info("Material " << i << ", " << l_facesVector.size() << " faces");
        l_facesVector.clear();
    }

    // Parse skeleton
    l_nodeIndex = l_documentRoot.find_object_key(sajson::literal("bones"));
    if(l_nodeIndex == l_documentRoot.get_length()) Error("No bones node");
    sajson::value l_bonesNode = l_documentRoot.get_object_value(l_nodeIndex);
    if(l_bonesNode.get_type() != sajson::TYPE_ARRAY) Error("Bones node isn't an array");
    size_t l_bonesValue = l_bonesNode.get_length();
    if(l_bonesValue == 0) Error("Bones node is empty");
    Info(l_bonesValue << " bones");

    int l_iBonesValue = l_bonesValue;
    l_file.write(reinterpret_cast<char*>(&l_iBonesValue), sizeof(int));

    for(size_t i = 0; i < l_bonesValue; i++)
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
        if(l_parentBoneValue >= int(l_bonesValue) || l_parentBoneValue < -1) Error("Wrong value of parent for bone " << i);
        l_file.write(reinterpret_cast<char*>(&l_parentBoneValue), sizeof(int));

        std::vector<float> l_valueVector;

        l_nodeIndex = b_node.find_object_key(sajson::literal("pos"));
        if(l_nodeIndex == b_node.get_length()) Error("No position node for bone " << i);
        sajson::value l_node2 = b_node.get_object_value(l_nodeIndex);
        if(l_node2.get_type() != sajson::TYPE_ARRAY) Error("Position node for bone " << i << " isn't an array");
        if(l_node2.get_length() != 3) Error("Position node size for bone " << i << " isn't equal 3");
        for(size_t j = 0; j < 3; j++)
        {
            sajson::value l_nodeData = l_node2.get_array_element(j);
            size_t l_nodeType = l_nodeData.get_type();
            if(l_nodeType != sajson::TYPE_INTEGER && l_nodeType != sajson::TYPE_DOUBLE) Error("Position value " << j << " for bone " << i << " isn't an number");
            l_valueVector.push_back(float(l_nodeType == sajson::TYPE_INTEGER ? l_nodeData.get_integer_value() : l_nodeData.get_double_value()));
        }
        glm::vec3 l_pos(l_valueVector[0], l_valueVector[1], l_valueVector[2]);
        l_valueVector.clear();
        l_file.write(reinterpret_cast<char*>(&l_pos), sizeof(glm::vec3));

        l_nodeIndex = b_node.find_object_key(sajson::literal("rotq"));
        if(l_nodeIndex == b_node.get_length()) Error("No rotation node for bone " << i);
        sajson::value l_node1 = b_node.get_object_value(l_nodeIndex);
        if(l_node1.get_type() != sajson::TYPE_ARRAY) Error("Rotation node for bone " << i << " isn't an array");
        if(l_node1.get_length() != 4) Error("Rotation node size for bone " << i << " isn't equal 4");
        for(size_t j = 0; j < 4; j++)
        {
            sajson::value l_nodeData = l_node1.get_array_element(j);
            size_t l_nodeType = l_nodeData.get_type();
            if(l_nodeType != sajson::TYPE_INTEGER && l_nodeType != sajson::TYPE_DOUBLE) Error("Rotation value " << j << " for bone " << i << " isn't an number");
            l_valueVector.push_back(float(l_nodeType == sajson::TYPE_INTEGER ? l_nodeData.get_integer_value() : l_nodeData.get_double_value()));
        }
        glm::quat l_quat(l_valueVector[3], l_valueVector[0], l_valueVector[1], l_valueVector[2]);
        l_valueVector.clear();
        l_file.write(reinterpret_cast<char*>(&l_quat), sizeof(glm::quat));

        l_nodeIndex = b_node.find_object_key(sajson::literal("scl"));
        if(l_nodeIndex == b_node.get_length())
        {
            Info("No scale node for bone " << i << ". Set to default 1.0");
            for(int i = 0; i < 3; i++) l_valueVector.push_back(1.f);
        }
        else
        {
            sajson::value l_node3 = b_node.get_object_value(l_nodeIndex);
            if(l_node3.get_type() != sajson::TYPE_ARRAY) Error("Scale node for bone " << i << " isn't an array");
            if(l_node3.get_length() != 3) Error("Scale node size for bone " << i << " isn't equal 3");
            for(size_t j = 0; j < 3; j++)
            {
                sajson::value l_nodeData = l_node3.get_array_element(j);
                size_t l_nodeType = l_nodeData.get_type();
                if(l_nodeType != sajson::TYPE_INTEGER && l_nodeType != sajson::TYPE_DOUBLE) Error("Scale value " << j << " for bone " << i << " isn't an number");
                l_valueVector.push_back(float(l_nodeType == sajson::TYPE_INTEGER ? l_nodeData.get_integer_value() : l_nodeData.get_double_value()));
            }
        }
        glm::vec3 l_scale(l_valueVector[0], l_valueVector[1], l_valueVector[2]);
        l_file.write(reinterpret_cast<char*>(&l_scale), sizeof(glm::vec3));
    }
    l_file.flush();
    l_file.close();
    Info("Model is reconverted to " << f_out.c_str());
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
            if(l_buffer.at(0) == 'N' || l_buffer.at(0) == 'K' || l_buffer.at(0) == 'd' || l_buffer.at(0) == 'i') continue;
            if(!l_buffer.find("newmtl "))
            {
                std::string l_materialName = l_buffer.substr(7U);
                l_materialNames.push_back(l_materialName);
                continue;
            }
            if(!l_buffer.find("type ")) // this is custom line, you have to insert it in file with value based on material bits
            {
                std::string l_typeString = l_buffer.substr(5U);
                l_materialTypes.push_back(static_cast<unsigned char>(std::stoi(l_typeString)));
                continue;
            }
            if(!l_buffer.find("map_Kd ") || !l_buffer.find("map_kd "))
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

    if(l_materialNames.size() == 0)
    {
        l_materialNames.push_back("DefaultMaterial");
        l_currentMaterial = 0;
    }

    if(l_materialTextureNames.size() < l_materialNames.size())
    {
        for(size_t i = 0, j = l_materialNames.size() - l_materialTextureNames.size(); i < j; i++)
        {
            l_materialTextureNames.push_back("");
        }
    }

    if(l_materialTypes.size() < l_materialNames.size())
    {
        for(size_t i = 0, j = l_materialNames.size() - l_materialTypes.size(); i < j; i++)
        {
            l_materialTypes.push_back(3U);
        }
    }

    std::ofstream l_outputFile(f_out, std::ios::out | std::ios::binary);
    if(l_outputFile.fail()) Error("Unable to create output file");
    l_outputFile.write("ROC", 3);
    unsigned char l_setter = 0x1U;
    l_outputFile.write(reinterpret_cast<char*>(&l_setter), sizeof(unsigned char));
    bool l_dataParsed = false;
    bool l_materialsSizeParsed = false;
    std::vector<Face> l_faceVector;
    while(std::getline(l_objectFile, l_buffer))
    {
        if(l_buffer.empty()) continue;
        if(l_buffer.at(0) == '#' || l_buffer.at(0) == 's' || l_buffer.at(0) == 'm' || l_buffer.at(0) == 'o') continue;
        if(!l_buffer.find("v "))
        {
            std::stringstream l_vertexStream(l_buffer.substr(2U));
            glm::vec3 l_vert;
            l_vertexStream >> l_vert.x >> l_vert.y >> l_vert.z;
            temp_vertex.push_back(l_vert);
            continue;
        }
        if(!l_buffer.find("vn "))
        {
            std::stringstream l_normalStream(l_buffer.substr(3U));
            glm::vec3 l_norm;
            l_normalStream >> l_norm.x >> l_norm.y >> l_norm.z;
            temp_normal.push_back(l_norm);
            continue;
        }
        if(!l_buffer.find("vt "))
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
            unsigned char *l_compressedData = new unsigned char[l_maxSize];
            int l_compressedSize = CompressData(temp_vertex.data(), l_origSize, l_compressedData, l_maxSize);
            if(l_compressedSize == -1) Error("Unable to compress vertices");
            l_outputFile.write(reinterpret_cast<char*>(&l_compressedSize), sizeof(int));
            l_outputFile.write(reinterpret_cast<char*>(&l_origSize), sizeof(int));
            l_outputFile.write(reinterpret_cast<char*>(l_compressedData), l_compressedSize);
            delete[]l_compressedData;
            Info(temp_vertex.size() << " vertices");

            l_origSize = temp_uv.size()*sizeof(glm::vec2);
            l_maxSize = GetMaxCompressedLen(l_origSize);
            l_compressedData = new unsigned char[l_maxSize];
            l_compressedSize = CompressData(temp_uv.data(), l_origSize, l_compressedData, l_maxSize);
            if(l_compressedSize == -1) Error("Unable to compress UVs");
            l_outputFile.write(reinterpret_cast<char*>(&l_compressedSize), sizeof(int));
            l_outputFile.write(reinterpret_cast<char*>(&l_origSize), sizeof(int));
            l_outputFile.write(reinterpret_cast<char*>(l_compressedData), l_compressedSize);
            delete[]l_compressedData;
            Info(temp_vertex.size() << " UVs");

            l_origSize = temp_normal.size()*sizeof(glm::vec3);
            l_maxSize = GetMaxCompressedLen(l_origSize);
            l_compressedData = new unsigned char[l_maxSize];
            l_compressedSize = CompressData(temp_normal.data(), l_origSize, l_compressedData, l_maxSize);
            if(l_compressedSize == -1) Error("Unable to compress normals");
            l_outputFile.write(reinterpret_cast<char*>(&l_compressedSize), sizeof(int));
            l_outputFile.write(reinterpret_cast<char*>(&l_origSize), sizeof(int));
            l_outputFile.write(reinterpret_cast<char*>(l_compressedData), l_compressedSize);
            delete[]l_compressedData;
            Info(temp_vertex.size() << " normals");
        }
        if(!l_buffer.find("usemtl "))
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
                unsigned char *l_compressedData = new unsigned char[l_maxSize];
                int l_compressedSize = CompressData(l_faceVector.data(), l_origSize, l_compressedData, l_maxSize);
                if(l_compressedSize == -1) Error("Unable to compress faces for material " << l_currentMaterial);
                l_outputFile.write(reinterpret_cast<char*>(&l_compressedSize), sizeof(int));
                l_outputFile.write(reinterpret_cast<char*>(&l_origSize), sizeof(int));
                l_outputFile.write(reinterpret_cast<char*>(l_compressedData), l_compressedSize);
                delete[]l_compressedData;
                Info("Material " << l_currentMaterial << ", " << l_faceVector.size() << " faces");
                l_faceVector.clear();
            }
            std::string l_textureName = l_buffer.substr(7U);
            l_currentMaterial = std::distance(l_materialNames.begin(), std::find(l_materialNames.begin(), l_materialNames.end(), l_textureName));
            if(l_currentMaterial >= int(l_materialNames.size()) || l_currentMaterial < 0) Error("Unable to parse materials");
            continue;
        }
        if(!l_buffer.find("f "))
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
        unsigned char *l_compressedData = new unsigned char[l_maxSize];
        int l_compressedSize = CompressData(l_faceVector.data(), l_origSize, l_compressedData, l_maxSize);
        if(l_compressedSize == -1) Error("Unable to compress faces for material " << l_currentMaterial);
        l_outputFile.write(reinterpret_cast<char*>(&l_compressedSize), sizeof(int));
        l_outputFile.write(reinterpret_cast<char*>(&l_origSize), sizeof(int));
        l_outputFile.write(reinterpret_cast<char*>(l_compressedData), l_compressedSize);
        delete[]l_compressedData;
        Info("Material " << l_currentMaterial << ", " << l_faceVector.size() << " faces");
        l_faceVector.clear();
    }
    l_objectFile.close();
    l_outputFile.flush();
    l_outputFile.close();
    Info("Model converted to " << f_out.c_str());
}

int main(int argc, char *argv[])
{
    if(argc < 3) std::cout << "Usage: [input_type] [input_file] <[output_file]>" << std::endl;
    else
    {
        std::string l_inputType(argv[1]);
        std::string l_inputFile(argv[2]);
        std::string l_outputFile;
        if(argc >= 4) l_outputFile.assign(argv[3]);
        else
        {
            l_outputFile.assign(l_inputFile);
            l_outputFile.append(".rmf");
        }
        switch(ReadEnumString(l_inputType, "obj,json"))
        {
            case 0:
            {
                Info("Converting OBJ format...");
                ConvertOBJ(l_inputFile, l_outputFile);
            } break;
            case 1:
            {
                Info("Converting JSON (THREE.js) format...");
                ConvertJSON(l_inputFile, l_outputFile);
            } break;
            default:
                Info("Unknown format. Avaliable formats: obj, json (THREE.js animated)");
        }
    }
    return EXIT_SUCCESS;
}
