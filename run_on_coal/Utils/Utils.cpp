#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/LogManager.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Utils
{

std::regex g_upRegex("(\\.\\.)+(\\/|\\\\)");

bool ReadFile(std::string &path, std::string &f_cont)
{
    std::ifstream l_file;
    l_file.open(path, std::ios::in);
    if(l_file.fail()) return false;

    std::istreambuf_iterator<char> l_inputIt(l_file), l_emptyInputIt;
    std::back_insert_iterator<std::string> l_stringInsert(f_cont);
    std::copy(l_inputIt, l_emptyInputIt, l_stringInsert);
    return true;
}

GLint CheckShader(GLuint f_shader)
{
    GLint l_params;
    glGetShaderiv(f_shader, GL_COMPILE_STATUS, &l_params);
    return l_params;
}
void GetShaderInfoLog(GLuint f_shader, std::string &f_log)
{
    GLint l_logSize = 0;
    glGetShaderiv(f_shader, GL_INFO_LOG_LENGTH, &l_logSize);
    f_log.resize(l_logSize);
    glGetShaderInfoLog(f_shader, l_logSize, &l_logSize, (GLchar*)f_log.data());
}

int ReadEnumVector(const std::vector<std::string> &f_vec, std::string &f_val)
{
    int l_return = -1;
    for(std::vector<std::string>::const_iterator iter = f_vec.begin(); iter != f_vec.end(); ++iter)
    {
        if(!iter->compare(f_val))
        {
            l_return = iter - f_vec.begin();
            break;
        }
    }
    return l_return;
}

void AnalyzePath(std::string &f_in, std::string &f_out)
{
    std::regex_replace(std::back_inserter(f_out), f_in.begin(), f_in.end(), g_upRegex, "");
}
void JoinPaths(std::string &f_result, std::string &f_path)
{
    f_result.push_back('/');
    f_result.insert(f_result.end(), f_path.begin(), f_path.end());
}

int CompressData(void *f_src, int f_srcLen, void *f_dest, int f_destLen)
{
    z_stream zInfo = { 0 };
    zInfo.total_in = zInfo.avail_in = f_srcLen;
    zInfo.total_out = zInfo.avail_out = f_destLen;
    zInfo.next_in = (unsigned char*)f_src;
    zInfo.next_out = (unsigned char*)f_dest;

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
    zInfo.next_in = (unsigned char*)f_src;
    zInfo.next_out = (unsigned char*)f_dest;

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
}