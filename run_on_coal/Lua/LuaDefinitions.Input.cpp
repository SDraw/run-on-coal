#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/RenderManager/RenderManager.h"
#include "Managers/SfmlManager.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Input.h"
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
    ArgReader argStream(f_vm);
    argStream.ReadText(l_state);
    if(!argStream.HasErrors() && !l_state.empty())
    {
        int l_type = Utils::Enum::ReadEnumVector(g_cursorLocksTable, l_state);
        if(l_type != -1)
        {
            LuaManager::GetCore()->GetSfmlManager()->SetCursorMode(static_cast<unsigned char>(l_type));
            argStream.PushBoolean(true);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int getCursorPosition(lua_State *f_vm)
{
    ArgReader argStream(f_vm);
    glm::ivec2 l_pos;
    LuaManager::GetCore()->GetSfmlManager()->GetCursorPosition(l_pos);
    argStream.PushInteger(l_pos.x);
    argStream.PushInteger(l_pos.y);
    return argStream.GetReturnValue();
}
int setCursorPosition(lua_State *f_vm)
{
    glm::ivec2 l_pos;
    ArgReader argStream(f_vm);
    for(int i = 0; i < 2; i++) argStream.ReadInteger(l_pos[i]);
    if(!argStream.HasErrors())
    {
        LuaManager::GetCore()->GetSfmlManager()->SetCursorPosition(l_pos);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int getWindowPosition(lua_State *f_vm)
{
    ArgReader argStream(f_vm);
    glm::ivec2 l_pos;
    LuaManager::GetCore()->GetSfmlManager()->GetWindowPosition(l_pos);
    argStream.PushInteger(l_pos.x);
    argStream.PushInteger(l_pos.y);
    return argStream.GetReturnValue();
}
int getWindowSize(lua_State *f_vm)
{
    ArgReader argStream(f_vm);
    glm::ivec2 l_size;
    LuaManager::GetCore()->GetSfmlManager()->GetWindowSize(l_size);
    argStream.PushInteger(l_size.x);
    argStream.PushInteger(l_size.y);
    return argStream.GetReturnValue();
}
int closeWindow(lua_State *f_vm)
{
    ArgReader argStream(f_vm);
    LuaManager::GetCore()->GetSfmlManager()->CloseWindow();
    argStream.PushBoolean(true);
    return argStream.GetReturnValue();
}

int isKeyPressed(lua_State *f_vm)
{
    std::string l_key;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_key);
    if(!argStream.HasErrors() && !l_key.empty())
    {
        int l_numKey = Utils::Enum::ReadEnumVector(g_keysTable, l_key);
        if(l_numKey != -1)
        {
            bool l_result = SfmlManager::IsKeyPressed(l_numKey);
            argStream.PushBoolean(l_result);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int isMouseKeyPressed(lua_State *f_vm)
{
    std::string l_key;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_key);
    if(!argStream.HasErrors() && !l_key.empty())
    {
        int l_numKey = Utils::Enum::ReadEnumVector(g_mouseKeysTable, l_key);
        if(l_numKey != -1)
        {
            bool l_result = SfmlManager::IsMouseKeyPressed(l_numKey);
            argStream.PushBoolean(l_result);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int setWindowVSync(lua_State *f_vm)
{
    bool l_sync;
    ArgReader argStream(f_vm);
    argStream.ReadBoolean(l_sync);
    if(!argStream.HasErrors())
    {
        LuaManager::GetCore()->GetSfmlManager()->SetVSync(l_sync);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int setWindowFramelimit(lua_State *f_vm)
{
    unsigned int l_fps;
    ArgReader argStream(f_vm);
    argStream.ReadInteger(l_fps);
    if(!argStream.HasErrors())
    {
        LuaManager::GetCore()->GetSfmlManager()->SetFramelimit(l_fps);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int getWindowFramelimit(lua_State *f_vm)
{
    ArgReader argStream(f_vm);
    argStream.PushInteger(LuaManager::GetCore()->GetSfmlManager()->GetFramelimit());
    return argStream.GetReturnValue();
}
int setWindowTitle(lua_State *f_vm)
{
    std::string l_title;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_title);
    if(!argStream.HasErrors())
    {
        sf::String l_title32 = sf::String::fromUtf8(l_title.begin(), l_title.end());
        LuaManager::GetCore()->GetSfmlManager()->SetTitle(l_title32);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int setWindowIcon(lua_State *f_vm)
{
    std::string l_path;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_path);
    if(!argStream.HasErrors() && !l_path.empty())
    {
        bool l_result = LuaManager::GetCore()->GetSfmlManager()->SetIcon(l_path);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int requestWindowFocus(lua_State *f_vm)
{
    ArgReader argStream(f_vm);
    LuaManager::GetCore()->GetSfmlManager()->RequestFocus();
    argStream.PushBoolean(true);
    return argStream.GetReturnValue();
}
int getWindowFocus(lua_State *f_vm)
{
    ArgReader argStream(f_vm);
    argStream.PushBoolean(LuaManager::GetCore()->GetSfmlManager()->GetFocusState());
    return argStream.GetReturnValue();
}

int isJoypadConnected(lua_State *f_vm)
{
    unsigned int l_joypad;
    ArgReader argStream(f_vm);
    argStream.ReadInteger(l_joypad);
    if(!argStream.HasErrors())
    {
        bool l_result = SfmlManager::IsJoypadConnected(l_joypad);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int joypadGetButtonCount(lua_State *f_vm)
{
    unsigned int l_joypad;
    ArgReader argStream(f_vm);
    argStream.ReadInteger(l_joypad);
    if(!argStream.HasErrors())
    {
        unsigned int l_count = SfmlManager::GetJoypadButtonCount(l_joypad);
        argStream.PushInteger(l_count);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int joypadGetButtonState(lua_State *f_vm)
{
    unsigned int l_joypad, l_button;
    ArgReader argStream(f_vm);
    argStream.ReadInteger(l_joypad);
    argStream.ReadInteger(l_button);
    if(!argStream.HasErrors())
    {
        bool l_state = SfmlManager::GetJoypadButtonState(l_joypad, l_button);
        argStream.PushBoolean(l_state);
    }
    else argStream.PushBoolean(false);
    return 1;
}
int joypadHasAxis(lua_State *f_vm)
{
    unsigned int l_joypad;
    std::string l_axis;
    ArgReader argStream(f_vm);
    argStream.ReadInteger(l_joypad);
    argStream.ReadText(l_axis);
    if(!argStream.HasErrors() && !l_axis.empty())
    {
        int l_axisID = Utils::Enum::ReadEnumVector(g_axisNames, l_axis);
        if(l_axisID != -1)
        {
            bool l_result = SfmlManager::CheckJoypadAxis(l_joypad, static_cast<unsigned int>(l_axisID));
            argStream.PushBoolean(l_result);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int joypadGetAxisValue(lua_State *f_vm)
{
    unsigned int l_joypad;
    std::string l_axis;
    ArgReader argStream(f_vm);
    argStream.ReadInteger(l_joypad);
    argStream.ReadText(l_axis);
    if(!argStream.HasErrors() && !l_axis.empty())
    {
        int l_axisID = Utils::Enum::ReadEnumVector(g_axisNames, l_axis);
        if(l_axisID != -1)
        {
            float l_val = SfmlManager::GetJoypadAxisValue(l_joypad, static_cast<unsigned int>(l_axisID));
            argStream.PushNumber(l_val);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

}
}
