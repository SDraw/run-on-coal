#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/LogManager.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Utils
{

bool ReadFile(std::string &path,std::string &f_cont)
{
    std::ifstream l_file;
    l_file.open(path,std::ios::in);
    if(l_file.fail()) return false;

    std::ostringstream l_data;
    l_data<<l_file.rdbuf();
    l_file.close();
    f_cont.append(l_data.str());
    return true;
}

GLint CheckShader(GLuint f_shader)
{
    GLint l_params;
    glGetShaderiv(f_shader,GL_COMPILE_STATUS,&l_params);
    return l_params;
}

int ReadEnumString(std::string &f_val,const std::string &f_enum)
{
    size_t first = f_enum.find(f_val);
    if(first==std::string::npos) return -1;
    return std::count(f_enum.begin(),f_enum.begin()+first,',');
}
unsigned long GetSystemTick()
{
#ifdef _WIN32
    return GetTickCount();
#elif __linux__
    struct timeval tv;
    if(gettimeofday(&tv,NULL)!=0) return 0;
    return (tv.tv_sec*1000)+(tv.tv_usec/1000);
#endif
}

void JoinPaths(std::string &f_result,std::string &f_path)
{
    f_result.push_back('/');
    f_result.append(f_path);
}

int CompressData(void *f_src,int f_srcLen,void *f_dest,int f_destLen)
{
    z_stream zInfo = {0};
    zInfo.total_in = zInfo.avail_in = f_srcLen;
    zInfo.total_out = zInfo.avail_out = f_destLen;
    zInfo.next_in = (unsigned char*)f_src;
    zInfo.next_out = (unsigned char*)f_dest;

    int l_error,l_ret = -1;
    l_error = deflateInit(&zInfo,Z_DEFAULT_COMPRESSION);
    if(l_error==Z_OK)
    {
        l_error = deflate(&zInfo,Z_FINISH);
        if(l_error==Z_STREAM_END) l_ret = zInfo.total_out;
    }
    deflateEnd(&zInfo);
    return l_ret;
}
int UncompressData(void *f_src,int f_srcLen,void *f_dest,int f_destLen)
{
    z_stream zInfo = {0};
    zInfo.total_in = zInfo.avail_in = f_srcLen;
    zInfo.total_out = zInfo.avail_out = f_destLen;
    zInfo.next_in = (unsigned char*)f_src;
    zInfo.next_out = (unsigned char*)f_dest;

    int l_error,l_ret = -1;
    l_error = inflateInit(&zInfo);
    if(l_error==Z_OK)
    {
        l_error = inflate(&zInfo,Z_FINISH);
        if(l_error==Z_STREAM_END) l_ret = zInfo.total_out;
    }
    inflateEnd(&zInfo);
    return l_ret;
}
int GetMaxCompressedLen(int nLenSrc)
{
    return (nLenSrc+6+((nLenSrc+16383)/16384*5));
}

}
}
