#pragma once

namespace zlibUtils
{

unsigned int CompressData(const void *f_src, unsigned int f_srcLen, void *f_dest, unsigned int f_destLen);
unsigned int UncompressData(const void *f_src, unsigned int f_srcLen, void *f_dest, unsigned int f_destLen);
unsigned int GetMaxCompressedLen(unsigned int nLenSrc);

}
