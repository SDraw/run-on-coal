#include "stdafx.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Utils
{

namespace Path
{

const std::regex g_UpRegex("(\\.\\.)+(\\/|\\\\)");
void EscapePath(std::string &f_path)
{
    std::string l_result;
    std::regex_replace(std::back_inserter(l_result), f_path.begin(), f_path.end(), g_UpRegex, "");
    f_path.assign(l_result);
}

}

namespace Enum
{

int ReadEnumVector(const std::vector<std::string> &f_vec, const std::string &f_val)
{
    int l_result = -1;
    for(auto iter = f_vec.begin(), iterEnd = f_vec.end(); iter != iterEnd; ++iter)
    {
        if(!iter->compare(f_val))
        {
            l_result = iter - f_vec.begin();
            break;
        }
    }
    return l_result;
}

}

}
}
