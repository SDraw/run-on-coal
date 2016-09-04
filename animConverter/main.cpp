#include "stdafx.h"
#include "classes/Animation.h"

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        std::cout << "Usage: [input file] <[output file]>" << std::endl;
        return EXIT_SUCCESS;
    }
    std::string l_inputFile(argv[1]);
    std::string l_outputFile(argc >= 3 ? argv[2] : l_inputFile + ".raf");
    Animation *l_anim = new Animation();
    l_anim->LoadFromJSON(l_inputFile);
    l_anim->GenerateBinary(l_outputFile);
    delete l_anim;
    std::getchar();
    return EXIT_SUCCESS;
}