#include "stdafx.h"

#include "Lua/LuaDefs/LuaInputDef.h"

#include "Interfaces/ICore.h"
#include "Interfaces/ISfmlManager.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaFunction.h"
#include "Module/LuaModule.h"
#include "Utils/EnumUtils.h"

const std::vector<std::string> g_KeyNames
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
const std::vector<std::string> g_MouseKeyNames
{
    "left", "right", "middle",
    "x1", "x2"
};
const std::vector<std::string> g_JoypadAxisNames
{
    "X", "Y", "Z",
    "R", "U", "V",
    "PovX", "PovY"
};

void LuaInputDef::Init(lua_State *f_vm)
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
    lua_register(f_vm, "setWindowInputEnabled", SetWindowInputEnabled);
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

int LuaInputDef::SetCursorMode(lua_State *f_vm)
{
    // bool setCursorMode(bool visible, bool locked)
    bool l_visible, l_locked;
    ArgReader argStream(f_vm);
    argStream.ReadBoolean(l_visible);
    argStream.ReadBoolean(l_locked);
    if(!argStream.HasErrors())
    {
        LuaModule::GetModule()->GetEngineCore()->GetISfmlManager()->SetCursorMode(l_visible, l_locked);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaInputDef::GetCursorPosition(lua_State *f_vm)
{
    // int int getCursorPosition()
    ArgReader argStream(f_vm);
    glm::ivec2 l_pos;
    LuaModule::GetModule()->GetEngineCore()->GetISfmlManager()->GetCursorPosition(l_pos);
    argStream.PushInteger(l_pos.x);
    argStream.PushInteger(l_pos.y);
    return argStream.GetReturnValue();
}
int LuaInputDef::SetCursorPosition(lua_State *f_vm)
{
    // bool setCursorPosition(int x, int y)
    glm::ivec2 l_pos;
    ArgReader argStream(f_vm);
    for(int i = 0; i < 2; i++) argStream.ReadInteger(l_pos[i]);
    if(!argStream.HasErrors())
    {
        LuaModule::GetModule()->GetEngineCore()->GetISfmlManager()->SetCursorPosition(l_pos);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaInputDef::SetWindowPosition(lua_State *f_vm)
{
    // bool setWindowPosition(int x, float y)
    glm::ivec2 l_pos;
    ArgReader argStream(f_vm);
    for(int i = 0; i < 2; i++) argStream.ReadInteger(l_pos[i]);
    if(!argStream.HasErrors())
    {
        LuaModule::GetModule()->GetEngineCore()->GetISfmlManager()->SetWindowPosition(l_pos);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaInputDef::GetWindowPosition(lua_State *f_vm)
{
    // int int getWindowPosition()
    ArgReader argStream(f_vm);
    glm::ivec2 l_pos;
    LuaModule::GetModule()->GetEngineCore()->GetISfmlManager()->GetWindowPosition(l_pos);
    argStream.PushInteger(l_pos.x);
    argStream.PushInteger(l_pos.y);
    return argStream.GetReturnValue();
}
int LuaInputDef::GetWindowSize(lua_State *f_vm)
{
    // int getWindowSize()
    ArgReader argStream(f_vm);
    glm::ivec2 l_size;
    LuaModule::GetModule()->GetEngineCore()->GetISfmlManager()->GetWindowSize(l_size);
    argStream.PushInteger(l_size.x);
    argStream.PushInteger(l_size.y);
    return argStream.GetReturnValue();
}
int LuaInputDef::CloseWindow(lua_State *f_vm)
{
    // bool closeWindow()
    ArgReader argStream(f_vm);
    LuaModule::GetModule()->GetEngineCore()->GetISfmlManager()->CloseWindow();
    argStream.PushBoolean(true);
    return argStream.GetReturnValue();
}

int LuaInputDef::IsKeyPressed(lua_State *f_vm)
{
    // bool isKeyPressed(str keyName)
    std::string l_key;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_key);
    if(!argStream.HasErrors() && !l_key.empty())
    {
        size_t l_numKey = EnumUtils::ReadEnumVector(l_key, g_KeyNames);
        if(l_numKey != std::numeric_limits<size_t>::max())
        {
            bool l_result = LuaModule::GetModule()->GetEngineCore()->GetISfmlManager()->IsKeyPressed(static_cast<int>(l_numKey));
            argStream.PushBoolean(l_result);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaInputDef::IsMouseKeyPressed(lua_State *f_vm)
{
    // bool isMouseKeyPressed(str keyName)
    std::string l_key;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_key);
    if(!argStream.HasErrors() && !l_key.empty())
    {
        size_t l_numKey = EnumUtils::ReadEnumVector(l_key, g_MouseKeyNames);
        if(l_numKey != std::numeric_limits<size_t>::max())
        {
            bool l_result = LuaModule::GetModule()->GetEngineCore()->GetISfmlManager()->IsMouseKeyPressed(static_cast<int>(l_numKey));
            argStream.PushBoolean(l_result);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaInputDef::SetWindowVSync(lua_State *f_vm)
{
    // bool setWindowVSync(bool sync)
    bool l_sync;
    ArgReader argStream(f_vm);
    argStream.ReadBoolean(l_sync);
    if(!argStream.HasErrors())
    {
        LuaModule::GetModule()->GetEngineCore()->GetISfmlManager()->SetVSync(l_sync);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaInputDef::SetWindowFramelimit(lua_State *f_vm)
{
    // bool setWindowFramelimit(int limit)
    unsigned int l_fps;
    ArgReader argStream(f_vm);
    argStream.ReadInteger(l_fps);
    if(!argStream.HasErrors())
    {
        LuaModule::GetModule()->GetEngineCore()->GetISfmlManager()->SetFramelimit(l_fps);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaInputDef::GetWindowFramelimit(lua_State *f_vm)
{
    // int getWindowFramelimit()
    ArgReader argStream(f_vm);
    argStream.PushInteger(LuaModule::GetModule()->GetEngineCore()->GetISfmlManager()->GetFramelimit());
    return argStream.GetReturnValue();
}
int LuaInputDef::SetWindowTitle(lua_State *f_vm)
{
    // bool setWindowTitle(str title)
    std::string l_title;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_title);
    if(!argStream.HasErrors() && !l_title.empty())
    {
        LuaModule::GetModule()->GetEngineCore()->GetISfmlManager()->SetTitle(l_title);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaInputDef::SetWindowIcon(lua_State *f_vm)
{
    // bool setWindowIcon(str path)
    std::string l_path;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_path);
    if(!argStream.HasErrors() && !l_path.empty())
    {
        bool l_result = LuaModule::GetModule()->GetEngineCore()->GetISfmlManager()->SetIcon(l_path);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaInputDef::RequestWindowFocus(lua_State *f_vm)
{
    // bool requestWindowFocus()
    ArgReader argStream(f_vm);
    LuaModule::GetModule()->GetEngineCore()->GetISfmlManager()->RequestFocus();
    argStream.PushBoolean(true);
    return argStream.GetReturnValue();
}
int LuaInputDef::GetWindowFocus(lua_State *f_vm)
{
    // bool getWindowFocus()
    ArgReader argStream(f_vm);
    argStream.PushBoolean(LuaModule::GetModule()->GetEngineCore()->GetISfmlManager()->GetFocusState());
    return argStream.GetReturnValue();
}
int LuaInputDef::SetWindowInputEnabled(lua_State *f_vm)
{
    // bool setWindowInputEnabled(bool state)
    bool l_state;
    ArgReader argStream(f_vm);
    argStream.ReadBoolean(l_state);
    if(!argStream.HasErrors())
    {
        LuaModule::GetModule()->GetEngineCore()->GetISfmlManager()->SetInputEnabled(l_state);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaInputDef::IsJoypadConnected(lua_State *f_vm)
{
    // bool isJoypadConnected(int index)
    unsigned int l_joypad;
    ArgReader argStream(f_vm);
    argStream.ReadInteger(l_joypad);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaModule::GetModule()->GetEngineCore()->GetISfmlManager()->IsJoypadConnected(l_joypad);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaInputDef::JoypadGetButtonCount(lua_State *f_vm)
{
    // int joypadGetButtonCount(int index)
    unsigned int l_joypad;
    ArgReader argStream(f_vm);
    argStream.ReadInteger(l_joypad);
    if(!argStream.HasErrors())
    {
        unsigned int l_count = LuaModule::GetModule()->GetEngineCore()->GetISfmlManager()->GetJoypadButtonCount(l_joypad);
        argStream.PushInteger(l_count);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaInputDef::JoypadGetButtonState(lua_State *f_vm)
{
    // bool joypadGetButtonState(int index, int buttonIndex)
    unsigned int l_joypad, l_button;
    ArgReader argStream(f_vm);
    argStream.ReadInteger(l_joypad);
    argStream.ReadInteger(l_button);
    if(!argStream.HasErrors())
    {
        bool l_state = LuaModule::GetModule()->GetEngineCore()->GetISfmlManager()->GetJoypadButtonState(l_joypad, l_button);
        argStream.PushBoolean(l_state);
    }
    else argStream.PushBoolean(false);
    return 1;
}
int LuaInputDef::JoypadHasAxis(lua_State *f_vm)
{
    // bool joypadHasAxis(int index, int axisIndex)
    unsigned int l_joypad;
    std::string l_axis;
    ArgReader argStream(f_vm);
    argStream.ReadInteger(l_joypad);
    argStream.ReadText(l_axis);
    if(!argStream.HasErrors() && !l_axis.empty())
    {
        size_t l_axisID = EnumUtils::ReadEnumVector(l_axis, g_JoypadAxisNames);
        if(l_axisID != std::numeric_limits<size_t>::max())
        {
            bool l_result = LuaModule::GetModule()->GetEngineCore()->GetISfmlManager()->CheckJoypadAxis(l_joypad, static_cast<unsigned int>(l_axisID));
            argStream.PushBoolean(l_result);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaInputDef::JoypadGetAxisValue(lua_State *f_vm)
{
    // float joypadGetAxisValue(int index, int axisIndex)
    unsigned int l_joypad;
    std::string l_axis;
    ArgReader argStream(f_vm);
    argStream.ReadInteger(l_joypad);
    argStream.ReadText(l_axis);
    if(!argStream.HasErrors() && !l_axis.empty())
    {
        size_t l_axisID = EnumUtils::ReadEnumVector(l_axis, g_JoypadAxisNames);
        if(l_axisID != std::numeric_limits<size_t>::max())
        {
            float l_val = LuaModule::GetModule()->GetEngineCore()->GetISfmlManager()->GetJoypadAxisValue(l_joypad, static_cast<unsigned int>(l_axisID));
            argStream.PushNumber(l_val);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaInputDef::GetClipboard(lua_State *f_vm)
{
    ArgReader argStream(f_vm);
    std::string l_clipboard;
    LuaModule::GetModule()->GetEngineCore()->GetISfmlManager()->GetClipboardString(l_clipboard);
    argStream.PushText(l_clipboard);
    return argStream.GetReturnValue();
}
int LuaInputDef::SetClipboard(lua_State *f_vm)
{
    ArgReader argStream(f_vm);
    std::string l_clipboard;
    argStream.ReadText(l_clipboard);
    if(!argStream.HasErrors())
    {
        LuaModule::GetModule()->GetEngineCore()->GetISfmlManager()->SetClipboardString(l_clipboard);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
