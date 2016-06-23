#pragma once

namespace ROC 
{
namespace Lua 
{

int fontCreate(lua_State *f_vm);
int fontDestroy(lua_State *f_vm);
int fontDraw(lua_State *f_vm);

}
}
