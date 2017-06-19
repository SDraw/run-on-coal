#pragma once

namespace ROC
{
namespace Utils
{

namespace Path
{

void EscapePath(std::string &f_path);

}

namespace Enum
{

int ReadEnumVector(const std::vector<std::string> &f_vec, const std::string &f_val);

}

}
}
