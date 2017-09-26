#include "stdafx.h"

namespace ROC
{

extern const glm::mat4 g_IdentityMatrix(1.f);
extern const glm::mat4 g_EmptyMat4(0.f);
extern const glm::vec3 g_DefaultPosition(0.f);
extern const glm::vec3 g_EmptyVec3(0.f);
extern const glm::vec4 g_EmptyVec4(0.f);
extern const glm::vec3 g_DefaultScale(1.f, 1.f, 1.f);
extern const glm::quat g_DefaultRotation(1.f, 0.f, 0.f, 0.f);
extern const float g_Epsilon = glm::epsilon<float>();

extern const std::vector<std::string> g_FilteringTypesTable
{
    "nearest", "linear"
};

extern const std::vector<std::string> g_KeyNamesTable
{
    "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
    "esc", "lctrl", "lshift", "lalt", "lsys", "rctrl", "rshift", "ralt", "rsys", "menu",
    "lbracket", "rbracket", "semicolon", "comma", "period", "quote", "slash", "backslash", "tilde", "equal", "dash",
    "space", "return", "backspace", "tab", "pgup", "pgdn", "end", "home", "insert", "delete",
    "num+", "num-", "num*", "num/",
    "arrow_l", "arrow_r", "arrow_u", "arrow_d",
    "num0", "num1", "num2", "num3", "num4", "num5", "num6", "num7", "num8", "num9",
    "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15",
    "pause"
};
extern const std::vector<std::string> g_MouseKeyNamesTable
{
    "left", "right", "middle",
    "x1", "x2"
};
extern const std::vector<std::string> g_JoypadAxisNamesTable
{
    "X", "Y", "Z",
    "R", "U", "V",
    "PovX", "PovY"
};

extern const float g_QuadVertexUV[] = {
    0.f, 0.f, 0.f, 1.f, 1.f, 1.f,
    0.f, 0.f, 1.f, 1.f, 1.f, 0.f
};

}
