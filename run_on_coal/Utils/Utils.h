#pragma once

namespace ROC
{
namespace Utils
{

void EscapePath(std::string &f_path);
int ReadEnumVector(const std::vector<std::string> &f_vec, const std::string &f_val);

int CompressData(void *f_src, int f_srcLen, void *f_dest, int f_destLen);
int UncompressData(void *f_src, int f_srcLen, void *f_dest, int f_destLen);
int GetMaxCompressedLen(int nLenSrc);

bool IsPowerOfTwo(int f_value);
float EaseInOut(float f_value);

}
}
