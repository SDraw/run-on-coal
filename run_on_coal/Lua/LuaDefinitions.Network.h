#pragma once

namespace ROC
{
namespace Lua
{

int networkConnect(lua_State *f_vm);
int networkDisconnect(lua_State *f_vm);
int networkSendData(lua_State *f_vm);
int networkGetState(lua_State *f_vm);
int networkGetPing(lua_State *f_vm);

}
}