#include "stdafx.h"

#include "Utils/EnumUtils.h"

namespace EnumUtils
{

int ReadEnumVector(const std::string &f_val, const std::vector<std::string> &f_vec)
{
    int l_result = -1;
    for(auto l_searchIter = f_vec.begin(), l_searchEnd = f_vec.end(); l_searchIter != l_searchEnd; ++l_searchIter)
    {
        if(!l_searchIter->compare(f_val))
        {
            l_result = static_cast<int>(std::distance(f_vec.begin(),l_searchIter));
            break;
        }
    }
    return l_result;
}

}