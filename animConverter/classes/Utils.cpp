#include "stdafx.h"
#include "classes/Utils.h"

namespace Utils
{

bool ReadFile(std::string &path, std::string &f_cont)
{
    std::ifstream l_file;
    l_file.open(path,std::ios::in);
    if(l_file.fail()) return false;

    std::ostringstream l_data;
    l_data << l_file.rdbuf();
    l_file.close();
    f_cont.append(l_data.str());
    return true;
}

}
