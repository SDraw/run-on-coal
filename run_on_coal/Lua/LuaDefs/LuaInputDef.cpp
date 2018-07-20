#include "stdafx.h"

#include "Lua/LuaDefs/LuaInputDef.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/RenderManager/RenderManager.h"
#include "Managers/SfmlManager.h"
#include "Lua/ArgReader.h"
#include "Utils/EnumUtils.h"

namespace ROC
{

extern const std::vector<std::string> g_KeyNamesTable;
extern const std::vector<std::string> g_MouseKeyNamesTable;
extern const std::vector<std::string> g_JoypadAxisNamesTable;

}

void ROC::LuaInputDef::Init(lua_State *f_vm)
{
    lua_register(f_vm, "setCursorMode", SetCursorMode);
    lua_register(f_vm, "setCursorPosition", SetCursorPosition);
    lua_register(f_vm, "getCursorPosition", GetCursorPosition);
    lua_register(f_vm, "setWindowPosition", SetWindowPosition);
    lua_register(f_vm, "getWindowPosition", GetWindowPosition);
    lua_register(f_vm, "getWindowSize", GetWindowSize);
    lua_register(f_vm, "setWindowVSync", SetWindowVSync);
    lua_register(f_vm, "setWindowFramelimit", SetWindowFramelimit);
    lua_register(f_vm, "getWindowFramelimit", GetWindowFramelimit);
    lua_register(f_vm, "setWindowTitle", SetWindowTitle);
    lua_register(f_vm, "setWindowIcon", SetWindowIcon);
    lua_register(f_vm, "requestWindowFocus", RequestWindowFocus);
    lua_register(f_vm, "getWindowFocus", GetWindowFocus);
    lua_register(f_vm, "closeWindow", CloseWindow);
    lua_register(f_vm, "isKeyPressed", IsKeyPressed);
    lua_register(f_vm, "isMouseKeyPressed", IsMouseKeyPressed);
    lua_register(f_vm, "isJoypadConnected", IsJoypadConnected);
    lua_register(f_vm, "joypadGetButtonCount", JoypadGetButtonCount);
    lua_register(f_vm, "joypadGetButtonState", JoypadGetButtonState);
    lua_register(f_vm, "joypadHasAxis", JoypadHasAxis);
    lua_register(f_vm, "joypadGetAxisValue", JoypadGetAxisValue);
    lua_register(f_vm, "getClipboard", GetClipboard);
    lua_register(f_vm, "setClipboard", SetClipboard);
}

int ROC::LuaInputDef::SetCursorMode(lua_State *f_vm)
{
    // bool setCursorMode(bool visible, bool locked)
    bool l_visible, l_locked;
    ArgReader argStream(f_vm);
    argStream.ReadBoolean(l_visible);
    argStream.ReadBoolean(l_locked);
    if(!argStream.HasErrors())
    {
        LuaManager::GetCore()->GetSfmlManager()->SetCursorMode(l_visible, l_locked);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaInputDef::GetCursorPosition(lua_State *f_vm)
{
    // int int getCursorPosition()
    ArgReader argStream(f_vm);
    glm::ivec2 l_pos;
    LuaManager::GetCore()->GetSfmlManager()->GetCursorPosition(l_pos);
    argStream.PushInteger(l_pos.x);
    argStream.PushInteger(l_pos.y);
    return argStream.GetReturnValue();
}
int ROC::LuaInputDef::SetCursorPosition(lua_State *f_vm)
{
    // bool setCursorPosition(int x, int y)
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

int ROC::LuaInputDef::SetWindowPosition(lua_State *f_vm)
{
    // bool setWindowPosition(int x, float y)
    glm::ivec2 l_pos;
    ArgReader argStream(f_vm);
    for(int i = 0; i < 2; i++) argStream.ReadInteger(l_pos[i]);
    if(!argStream.HasErrors())
    {
        LuaManager::GetCore()->GetSfmlManager()->SetWindowPosition(l_pos);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaInputDef::GetWindowPosition(lua_State *f_vm)
{
    // int int getWindowPosition()
    ArgReader argStream(f_vm);
    glm::ivec2 l_pos;
    LuaManager::GetCore()->GetSfmlManager()->GetWindowPosition(l_pos);
    argStream.PushInteger(l_pos.x);
    argStream.PushInteger(l_pos.y);
    return argStream.GetReturnValue();
}
int ROC::LuaInputDef::GetWindowSize(lua_State *f_vm)
{
    // int getWindowSize()
    ArgReader argStream(f_vm);
    glm::ivec2 l_size;
    LuaManager::GetCore()->GetSfmlManager()->GetWindowSize(l_size);
    argStream.PushInteger(l_size.x);
    argStream.PushInteger(l_size.y);
    return argStream.GetReturnValue();
}
int ROC::LuaInputDef::CloseWindow(lua_State *f_vm)
{
    // bool closeWindow()
    ArgReader argStream(f_vm);
    LuaManager::GetCore()->GetSfmlManager()->CloseWindow();
    argStream.PushBoolean(true);
    return argStream.GetReturnValue();
}

int ROC::LuaInputDef::IsKeyPressed(lua_State *f_vm)
{
    // bool isKeyPressed(str keyName)
    std::string l_key;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_key);
    if(!argStream.HasErrors() && !l_key.empty())
    {
        int l_numKey = EnumUtils::ReadEnumVector(l_key, g_KeyNamesTable);
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
int ROC::LuaInputDef::IsMouseKeyPressed(lua_State *f_vm)
{
    // bool isMouseKeyPressed(str keyName)
    std::string l_key;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_key);
    if(!argStream.HasErrors() && !l_key.empty())
    {
        int l_numKey = EnumUtils::ReadEnumVector(l_key, g_MouseKeyNamesTable);
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

int ROC::LuaInputDef::SetWindowVSync(lua_State *f_vm)
{
    // bool setWindowVSync(bool sync)
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
int ROC::LuaInputDef::SetWindowFramelimit(lua_State *f_vm)
{
    // bool setWindowFramelimit(int limit)
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
int ROC::LuaInputDef::GetWindowFramelimit(lua_State *f_vm)
{
    // int getWindowFramelimit()
    ArgReader argStream(f_vm);
    argStream.PushInteger(LuaManager::GetCore()->GetSfmlManager()->GetFramelimit());
    return argStream.GetReturnValue();
}
int ROC::LuaInputDef::SetWindowTitle(lua_State *f_vm)
{
    // bool setWindowTitle(str title)
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
int ROC::LuaInputDef::SetWindowIcon(lua_State *f_vm)
{
    // bool setWindowIcon(str path)
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
int ROC::LuaInputDef::RequestWindowFocus(lua_State *f_vm)
{
    // bool requestWindowFocus()
    ArgReader argStream(f_vm);
    LuaManager::GetCore()->GetSfmlManager()->RequestFocus();
    argStream.PushBoolean(true);
    return argStream.GetReturnValue();
}
int ROC::LuaInputDef::GetWindowFocus(lua_State *f_vm)
{
    // bool getWindowFocus()
    ArgReader argStream(f_vm);
    argStream.PushBoolean(LuaManager::GetCore()->GetSfmlManager()->GetFocusState());
    return argStream.GetReturnValue();
}

int ROC::LuaInputDef::IsJoypadConnected(lua_State *f_vm)
{
    // bool isJoypadConnected(int index)
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
int ROC::LuaInputDef::JoypadGetButtonCount(lua_State *f_vm)
{
    // int joypadGetButtonCount(int index)
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
int ROC::LuaInputDef::JoypadGetButtonState(lua_State *f_vm)
{
    // bool joypadGetButtonState(int index, int buttonIndex)
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
int ROC::LuaInputDef::JoypadHasAxis(lua_State *f_vm)
{
    // bool joypadHasAxis(int index, int axisIndex)
    unsigned int l_joypad;
    std::string l_axis;
    ArgReader argStream(f_vm);
    argStream.ReadInteger(l_joypad);
    argStream.ReadText(l_axis);
    if(!argStream.HasErrors() && !l_axis.empty())
    {
        int l_axisID = EnumUtils::ReadEnumVector(l_axis, g_JoypadAxisNamesTable);
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
int ROC::LuaInputDef::JoypadGetAxisValue(lua_State *f_vm)
{
    // float joypadGetAxisValue(int index, int axisIndex)
    unsigned int l_joypad;
    std::string l_axis;
    ArgReader argStream(f_vm);
    argStream.ReadInteger(l_joypad);
    argStream.ReadText(l_axis);
    if(!argStream.HasErrors() && !l_axis.empty())
    {
        int l_axisID = EnumUtils::ReadEnumVector(l_axis, g_JoypadAxisNamesTable);
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

int ROC::LuaInputDef::GetClipboard(lua_State *f_vm)
{
    ArgReader argStream(f_vm);
    std::string l_clipboard;
    LuaManager::GetCore()->GetSfmlManager()->GetClipboardString(l_clipboard);
    argStream.PushText(l_clipboard);
    return argStream.GetReturnValue();
}
int ROC::LuaInputDef::SetClipboard(lua_State *f_vm)
{
    ArgReader argStream(f_vm);
    std::string l_clipboard;
    argStream.ReadText(l_clipboard);
    if(!argStream.HasErrors())
    {
        LuaManager::GetCore()->GetSfmlManager()->SetClipboardString(l_clipboard);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
