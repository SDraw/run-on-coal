#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtx/compatibility.hpp"

#include "Animation.h"

int main(int argc, char* argv[])
{
    if(argc > 1)
    {
        std::string l_inputFile(argv[1]);
        std::string l_outputFile(l_inputFile);
        l_outputFile.append(".raf");

        Animation *l_anim = new Animation();
        if(l_anim->Load(l_inputFile)) l_anim->Generate(l_outputFile);
        std::getchar();
        delete l_anim;
    }
    else std::cout << "Usage: [input file]" << std::endl;
    std::getchar();
    return EXIT_SUCCESS;
}