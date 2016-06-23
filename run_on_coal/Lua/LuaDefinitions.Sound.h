#pragma once

namespace ROC 
{
namespace Lua 
{

int soundCreate(lua_State *f_vm);
int soundDestroy(lua_State *f_vm);
int soundPlay(lua_State *f_vm);
int soundPause(lua_State *f_vm);
int soundStop(lua_State *f_vm);
int soundIsLooped(lua_State *f_vm);
int soundGetState(lua_State *f_vm);
int soundSetSpeed(lua_State *f_vm);
int soundGetSpeed(lua_State *f_vm);
int soundSetVolume(lua_State *f_vm);
int soundGetVolume(lua_State *f_vm);
int soundSetTime(lua_State *f_vm);
int soundGetTime(lua_State *f_vm);
int soundSet3DPositionEnabled(lua_State *f_vm);
int soundGet3DPositionEnabled(lua_State *f_vm);
int soundSet3DPosition(lua_State *f_vm);
int soundGet3DPosition(lua_State *f_vm);
int soundSet3DDistance(lua_State *f_vm);
int soundGet3DDistance(lua_State *f_vm);
int soundSetListener3DOrientation(lua_State *f_vm);
int soundGetListener3DOrientation(lua_State *f_vm);

}
}
