#pragma once

namespace ROC
{
namespace Lua
{

int textureCreate(lua_State *f_vm);
int textureDestroy(lua_State *f_vm);
int textureDraw(lua_State *f_vm);

}
}
