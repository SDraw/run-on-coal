#include "stdafx.h"

#include "Utils.h"

unsigned int CompressData(const void *f_src, unsigned int f_srcLen, void *f_dest, unsigned int f_destLen)
{
    z_stream zInfo = { 0 };
    zInfo.total_in = zInfo.avail_in = f_srcLen;
    zInfo.total_out = zInfo.avail_out = f_destLen;
    zInfo.next_in = static_cast<unsigned char*>(const_cast<void*>(f_src));
    zInfo.next_out = static_cast<unsigned char*>(f_dest);

    int l_error;
    unsigned int l_ret = 0U;
    l_error = deflateInit(&zInfo, Z_DEFAULT_COMPRESSION);
    if(l_error == Z_OK)
    {
        l_error = deflate(&zInfo, Z_FINISH);
        if(l_error == Z_STREAM_END) l_ret = zInfo.total_out;
    }
    deflateEnd(&zInfo);
    return l_ret;
}

unsigned int GetMaxCompressedLen(unsigned int nLenSrc)
{
    unsigned int n16kBlocks = (nLenSrc + 16383) / 16384;
    return (nLenSrc + 6 + (n16kBlocks * 5));
}
