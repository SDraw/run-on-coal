#pragma once

namespace ROC 
{
namespace Lua 
{

int physicsSetEnabled(lua_State *f_vm);
int physicsGetEnabled(lua_State *f_vm);
int physicsSetFloorEnabled(lua_State *f_vm);
int physicsGetFloorEnabled(lua_State *f_vm);
int physicsSetGravity(lua_State *f_vm);
int physicsGetGravity(lua_State *f_vm);
int physicsRayCast(lua_State *f_vm);

}
}
