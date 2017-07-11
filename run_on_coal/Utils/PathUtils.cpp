#include "stdafx.h"

#include "Utils/PathUtils.h"

namespace PathUtils
{

const std::regex g_UpRegex("(\\.\\.)+(\\/|\\\\)");

void EscapePath(std::string &f_path)
{
    std::string l_result;
    std::regex_replace(std::back_inserter(l_result), f_path.begin(), f_path.end(), g_UpRegex, "");
    f_path.assign(l_result);
}

}
