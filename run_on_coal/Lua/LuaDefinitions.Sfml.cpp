#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/RenderManager.h"
#include "Managers/SfmlManager.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Sfml.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Lua
{
std::vector<std::string> g_keysTable
{
    "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
    "esc", "lctrl", "lshift", "lalt", "lsys", "rctrl", "rshift", "ralt", "rsys", "menu",
    "[", "]", ";", ",", ".", "'", "/", "\\", "~", "=", "-",
    "space", "return", "backspace", "tab", "pgup", "pgdn", "end", "home", "insert", "delete",
    "num+", "num-", "num*", "num/",
    "arrow_l", "arrow_r", "arrow_u", "arrow_d",
    "num0", "num1", "num2", "num3", "num4", "num5", "num6", "num7", "num8", "num9",
    "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15",
    "pause"
};
std::vector<std::string> g_mouseKeysTable
{
    "left", "right", "middle",
    "x1", "x2"
};
std::vector<std::string> g_cursorLocksTable
{
    "hu", "hl", "vu", "vl"
};

int setCursorMode(lua_State *f_vm)
{
    std::string l_state;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadText(l_state);
    if(argStream.HasErrors() || l_state.empty())
    {
        lua_pushboolean(f_vm, 0);
        return 1;
    }
    int l_type = Utils::ReadEnumVector(g_cursorLocksTable, l_state);
    if(l_type == -1)
    {
        lua_pushboolean(f_vm, 0);
        return 1;
    }
    LuaManager::m_corePointer->GetSfmlManager()->SetCursorMode(static_cast<unsigned char>(l_type));
    lua_pushboolean(f_vm, 1);
    return 1;
}
int getCursorPosition(lua_State *f_vm)
{
    glm::ivec2 l_pos;
    LuaManager::m_corePointer->GetSfmlManager()->GetCursorPosition(l_pos);
    lua_pushnumber(f_vm, l_pos.x);
    lua_pushnumber(f_vm, l_pos.y);
    return 2;
}
int setCursorPosition(lua_State *f_vm)
{
    LUA_INTEGER l_val[2];
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    for(int i = 0; i < 2; i++) argStream.ReadInteger(l_val[i]);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm, 0);
        return 1;
    }
    glm::ivec2 l_pos(l_val[0], l_val[1]);
    LuaManager::m_corePointer->GetSfmlManager()->SetCursorPosition(l_pos);
    lua_pushboolean(f_vm, 1);
    return 1;
}

int getWindowPosition(lua_State *f_vm)
{
    glm::ivec2 l_pos;
    LuaManager::m_corePointer->GetSfmlManager()->GetWindowPosition(l_pos);
    lua_pushinteger(f_vm, l_pos.x);
    lua_pushinteger(f_vm, l_pos.y);
    return 2;
}
int getWindowSize(lua_State *f_vm)
{
    glm::ivec2 l_size;
    LuaManager::m_corePointer->GetSfmlManager()->GetWindowSize(l_size);
    lua_pushinteger(f_vm, l_size.x);
    lua_pushinteger(f_vm, l_size.y);
    return 2;
}
int closeApp(lua_State *f_vm)
{
    LuaManager::m_corePointer->GetSfmlManager()->CloseWindow();
    lua_pushboolean(f_vm, 1);
    return 1;
}

int getTime(lua_State *f_vm)
{
    float l_time = LuaManager::m_corePointer->GetSfmlManager()->GetTime();
    lua_pushnumber(f_vm, static_cast<lua_Number>(l_time));
    return 1;
}

int isKeyPressed(lua_State *f_vm)
{
    std::string l_key;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadText(l_key);
    if(argStream.HasErrors() || l_key.empty())
    {
        lua_pushboolean(f_vm, 0);
        return 1;
    }
    int l_numKey = Utils::ReadEnumVector(g_keysTable, l_key);
    if(l_numKey == -1)
    {
        lua_pushboolean(f_vm, 0);
        return 1;
    }
    bool l_result = LuaManager::m_corePointer->GetSfmlManager()->IsKeyPressed(static_cast<int>(l_numKey));
    lua_pushboolean(f_vm, l_result);
    return 1;
}
int isMouseKeyPressed(lua_State *f_vm)
{
    std::string l_key;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadText(l_key);
    if(argStream.HasErrors() || l_key.empty())
    {
        lua_pushboolean(f_vm, 0);
        return 1;
    }
    int l_numKey = Utils::ReadEnumVector(g_mouseKeysTable, l_key);
    if(l_numKey == -1)
    {
        lua_pushboolean(f_vm, 0);
        return 1;
    }
    bool l_result = LuaManager::m_corePointer->GetSfmlManager()->IsMouseKeyPressed(static_cast<int>(l_numKey));
    lua_pushboolean(f_vm, l_result);
    return 1;
}
}
}