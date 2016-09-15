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
}
}
