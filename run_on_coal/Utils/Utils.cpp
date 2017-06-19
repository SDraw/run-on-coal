#include "stdafx.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Utils
{

namespace Path
{

const std::regex g_UpRegex("(\\.\\.)+(\\/|\\\\)");
void EscapePath(std::string &f_path)
{
    std::string l_result;
    std::regex_replace(std::back_inserter(l_result), f_path.begin(), f_path.end(), g_UpRegex, "");
    f_path.assign(l_result);
}

}

namespace Enum
{

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

namespace zlib
{

int CompressData(void *f_src, int f_srcLen, void *f_dest, int f_destLen)
{
    z_stream zInfo = { 0 };
    zInfo.total_in = zInfo.avail_in = f_srcLen;
    zInfo.total_out = zInfo.avail_out = f_destLen;
    zInfo.next_in = reinterpret_cast<unsigned char*>(f_src);
    zInfo.next_out = reinterpret_cast<unsigned char*>(f_dest);

    int l_error, l_ret = -1;
    l_error = deflateInit(&zInfo, Z_DEFAULT_COMPRESSION);
    if(l_error == Z_OK)
    {
        l_error = deflate(&zInfo, Z_FINISH);
        if(l_error == Z_STREAM_END) l_ret = zInfo.total_out;
    }
    deflateEnd(&zInfo);
    return l_ret;
}
int UncompressData(void *f_src, int f_srcLen, void *f_dest, int f_destLen)
{
    z_stream zInfo = { 0 };
    zInfo.total_in = zInfo.avail_in = f_srcLen;
    zInfo.total_out = zInfo.avail_out = f_destLen;
    zInfo.next_in = reinterpret_cast<unsigned char*>(f_src);
    zInfo.next_out = reinterpret_cast<unsigned char*>(f_dest);

    int l_error, l_ret = -1;
    l_error = inflateInit(&zInfo);
    if(l_error == Z_OK)
    {
        l_error = inflate(&zInfo, Z_FINISH);
        if(l_error == Z_STREAM_END) l_ret = zInfo.total_out;
    }
    inflateEnd(&zInfo);
    return l_ret;
}
int GetMaxCompressedLen(int nLenSrc)
{
    return (nLenSrc + 6 + ((nLenSrc + 16383) / 16384 * 5));
}

}

namespace Math
{

bool IsPowerOfTwo(int f_value)
{
    return (f_value > 0 && ((f_value & (f_value - 1)) == 0));
}
float EaseInOut(float f_value)
{
    return -0.5f*(cos(3.141592f*f_value) - 1.f);
}

}

}
}
