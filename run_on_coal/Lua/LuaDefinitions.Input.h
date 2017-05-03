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
int setWindowVSync(lua_State *f_vm);
int setWindowFramelimit(lua_State *f_vm);
int getWindowFramelimit(lua_State *f_vm);
int setWindowTitle(lua_State *f_vm);
int setWindowIcon(lua_State *f_vm);
int requestWindowFocus(lua_State *f_vm);
int getWindowFocus(lua_State *f_vm);
int closeWindow(lua_State *f_vm);
int isKeyPressed(lua_State *f_vm);
int isMouseKeyPressed(lua_State *f_vm);
int isJoypadConnected(lua_State *f_vm);
int joypadGetButtonCount(lua_State *f_vm);
int joypadGetButtonState(lua_State *f_vm);
int joypadHasAxis(lua_State *f_vm);
int joypadGetAxisValue(lua_State *f_vm);

}
}