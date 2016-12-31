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
extern const std::vector<std::string> g_keysTable;
extern const std::vector<std::string> g_mouseKeysTable;
extern const std::vector<std::string> g_axisNames;

namespace Lua
{

const std::vector<std::string> g_cursorLocksTable
{
    "hu", "hl", "vu", "vl"
};

int setCursorMode(lua_State *f_vm)
{
    std::string l_state;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadText(l_state);
    if(!argStream.HasErrors() && !l_state.empty())
    {
        int l_type = Utils::ReadEnumVector(g_cursorLocksTable, l_state);
        if(l_type != -1)
        {
            LuaManager::m_corePointer->GetSfmlManager()->SetCursorMode(static_cast<unsigned char>(l_type));
            lua_pushboolean(f_vm, 1);
        }
        else lua_pushboolean(f_vm, 0);
    }
    else lua_pushboolean(f_vm, 0);
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
    glm::ivec2 l_pos;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    for(int i = 0; i < 2; i++) argStream.ReadInteger(l_pos[i]);
    if(!argStream.HasErrors())
    {
        LuaManager::m_corePointer->GetSfmlManager()->SetCursorPosition(l_pos);
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
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
    lua_pushnumber(f_vm, LuaManager::m_corePointer->GetSfmlManager()->GetTime());
    return 1;
}

int isKeyPressed(lua_State *f_vm)
{
    std::string l_key;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadText(l_key);
    if(!argStream.HasErrors() && !l_key.empty())
    {
        int l_numKey = Utils::ReadEnumVector(g_keysTable, l_key);
        if(l_numKey != -1)
        {
            bool l_result = LuaManager::m_corePointer->GetSfmlManager()->IsKeyPressed(l_numKey);
            lua_pushboolean(f_vm, l_result);
        }
        else lua_pushboolean(f_vm, 0);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int isMouseKeyPressed(lua_State *f_vm)
{
    std::string l_key;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadText(l_key);
    if(!argStream.HasErrors() && !l_key.empty())
    {
        int l_numKey = Utils::ReadEnumVector(g_mouseKeysTable, l_key);
        if(l_numKey != -1)
        {
            bool l_result = LuaManager::m_corePointer->GetSfmlManager()->IsMouseKeyPressed(l_numKey);
            lua_pushboolean(f_vm, l_result);
        }
        else lua_pushboolean(f_vm, 0);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}

int setWindowVSync(lua_State *f_vm)
{
    bool l_sync;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadBoolean(l_sync);
    if(!argStream.HasErrors())
    {
        LuaManager::m_corePointer->GetSfmlManager()->SetVSync(l_sync);
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int setWindowFramelimit(lua_State *f_vm)
{
    unsigned int l_fps;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadInteger(l_fps);
    if(!argStream.HasErrors())
    {
        LuaManager::m_corePointer->GetSfmlManager()->SetFramelimit(l_fps);
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int setWindowTitle(lua_State *f_vm)
{
    std::string l_title;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadText(l_title);
    if(!argStream.HasErrors())
    {
        sf::String l_title32 = sf::String::fromUtf8(l_title.begin(), l_title.end());
        LuaManager::m_corePointer->GetSfmlManager()->SetTitle(l_title32);
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int setWindowIcon(lua_State *f_vm)
{
    std::string l_path;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadText(l_path);
    if(!argStream.HasErrors() && !l_path.empty())
    {
        bool l_result = LuaManager::m_corePointer->GetSfmlManager()->SetIcon(l_path);
        lua_pushboolean(f_vm, l_result);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int requestWindowFocus(lua_State *f_vm)
{
    LuaManager::m_corePointer->GetSfmlManager()->RequestFocus();
    lua_pushboolean(f_vm, 1);
    return 1;
}
int getWindowFocus(lua_State *f_vm)
{
    bool l_focus = LuaManager::m_corePointer->GetSfmlManager()->GetFocusState();
    lua_pushboolean(f_vm, l_focus);
    return 1;
}

int isJoypadConnected(lua_State *f_vm)
{
    unsigned int l_joypad;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadInteger(l_joypad);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::m_corePointer->GetSfmlManager()->IsJoypadConnected(l_joypad);
        lua_pushboolean(f_vm, l_result);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int joypadGetButtonCount(lua_State *f_vm)
{
    unsigned int l_joypad;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadInteger(l_joypad);
    if(!argStream.HasErrors())
    {
        unsigned int l_count = LuaManager::m_corePointer->GetSfmlManager()->GetJoypadButtonCount(l_joypad);
        lua_pushinteger(f_vm, static_cast<lua_Integer>(l_count));
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int joypadGetButtonState(lua_State *f_vm)
{
    unsigned int l_joypad, l_button;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadInteger(l_joypad);
    argStream.ReadInteger(l_button);
    if(!argStream.HasErrors())
    {
        bool l_state = LuaManager::m_corePointer->GetSfmlManager()->GetJoypadButtonState(l_joypad, l_button);
        lua_pushboolean(f_vm, l_state);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int joypadHasAxis(lua_State *f_vm)
{
    unsigned int l_joypad;
    std::string l_axis;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadInteger(l_joypad);
    argStream.ReadText(l_axis);
    if(!argStream.HasErrors() && !l_axis.empty())
    {
        int l_axisID = Utils::ReadEnumVector(g_axisNames, l_axis);
        if(l_axisID != -1)
        {
            bool l_result = LuaManager::m_corePointer->GetSfmlManager()->CheckJoypadAxis(l_joypad, static_cast<unsigned int>(l_axisID));
            lua_pushboolean(f_vm, l_result);
        }
        else lua_pushboolean(f_vm, 0);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int joypadGetAxisValue(lua_State *f_vm)
{
    unsigned int l_joypad;
    std::string l_axis;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadInteger(l_joypad);
    argStream.ReadText(l_axis);
    if(!argStream.HasErrors() && !l_axis.empty())
    {
        int l_axisID = Utils::ReadEnumVector(g_axisNames, l_axis);
        if(l_axisID != -1)
        {
            float l_value = LuaManager::m_corePointer->GetSfmlManager()->GetJoypadAxisValue(l_joypad, static_cast<unsigned int>(l_axisID));
            lua_pushnumber(f_vm, l_value);
        }
        else lua_pushboolean(f_vm, 0);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}

}
}