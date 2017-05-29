#pragma once

namespace ROC
{
namespace Lua
{

int textureCreate(lua_State *f_vm);
int textureGetSize(lua_State *f_vm);
int textureGetFiltering(lua_State *f_vm);

}
}
