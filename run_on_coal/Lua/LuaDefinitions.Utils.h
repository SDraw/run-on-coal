#pragma once

namespace ROC
{
namespace Lua
{
int disabledFunction(lua_State *f_vm);
int logPrint(lua_State *f_vm);
int isElement(lua_State *f_vm);
int getElementType(lua_State *f_vm);
}
}
