#pragma once

namespace ROC 
{
namespace Lua 
{

int lightCreate(lua_State *f_vm);
int lightDestroy(lua_State *f_vm);
int lightSetParams(lua_State *f_vm);
int lightGetParams(lua_State *f_vm);
int lightSetColor(lua_State *f_vm);
int lightGetColor(lua_State *f_vm);
int lightSetDirection(lua_State *f_vm);
int lightGetDirection(lua_State *f_vm);

}
}
