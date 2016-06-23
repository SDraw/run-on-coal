#pragma once

namespace ROC 
{
namespace Lua 
{

int cameraCreate(lua_State *f_vm);
int cameraDestroy(lua_State *f_vm);
int cameraSetPosition(lua_State *f_vm);
int cameraGetPosition(lua_State *f_vm);
int cameraSetDirection(lua_State *f_vm);
int cameraGetDirection(lua_State *f_vm);
int cameraSetPerspective(lua_State *f_vm);
int cameraSetOrtho(lua_State *f_vm);
int cameraGetViewMatrix(lua_State *f_vm);
int cameraGetProjectionMatrix(lua_State *f_vm);

}
}
