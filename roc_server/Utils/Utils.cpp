#include "stdafx.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Utils
{

std::regex g_upRegex("(\\.\\.)+(\\/|\\\\)");

void AnalyzePath(const std::string &f_in, std::string &f_out)
{
    std::regex_replace(std::back_inserter(f_out), f_in.begin(), f_in.end(), g_upRegex, "");
}
void JoinPaths(std::string &f_result, const std::string &f_path)
{
    f_result.push_back('/');
    f_result.append(f_path);
}
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
