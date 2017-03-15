#pragma once

namespace ROC
{
namespace Utils
{

void AnalyzePath(const std::string &f_in, std::string &f_out);
void JoinPaths(std::string &f_result, const std::string &f_path);
int ReadEnumVector(const std::vector<std::string> &f_vec, const std::string &f_val);

}
}
