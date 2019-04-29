#pragma once

class LuaInputDef final
{
    static int SetCursorMode(lua_State *f_vm);
    static int GetCursorPosition(lua_State *f_vm);
    static int SetCursorPosition(lua_State *f_vm);
    static int SetWindowPosition(lua_State *f_vm);
    static int GetWindowPosition(lua_State *f_vm);
    static int GetWindowSize(lua_State *f_vm);
    static int SetWindowVSync(lua_State *f_vm);
    static int SetWindowFramelimit(lua_State *f_vm);
    static int GetWindowFramelimit(lua_State *f_vm);
    static int SetWindowTitle(lua_State *f_vm);
    static int SetWindowIcon(lua_State *f_vm);
    static int RequestWindowFocus(lua_State *f_vm);
    static int GetWindowFocus(lua_State *f_vm);
    static int SetWindowInputEnabled(lua_State *f_vm);
    static int CloseWindow(lua_State *f_vm);
    static int IsKeyPressed(lua_State *f_vm);
    static int IsMouseKeyPressed(lua_State *f_vm);
    static int IsJoypadConnected(lua_State *f_vm);
    static int JoypadGetButtonCount(lua_State *f_vm);
    static int JoypadGetButtonState(lua_State *f_vm);
    static int JoypadHasAxis(lua_State *f_vm);
    static int JoypadGetAxisValue(lua_State *f_vm);
    static int GetClipboard(lua_State *f_vm);
    static int SetClipboard(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaVM;
};
