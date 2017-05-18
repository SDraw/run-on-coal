#pragma once

namespace ROC
{
namespace Lua
{

int disabledFunction(lua_State *f_vm);
int logPrint(lua_State *f_vm);
int getTickCount(lua_State *f_vm);
int getTime(lua_State *f_vm);

}
}
