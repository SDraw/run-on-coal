#include "stdafx.h"

#include "Utils/EnumUtils.h"

namespace EnumUtils
{

size_t ReadEnumVector(const std::string &f_val, const std::vector<std::string> &f_vec)
{
    size_t l_result = std::numeric_limits<size_t>::max();
    for(auto l_searchIter = f_vec.begin(), l_searchEnd = f_vec.end(); l_searchIter != l_searchEnd; ++l_searchIter)
    {
        if(!l_searchIter->compare(f_val))
        {
            l_result = std::distance(f_vec.begin(),l_searchIter);
            break;
        }
    }
    return l_result;
}
size_t ReadEnumVector(const char *f_val, const std::vector<std::string> &f_vec)
{
    size_t l_result = std::numeric_limits<size_t>::max();
    for(auto l_searchIter = f_vec.begin(), l_searchEnd = f_vec.end(); l_searchIter != l_searchEnd; ++l_searchIter)
    {
        if(!l_searchIter->compare(f_val))
        {
            l_result = std::distance(f_vec.begin(),l_searchIter);
            break;
        }
    }
    return l_result;
}

}