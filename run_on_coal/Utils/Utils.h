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

namespace zlib
{

int CompressData(void *f_src, int f_srcLen, void *f_dest, int f_destLen);
int UncompressData(void *f_src, int f_srcLen, void *f_dest, int f_destLen);
int GetMaxCompressedLen(int nLenSrc);

}

namespace Math
{

bool IsPowerOfTwo(int f_value);
float EaseInOut(float f_value);

}

}
}
