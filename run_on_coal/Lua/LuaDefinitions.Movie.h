#pragma once

namespace ROC
{
namespace Lua
{

int movieCreate(lua_State *f_vm);
int moviePlay(lua_State *f_vm);
int moviePause(lua_State *f_vm);
int movieStop(lua_State *f_vm);

}
}
