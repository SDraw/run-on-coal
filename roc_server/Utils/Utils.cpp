#include "stdafx.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Utils
{

int ReadEnumVector(const std::vector<std::string> &f_vec, std::string &f_val)
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
