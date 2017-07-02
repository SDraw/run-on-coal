#define _CRT_SECURE_NO_WARNINGS
#include "Animation.h"

int main(int argc, char* argv[])
{
    if(argc < 2) std::cout << "Usage: [input file] <[output file]>" << std::endl;
    else
    {
        std::string l_inputFile(argv[1]);
        std::string l_outputFile;
        if(argc >= 3) l_outputFile.assign(argv[2]);
        else
        {
            l_outputFile.assign(l_inputFile);
            l_outputFile.append(".raf");
        }
        Animation *l_anim = new Animation();
        if(l_anim->LoadFromJSON(l_inputFile)) l_anim->GenerateBinary(l_outputFile);
        delete l_anim;
        std::getchar();
    }
    return EXIT_SUCCESS;
}