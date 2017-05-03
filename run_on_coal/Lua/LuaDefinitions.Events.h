#pragma once

namespace ROC
{
namespace Lua
{

int addEvent(lua_State *f_vm);
int addEventHandler(lua_State *f_vm);
int removeEvent(lua_State *f_vm);
int removeEventHandler(lua_State *f_vm);
int callEvent(lua_State *f_vm);

}
}
