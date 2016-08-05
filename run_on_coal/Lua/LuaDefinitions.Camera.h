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

int cameraSetType(lua_State *f_vm);
int cameraGetType(lua_State *f_vm);

int cameraSetFOV(lua_State *f_vm);
int cameraGetFOV(lua_State *f_vm);

int cameraSetPerspectiveSize(lua_State *f_vm);
int cameraGetPerspectiveSize(lua_State *f_vm);

int cameraSetOrthoSize(lua_State *f_vm);
int cameraGetOrthoSize(lua_State *f_vm);

int cameraSetDepth(lua_State *f_vm);
int cameraGetDepth(lua_State *f_vm);

int cameraGetViewMatrix(lua_State *f_vm);
int cameraGetProjectionMatrix(lua_State *f_vm);

}
}
