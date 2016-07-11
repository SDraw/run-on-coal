#pragma once

namespace ROC
{
namespace Utils
{

bool ReadFile(std::string &path, std::string &f_cont);
GLint CheckShader(GLuint f_shader);
void GetScreenPosition(glm::vec3 &pos, glm::mat4 &proj, glm::mat4 &view, glm::ivec2 &window, glm::vec2 &out);
int ReadEnumString(std::string &f_val, const std::string &f_enum);
unsigned long GetSystemTick();
void PathsJoin(std::string &f_out, std::string &f_path1, std::string &f_path2);

int CompressData(void *f_src, int f_srcLen, void *f_dest, int f_destLen);
int UncompressData(void *f_src, int f_srcLen, void *f_dest, int f_destLen);
int GetMaxCompressedLen(int nLenSrc);

}
}
