#include <iostream>
#include <fstream>
#include <string>
#include "pugixml.hpp"

#define Error(first) { std::cout << first << std::endl; return EXIT_FAILURE; }
int main(int argc, char *argv[])
{
    if(argc < 2) return EXIT_FAILURE;
    pugi::xml_document l_meta;
    if(!l_meta.load_file(argv[1])) Error("Not XML file\n");
    pugi::xml_node l_root = l_meta.child("chains");
    if(!l_root) Error("No 'chains' root\n");
    std::ofstream l_file(std::string(argv[1]) + ".bin", std::ios::out | std::ios::binary);
    if(l_file.fail()) Error("Unable to create binary file\n");

    pugi::xml_attribute l_attr = l_root.attribute("count");
    if(!l_attr) Error("No 'count' attribute at root node\n");
    unsigned int l_val = l_attr.as_uint();
    l_file.write((char*)&l_val, sizeof(unsigned int));

    for(pugi::xml_node l_chain = l_root.child("chain"); l_chain; l_chain = l_chain.next_sibling("chain"))
    {
        l_attr = l_chain.attribute("count");
        if(!l_attr) Error("No 'count' attribute at chain node\n");
        l_val = l_attr.as_uint();
        l_file.write((char*)&l_val, sizeof(unsigned int));
        for(pugi::xml_node l_part = l_chain.child("part"); l_part; l_part = l_part.next_sibling("part"))
        {
            l_attr = l_part.attribute("bType");
            if(!l_attr) Error("No 'bType' attribute at part node\n");
            unsigned char l_type = l_attr.as_uint();
            l_file.write((char*)&l_type, sizeof(unsigned char));

            l_attr = l_part.attribute("mass");
            if(!l_attr) Error("No 'mass' attribute at part node\n");
            float l_fVal = l_attr.as_float();
            l_file.write((char*)&l_fVal, sizeof(float));

            l_attr = l_part.attribute("sx");
            if(!l_attr) Error("No 'sx' attribute at part node\n");
            l_fVal = l_attr.as_float();
            l_file.write((char*)&l_fVal, sizeof(float));

            l_attr = l_part.attribute("sy");
            if(!l_attr) Error("No 'sy' attribute at part node\n");
            l_fVal = l_attr.as_float();
            l_file.write((char*)&l_fVal, sizeof(float));

            l_attr = l_part.attribute("sz");
            if(!l_attr) Error("No 'sz' attribute at part node\n");
            l_fVal = l_attr.as_float();
            l_file.write((char*)&l_fVal, sizeof(float));

            l_attr = l_part.attribute("boneID");
            if(!l_attr) Error("No 'boneID' attribute at part node\n");
            l_val = l_attr.as_uint();
            l_file.write((char*)&l_val, sizeof(unsigned int));
        }
    }
    l_file.flush();
    l_file.close();
    std::cout << "File converted" << std::endl;
    getchar();
    return EXIT_SUCCESS;
}