#pragma once

namespace ROC
{
namespace Lua
{
int setCursorMode(lua_State *f_vm);
int getCursorPosition(lua_State *f_vm);
int setCursorPosition(lua_State *f_vm);
int getWindowPosition(lua_State *f_vm);
int getWindowSize(lua_State *f_vm);
int closeApp(lua_State *f_vm);
int getTime(lua_State *f_vm);
int isKeyPressed(lua_State *f_vm);
int isMouseKeyPressed(lua_State *f_vm);
int setWindowVSync(lua_State *f_vm);
int setWindowFramelimit(lua_State *f_vm);
int setWindowTitle(lua_State *f_vm);
int setWindowIcon(lua_State *f_vm);
int requestWindowFocus(lua_State *f_vm);
int getWindowFocus(lua_State *f_vm);
}
}
