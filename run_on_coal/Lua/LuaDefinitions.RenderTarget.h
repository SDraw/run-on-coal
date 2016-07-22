#pragma once

namespace ROC 
{
namespace Lua 
{

int rtCreate(lua_State *f_vm);
int rtDestroy(lua_State *f_vm);
int rtDraw(lua_State *f_vm);

}
}
