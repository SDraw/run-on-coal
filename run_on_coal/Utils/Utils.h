#pragma once

namespace ROC
{
namespace Utils
{
bool ReadFile(std::string &path, std::string &f_cont);

GLint CheckShader(GLuint f_shader);

unsigned long GetSystemTick();

void JoinPaths(std::string &f_result, std::string &f_path);
int ReadEnumVector(std::vector<std::string> &f_vec, std::string &f_val);

int CompressData(void *f_src, int f_srcLen, void *f_dest, int f_destLen);
int UncompressData(void *f_src, int f_srcLen, void *f_dest, int f_destLen);
int GetMaxCompressedLen(int nLenSrc);
}
}
