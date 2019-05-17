#include "stdafx.h"

#include "Model.h"

//void ConvertOBJ(std::string &f_path, std::string &f_out)
//{
//    try
//    {
//        std::vector<glm::vec3> temp_vertex;
//        std::vector<glm::vec2> temp_uv;
//        std::vector<glm::vec3> temp_normal;
//
//        std::vector<std::string> l_materialNames;
//        std::vector<std::string> l_materialTextureNames;
//        std::vector<unsigned char> l_materialTypes;
//        int l_currentMaterial = -1;
//        bool l_defaultMaterial = true;
//
//        std::ifstream l_objectFile;
//        l_objectFile.open(f_path + ".obj", std::ios::in);
//        if(l_objectFile.fail()) throw std::exception("Unable to load input file");
//
//        std::string l_buffer;
//
//        std::ifstream l_materialFile;
//        l_materialFile.open(f_path + ".mtl", std::ios::in);
//        if(!l_materialFile.fail())
//        {
//            l_defaultMaterial = false;
//            //Materials parsing
//            while(std::getline(l_materialFile, l_buffer))
//            {
//                if(l_buffer.empty()) continue;
//                if(l_buffer.at(0U) == 'N' || l_buffer.at(0U) == 'K' || l_buffer.at(0U) == 'd' || l_buffer.at(0U) == 'i') continue;
//                if(l_buffer.find("newmtl ") != std::string::npos)
//                {
//                    std::string l_materialName = l_buffer.substr(7U);
//                    l_materialNames.push_back(l_materialName);
//                    continue;
//                }
//                if(l_buffer.find("type ") != std::string::npos) // this is custom line, you have to insert it in file with value based on material bits
//                {
//                    std::string l_typeString = l_buffer.substr(5U);
//                    l_materialTypes.push_back(static_cast<unsigned char>(std::stoi(l_typeString)));
//                    continue;
//                }
//                if(l_buffer.find("map_Kd ") != std::string::npos || l_buffer.find("map_kd ") != std::string::npos)
//                {
//                    std::string l_textureName = l_buffer.substr(7U);
//                    l_textureName.insert(0U, "textures/");
//                    l_materialTextureNames.push_back(l_textureName);
//                    continue;
//                }
//            }
//            l_materialFile.close();
//        }
//        else std::cout << "No .mtl file, assumed that all materials are default" << std::endl;
//
//        if(l_materialNames.size() == 0U)
//        {
//            l_materialNames.push_back("DefaultMaterial");
//            l_currentMaterial = 0;
//        }
//
//        if(l_materialTextureNames.size() < l_materialNames.size())
//        {
//            for(size_t i = 0U, j = l_materialNames.size() - l_materialTextureNames.size(); i < j; i++)
//            {
//                l_materialTextureNames.push_back("");
//            }
//        }
//
//        if(l_materialTypes.size() < l_materialNames.size())
//        {
//            for(size_t i = 0U, j = l_materialNames.size() - l_materialTypes.size(); i < j; i++)
//            {
//                l_materialTypes.push_back(3U);
//            }
//        }
//
//        std::ofstream l_outputFile(f_out, std::ios::out | std::ios::binary);
//        if(l_outputFile.fail()) throw std::exception("Unable to create output file");
//
//        l_outputFile.write("ROC", 3U);
//        unsigned char l_setter = 0x1U;
//        l_outputFile.write(reinterpret_cast<char*>(&l_setter), sizeof(unsigned char));
//
//        bool l_dataParsed = false;
//        bool l_materialsSizeParsed = false;
//        std::vector<Face> l_faceVector;
//        std::vector<unsigned char> l_compressedData;
//
//        while(std::getline(l_objectFile, l_buffer))
//        {
//            if(l_buffer.empty()) continue;
//            if(l_buffer.at(0U) == '#' || l_buffer.at(0U) == 's' || l_buffer.at(0U) == 'm' || l_buffer.at(0U) == 'o') continue;
//            if(l_buffer.find("v ") != std::string::npos)
//            {
//                std::stringstream l_vertexStream(l_buffer.substr(2U));
//                glm::vec3 l_vert;
//                l_vertexStream >> l_vert.x >> l_vert.y >> l_vert.z;
//                temp_vertex.push_back(l_vert);
//                continue;
//            }
//            if(l_buffer.find("vn ") != std::string::npos)
//            {
//                std::stringstream l_normalStream(l_buffer.substr(3U));
//                glm::vec3 l_norm;
//                l_normalStream >> l_norm.x >> l_norm.y >> l_norm.z;
//                temp_normal.push_back(l_norm);
//                continue;
//            }
//            if(l_buffer.find("vt ") != std::string::npos)
//            {
//                std::stringstream l_uvStream(l_buffer.substr(3U));
//                glm::vec2 l_uv;
//                l_uvStream >> l_uv.x >> l_uv.y;
//                l_uv.y = 1.f - l_uv.y;
//                temp_uv.push_back(l_uv);
//                continue;
//            }
//            if(!l_dataParsed)
//            {
//                l_dataParsed = true;
//
//                unsigned int l_origSize = temp_vertex.size()*sizeof(glm::vec3);
//                unsigned int l_maxSize = GetMaxCompressedLen(l_origSize);
//                l_compressedData.resize(static_cast<size_t>(l_maxSize));
//                unsigned int l_compressedSize = CompressData(temp_vertex.data(), l_origSize, l_compressedData.data(), l_maxSize);
//                if(l_compressedSize == 0U) throw std::exception("Unable to compress vertices");
//                l_outputFile.write(reinterpret_cast<char*>(&l_compressedSize), sizeof(unsigned int));
//                l_outputFile.write(reinterpret_cast<char*>(&l_origSize), sizeof(unsigned int));
//                l_outputFile.write(reinterpret_cast<char*>(l_compressedData.data()), l_compressedSize);
//                std::cout << temp_vertex.size() << " vertices" << std::endl;
//
//                l_origSize = temp_uv.size()*sizeof(glm::vec2);
//                l_maxSize = GetMaxCompressedLen(l_origSize);
//                l_compressedData.resize(static_cast<size_t>(l_maxSize));
//                l_compressedSize = CompressData(temp_uv.data(), l_origSize, l_compressedData.data(), l_maxSize);
//                if(l_compressedSize == 0U) throw std::exception("Unable to compress UVs");
//                l_outputFile.write(reinterpret_cast<char*>(&l_compressedSize), sizeof(unsigned int));
//                l_outputFile.write(reinterpret_cast<char*>(&l_origSize), sizeof(unsigned int));
//                l_outputFile.write(reinterpret_cast<char*>(l_compressedData.data()), l_compressedSize);
//                std::cout << temp_vertex.size() << " UVs" << std::endl;
//
//                l_origSize = temp_normal.size()*sizeof(glm::vec3);
//                l_maxSize = GetMaxCompressedLen(l_origSize);
//                l_compressedData.resize(static_cast<size_t>(l_maxSize));
//                l_compressedSize = CompressData(temp_normal.data(), l_origSize, l_compressedData.data(), l_maxSize);
//                if(l_compressedSize == -1) throw std::exception("Unable to compress normals");
//                l_outputFile.write(reinterpret_cast<char*>(&l_compressedSize), sizeof(unsigned int));
//                l_outputFile.write(reinterpret_cast<char*>(&l_origSize), sizeof(unsigned int));
//                l_outputFile.write(reinterpret_cast<char*>(l_compressedData.data()), l_compressedSize);
//                std::cout << temp_vertex.size() << " normals" << std::endl;
//            }
//            if(l_buffer.find("usemtl ") != std::string::npos)
//            {
//                if(!l_materialsSizeParsed)
//                {
//                    int l_materialsSizeI = static_cast<int>(l_materialNames.size());
//                    l_outputFile.write(reinterpret_cast<char*>(&l_materialsSizeI), sizeof(int));
//                    l_materialsSizeParsed = true;
//                    std::cout << l_materialsSizeI << " material(s)" << std::endl;
//                }
//                if(l_defaultMaterial) continue;
//                if(l_currentMaterial != -1)
//                {
//                    std::cout << "Building material " << l_currentMaterial << " ..." << std::endl;
//
//                    l_setter = static_cast<unsigned char>(l_materialTypes[static_cast<size_t>(l_currentMaterial)]);
//                    l_outputFile.write(reinterpret_cast<char*>(&l_setter), sizeof(unsigned char));
//                    glm::vec4 l_params(1.f);
//                    l_outputFile.write(reinterpret_cast<char*>(&l_params), sizeof(glm::vec4));
//                    l_setter = static_cast<unsigned char>(l_materialTextureNames[static_cast<size_t>(l_currentMaterial)].size());
//                    l_outputFile.write(reinterpret_cast<char*>(&l_setter), sizeof(unsigned char));
//                    if(l_setter) l_outputFile.write(l_materialTextureNames[static_cast<size_t>(l_currentMaterial)].data(), l_setter);
//
//                    unsigned int l_origSize = l_faceVector.size()*sizeof(Face);
//                    unsigned int l_maxSize = GetMaxCompressedLen(l_origSize);
//                    l_compressedData.resize(static_cast<size_t>(l_maxSize));
//                    unsigned int l_compressedSize = CompressData(l_faceVector.data(), l_origSize, l_compressedData.data(), l_maxSize);
//                    if(l_compressedSize == 0U) throw std::exception("Unable to compress faces");
//                    l_outputFile.write(reinterpret_cast<char*>(&l_compressedSize), sizeof(unsigned int));
//                    l_outputFile.write(reinterpret_cast<char*>(&l_origSize), sizeof(unsigned int));
//                    l_outputFile.write(reinterpret_cast<char*>(l_compressedData.data()), l_compressedSize);
//                    std::cout << l_faceVector.size() << " faces" << std::endl;
//                    l_faceVector.clear();
//                }
//                std::string l_textureName = l_buffer.substr(7U);
//                l_currentMaterial = static_cast<int>(std::distance(l_materialNames.begin(), std::find(l_materialNames.begin(), l_materialNames.end(), l_textureName)));
//                if(l_currentMaterial >= int(l_materialNames.size()) || l_currentMaterial < 0) throw std::exception("Unable to parse materials");
//                continue;
//            }
//            if(l_buffer.find("f ") != std::string::npos)
//            {
//                if(!l_materialsSizeParsed)
//                {
//                    int l_materialsSizeI = static_cast<int>(l_materialNames.size());
//                    l_outputFile.write(reinterpret_cast<char*>(&l_materialsSizeI), sizeof(int));
//                    l_materialsSizeParsed = true;
//                    std::cout << l_materialsSizeI << " material(s)" << std::endl;
//                }
//                std::string l_faceString = l_buffer.substr(2U);
//                std::replace(l_faceString.begin(), l_faceString.end(), '/', ' ');
//                std::stringstream l_faceStream(l_faceString);
//                Face l_face;
//                l_faceStream >>
//                    l_face.m_materialIndices[0] >> l_face.m_materialIndices[3] >> l_face.m_materialIndices[6] >>
//                    l_face.m_materialIndices[1] >> l_face.m_materialIndices[4] >> l_face.m_materialIndices[7] >>
//                    l_face.m_materialIndices[2] >> l_face.m_materialIndices[5] >> l_face.m_materialIndices[8];
//                if(l_faceStream.fail()) throw std::exception("Invalid face line");
//
//                for(int i = 0; i < 9; i++) l_face.m_materialIndices[i]--;
//                l_faceVector.push_back(l_face);
//                continue;
//            }
//        }
//        if(l_currentMaterial != -1)
//        {
//            std::cout << "Building material " << l_currentMaterial << " ..." << std::endl;
//
//            l_setter = static_cast<unsigned char>(l_materialTypes[static_cast<size_t>(l_currentMaterial)]);
//            l_outputFile.write(reinterpret_cast<char*>(&l_setter), sizeof(unsigned char));
//            glm::vec4 l_params(1.f);
//            l_outputFile.write(reinterpret_cast<char*>(&l_params), sizeof(glm::vec4));
//            l_setter = static_cast<unsigned char>(l_materialTextureNames[static_cast<size_t>(l_currentMaterial)].size());
//            l_outputFile.write(reinterpret_cast<char*>(&l_setter), sizeof(unsigned char));
//            if(l_setter) l_outputFile.write(l_materialTextureNames[static_cast<size_t>(l_currentMaterial)].data(), l_setter);
//
//            unsigned int l_origSize = l_faceVector.size()*sizeof(Face);
//            unsigned int l_maxSize = GetMaxCompressedLen(l_origSize);
//            l_compressedData.resize(static_cast<size_t>(l_maxSize));
//            unsigned int l_compressedSize = CompressData(l_faceVector.data(), l_origSize, l_compressedData.data(), l_maxSize);
//            if(l_compressedSize == 0U) throw std::exception("Unable to compress faces");
//            l_outputFile.write(reinterpret_cast<char*>(&l_compressedSize), sizeof(unsigned int));
//            l_outputFile.write(reinterpret_cast<char*>(&l_origSize), sizeof(unsigned int));
//            l_outputFile.write(reinterpret_cast<char*>(l_compressedData.data()), l_compressedSize);
//            std::cout << l_faceVector.size() << " faces" << std::endl;
//            l_faceVector.clear();
//        }
//        l_objectFile.close();
//        l_outputFile.flush();
//        l_outputFile.close();
//
//        l_compressedData.clear();
//
//        std::cout << "Converted to " << f_out << std::endl;
//    }
//    catch(const std::exception &e)
//    {
//        std::cout << "Failed: " << e.what() << std::endl;
//
//        std::cout << "Cleaning ..." << std::endl;
//        std::remove(f_out.c_str());
//    }
//}

int main(int argc, char *argv[])
{
    if(argc > 1)
    {
        std::string l_inputFile(argv[1]);

        Model *l_model = new Model();
        bool l_loadResult = false;
        if(l_inputFile.find(".obj") != std::string::npos) l_loadResult = l_model->LoadOBJ(l_inputFile);
        else if(l_inputFile.find(".json") != std::string::npos) l_loadResult = l_model->LoadJSON(l_inputFile);
        else std::cout << "Error: Unknown model file for conversion. Only OBJ and THREE.js are supported" << std::endl;
        if(l_loadResult)
        {
            std::string l_outputFile(l_inputFile + ".rmf");
            l_model->Generate(l_outputFile);
        }
        delete l_model;
    }
    else std::cout << "Usage: [input_file]" << std::endl;
    std::cout << "Press any key to exit" << std::endl;
    std::getchar();
    return EXIT_SUCCESS;
}
