#pragma once

namespace ROC
{
namespace Lua
{
int collisionCreate(lua_State *f_vm);
int collisionDestroy(lua_State *f_vm);
int collisionSetPosition(lua_State *f_vm);
int collisionGetPosition(lua_State *f_vm);
int collisionSetRotation(lua_State *f_vm);
int collisionGetRotation(lua_State *f_vm);
}
}
