#include "stdafx.h"
#include "Utils/zlibUtils.h"

namespace zlibUtils
{

int CompressData(void *f_src, int f_srcLen, void *f_dest, int f_destLen)
{
    z_stream zInfo = { 0 };
    zInfo.total_in = zInfo.avail_in = f_srcLen;
    zInfo.total_out = zInfo.avail_out = f_destLen;
    zInfo.next_in = reinterpret_cast<unsigned char*>(f_src);
    zInfo.next_out = reinterpret_cast<unsigned char*>(f_dest);

    int l_ret = -1;
    if(deflateInit(&zInfo, Z_DEFAULT_COMPRESSION) == Z_OK)
    {
        if(deflate(&zInfo, Z_FINISH) == Z_STREAM_END) l_ret = zInfo.total_out;
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
