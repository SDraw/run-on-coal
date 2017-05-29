#pragma once

namespace ROC
{
namespace Lua
{

int movieCreate(lua_State *f_vm);
int moviePlay(lua_State *f_vm);
int moviePause(lua_State *f_vm);
int movieStop(lua_State *f_vm);
int movieGetSampleRate(lua_State *f_vm);
int movieGetChannelCount(lua_State *f_vm);
int movieGetFramerate(lua_State *f_vm);
int movieGetDuration(lua_State *f_vm);
int movieGetVolume(lua_State *f_vm);
int movieSetVolume(lua_State *f_vm);
int movieGetTime(lua_State *f_vm);
int movieSetTime(lua_State *f_vm);

}
}
