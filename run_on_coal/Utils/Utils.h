#pragma once

namespace ROC
{
namespace Utils
{

void AnalyzePath(std::string &f_in, std::string &f_out);
void JoinPaths(std::string &f_result, std::string &f_path);
int ReadEnumVector(const std::vector<std::string> &f_vec, std::string &f_val);

int CompressData(void *f_src, int f_srcLen, void *f_dest, int f_destLen);
int UncompressData(void *f_src, int f_srcLen, void *f_dest, int f_destLen);
int GetMaxCompressedLen(int nLenSrc);
}
}
