#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "pugixml.hpp"

#define Error(T) { std::cout << T << std::endl; std::getchar(); return EXIT_FAILURE; }
#define Info(T) { std::cout << T << std::endl; }

struct sbcData
{
    unsigned char m_type;
    float m_sizeX, m_sizeY, m_sizeZ;
    float m_posX, m_posY, m_posZ;
    float m_rotX, m_rotY, m_rotZ, m_rotW;
    unsigned int m_boneID;
};
struct chainPartData
{
    unsigned int m_boneID;
    unsigned char m_type;
    float m_sizeX, m_sizeY, m_sizeZ;
    float m_offsetX, m_offsetY, m_offsetZ;
    float m_rotationX, m_rotationY, m_rotationZ, m_rotationW;
    float m_mass, m_restutition, m_friction, m_linearDamping, m_angularDamping;
    float m_lowerAngularLimitX, m_lowerAngularLimitY, m_lowerAngularLimitZ;
    float m_upperAngularLimitX, m_upperAngularLimitY, m_upperAngularLimitZ;
    float m_angularStiffnessX, m_angularStiffnessY, m_angularStiffnessZ;
    float m_lowerLinearLimitX, m_lowerLinearLimitY, m_lowerLinearLimitZ;
    float m_upperLinearLimitX, m_upperLinearLimitY, m_upperLinearLimitZ;
    float m_linearStiffnessX, m_linearStiffnessY, m_linearStiffnessZ;
};

int main(int argc, char *argv[])
{
    if(argc > 1)
    {
        pugi::xml_document l_meta;
        if(!l_meta.load_file(argv[1])) Error("Not XML file\n");
        pugi::xml_node l_root = l_meta.child("collision");
        if(!l_root) Error("No 'collision' root\n");
        std::ofstream l_file(std::string(argv[1]) + ".bin", std::ios::out | std::ios::binary);
        if(l_file.fail()) Error("Unable to create binary file\n");

        unsigned char l_setter = 0xCBU;
        l_file.write(reinterpret_cast<char*>(&l_setter), sizeof(unsigned char));

        unsigned int l_sbcCount = 0U;
        pugi::xml_node l_statics = l_root.child("statics");
        if(l_statics)
        {
            Info("Data of static bone collision has been found(ed?)");
            pugi::xml_attribute l_sbcCountAtr = l_statics.attribute("count");
            if(l_sbcCountAtr)
            {
                l_sbcCount = l_sbcCountAtr.as_uint();
                l_file.write(reinterpret_cast<char*>(&l_sbcCount), sizeof(unsigned int));

                if(l_sbcCount > 0U)
                {
                    for(pugi::xml_node l_sbcNode = l_statics.child("sbc"); l_sbcNode; l_sbcNode = l_sbcNode.next_sibling("sbc"))
                    {
                        if(l_sbcCount == 0U) Error("Count of 'sbc' nodes isn't equal to 'count' attribute");
                        sbcData l_sbcData;

                        pugi::xml_attribute l_attr = l_sbcNode.attribute("bone");
                        if(!l_attr) Error("One of 'sbc' nodes doesn't have 'bone' attribute");
                        l_sbcData.m_boneID = l_attr.as_uint();

                        l_attr = l_sbcNode.attribute("type");
                        if(!l_attr) Error("One of sbc nodes doesn't have 'type' attribute");
                        l_sbcData.m_type = static_cast<unsigned char>(l_attr.as_uint());

                        l_attr = l_sbcNode.attribute("offset");
                        if(!l_attr) Error("One of 'sbc' nodes doesn't have 'offset' attribute");
                        std::stringstream l_offsetStream(l_attr.as_string("0.0 0.0 0.0"));
                        l_offsetStream >> l_sbcData.m_posX >> l_sbcData.m_posY >> l_sbcData.m_posZ;

                        l_attr = l_sbcNode.attribute("size");
                        if(!l_attr) Error("One of 'sbc' nodes doesn't have 'size' attribute");
                        std::stringstream l_sizeStream(l_attr.as_string("0.0 0.0 0.0"));
                        l_sizeStream >> l_sbcData.m_sizeX >> l_sbcData.m_sizeY >> l_sbcData.m_sizeZ;

                        l_attr = l_sbcNode.attribute("rotation");
                        if(!l_attr) Error("One of sbc nodes doesn't have 'rotation' attribute");
                        std::stringstream l_rotationStream(l_attr.as_string("0.0 0.0 0.0 1.0"));
                        l_rotationStream >> l_sbcData.m_rotX >> l_sbcData.m_rotY >> l_sbcData.m_rotZ >> l_sbcData.m_rotW;

                        l_file.write(reinterpret_cast<char*>(&l_sbcData.m_type), sizeof(unsigned char));
                        l_file.write(reinterpret_cast<char*>(&l_sbcData.m_sizeX), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_sbcData.m_sizeY), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_sbcData.m_sizeZ), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_sbcData.m_posX), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_sbcData.m_posY), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_sbcData.m_posZ), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_sbcData.m_rotX), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_sbcData.m_rotY), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_sbcData.m_rotZ), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_sbcData.m_rotW), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_sbcData.m_boneID), sizeof(unsigned int));

                        l_sbcCount--;
                    }
                }
            }
            else Error("No 'count' attribute at 'statics' subnode");
        }
        else
        {
            l_file.write(reinterpret_cast<char*>(&l_sbcCount), sizeof(unsigned int));
            Info("No data of static bone collisions");
        }

        unsigned int l_chainsCount = 0U;
        pugi::xml_node l_chains = l_root.child("chains");
        if(l_chains)
        {
            Info("Data of bone chains has been found(ed?)");
            pugi::xml_attribute l_chainsCountAtr = l_chains.attribute("count");
            if(l_chainsCountAtr)
            {
                l_chainsCount = l_chainsCountAtr.as_uint();
                l_file.write(reinterpret_cast<char*>(&l_chainsCount), sizeof(unsigned int));

                for(pugi::xml_node l_chain = l_chains.child("chain"); l_chain; l_chain = l_chain.next_sibling("chain"))
                {
                    if(l_chainsCount == 0U) Error("Count of 'chain' nodes isn't equal to 'count' attribute");

                    pugi::xml_attribute l_attr = l_chain.attribute("count");
                    pugi::xml_attribute l_boneIDAtr = l_chain.attribute("bone");
                    if(!l_attr || !l_boneIDAtr) Error("No 'count' or 'bone' attribute at 'chain' node\n");

                    unsigned int l_chainPartsCount = l_attr.as_uint();
                    l_file.write(reinterpret_cast<char*>(&l_chainPartsCount), sizeof(unsigned int));
                    unsigned int l_chainBone = l_boneIDAtr.as_uint();
                    l_file.write(reinterpret_cast<char*>(&l_chainBone), sizeof(unsigned int));


                    for(pugi::xml_node l_part = l_chain.child("part"); l_part; l_part = l_part.next_sibling("part"))
                    {
                        if(l_chainPartsCount == 0U) Error("Count of 'part' nodes isn't equal to 'count' attribute");

                        chainPartData l_chainPartData;

                        l_attr = l_part.attribute("bone");
                        if(!l_attr) Error("No 'boneID' attribute at part node\n");
                        l_chainPartData.m_boneID = l_attr.as_int();

                        l_attr = l_part.attribute("type");
                        if(!l_attr) Error("No 'type' attribute at part node\n");
                        l_chainPartData.m_type = static_cast<unsigned char>(l_attr.as_uint());

                        l_attr = l_part.attribute("size");
                        if(!l_attr) Error("No 'size' attribute at part node\n");
                        std::stringstream l_sizeStream(l_attr.as_string("0.0 0.0 0.0"));
                        l_sizeStream >> l_chainPartData.m_sizeX >> l_chainPartData.m_sizeY >> l_chainPartData.m_sizeZ;

                        l_attr = l_part.attribute("offset");
                        if(!l_attr) Error("No 'offset' attribute at part node\n");
                        std::stringstream l_offsetStream(l_attr.as_string("0.0 0.0 0.0"));
                        l_offsetStream >> l_chainPartData.m_offsetX >> l_chainPartData.m_offsetY >> l_chainPartData.m_offsetZ;

                        l_attr = l_part.attribute("rotation");
                        if(!l_attr) Error("No 'rotation' attribute at part node\n");
                        std::stringstream l_rotationStream(l_attr.as_string("0.0 0.0 0.0 1.0"));
                        l_rotationStream >> l_chainPartData.m_rotationX >> l_chainPartData.m_rotationY >> l_chainPartData.m_rotationZ >> l_chainPartData.m_rotationW;

                        l_attr = l_part.attribute("mass");
                        if(!l_attr) Error("No 'mass' attribute at part node\n");
                        l_chainPartData.m_mass = l_attr.as_float(1.f);

                        l_attr = l_part.attribute("restutition");
                        if(!l_attr) Error("No 'restutition' attribute at part node\n");
                        l_chainPartData.m_restutition = l_attr.as_float(0.f);

                        l_attr = l_part.attribute("friction");
                        if(!l_attr) Error("No 'friction' attribute at part node\n");
                        l_chainPartData.m_friction = l_attr.as_float(0.5f);

                        l_attr = l_part.attribute("damping");
                        if(!l_attr) Error("No 'damping' attribute at part node\n");
                        std::stringstream l_dampingStream(l_attr.as_string("0.0 0.0"));
                        l_dampingStream >> l_chainPartData.m_linearDamping >> l_chainPartData.m_angularDamping;

                        l_attr = l_part.attribute("lowAngLim");
                        if(!l_attr) Error("No 'lowAngLim' attribute at part node\n");
                        std::stringstream l_lowAngLimStream(l_attr.as_string("0.0 0.0 0.0"));
                        l_lowAngLimStream >> l_chainPartData.m_lowerAngularLimitX >> l_chainPartData.m_lowerAngularLimitY >> l_chainPartData.m_lowerAngularLimitZ;

                        l_attr = l_part.attribute("uppAngLim");
                        if(!l_attr) Error("No 'uppAngLim' attribute at part node\n");
                        std::stringstream l_uppAngLimStream(l_attr.as_string("0.0 0.0 0.0"));
                        l_uppAngLimStream >> l_chainPartData.m_upperAngularLimitX >> l_chainPartData.m_upperAngularLimitY >> l_chainPartData.m_upperAngularLimitZ;

                        l_attr = l_part.attribute("angStiff");
                        if(!l_attr) Error("No 'angStiff' attribute at part node\n");
                        std::stringstream l_angStiffStream(l_attr.as_string("0.0 0.0 0.0"));
                        l_angStiffStream >> l_chainPartData.m_angularStiffnessX >> l_chainPartData.m_angularStiffnessY >> l_chainPartData.m_angularStiffnessZ;

                        l_attr = l_part.attribute("lowLinLim");
                        if(!l_attr) Error("No 'lowLinLim' attribute at part node\n");
                        std::stringstream l_lowLinLimStream(l_attr.as_string("0.0 0.0 0.0"));
                        l_lowLinLimStream >> l_chainPartData.m_lowerLinearLimitX >> l_chainPartData.m_lowerLinearLimitY >> l_chainPartData.m_lowerLinearLimitZ;

                        l_attr = l_part.attribute("uppLinLim");
                        if(!l_attr) Error("No 'uppLinLim' attribute at part node\n");
                        std::stringstream l_uppLinLimStream(l_attr.as_string("0.0 0.0 0.0"));
                        l_uppLinLimStream >> l_chainPartData.m_upperLinearLimitX >> l_chainPartData.m_upperLinearLimitY >> l_chainPartData.m_upperLinearLimitZ;

                        l_attr = l_part.attribute("linStiff");
                        if(!l_attr) Error("No 'linStiff' attribute at part node\n");
                        std::stringstream l_linStiffStream(l_attr.as_string("0.0 0.0 0.0"));
                        l_linStiffStream >> l_chainPartData.m_linearStiffnessX >> l_chainPartData.m_linearStiffnessY >> l_chainPartData.m_linearStiffnessZ;

                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_boneID), sizeof(int));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_type), sizeof(unsigned char));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_sizeX), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_sizeY), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_sizeZ), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_offsetX), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_offsetY), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_offsetZ), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_rotationX), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_rotationY), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_rotationZ), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_rotationW), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_mass), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_restutition), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_friction), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_linearDamping), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_angularDamping), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_lowerAngularLimitX), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_lowerAngularLimitY), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_lowerAngularLimitZ), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_upperAngularLimitX), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_upperAngularLimitY), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_upperAngularLimitZ), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_angularStiffnessX), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_angularStiffnessY), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_angularStiffnessZ), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_lowerLinearLimitX), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_lowerLinearLimitY), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_lowerLinearLimitZ), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_upperLinearLimitX), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_upperLinearLimitY), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_upperLinearLimitZ), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_linearStiffnessX), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_linearStiffnessY), sizeof(float));
                        l_file.write(reinterpret_cast<char*>(&l_chainPartData.m_linearStiffnessZ), sizeof(float));

                        l_chainPartsCount--;
                    }
                    l_chainsCount--;
                }
            }
            else Error("No 'count' or 'bone' attribute at 'chains' subnode");
        }
        else
        {
            l_file.write(reinterpret_cast<char*>(&l_chainsCount), sizeof(unsigned int));
            Info("No data of bone chains");
        }
        l_file.flush();
        l_file.close();
        std::cout << "File converted" << std::endl;
    }
    else Info("Usage: [file path]");
    std::getchar();
    return EXIT_SUCCESS;
}