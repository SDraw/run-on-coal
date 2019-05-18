#include "stdafx.h"

#include "Model.h"

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
