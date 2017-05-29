#pragma once

namespace ROC
{
namespace Lua
{

int rtCreate(lua_State *f_vm);
int rtGetSize(lua_State *f_vm);
int rtGetFiltering(lua_State *f_vm);

}
}
