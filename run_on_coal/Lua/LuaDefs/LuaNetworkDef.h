#pragma once

namespace ROC
{

class LuaNetworkDef final
{
    static int NetworkConnect(lua_State *f_vm);
    static int NetworkDisconnect(lua_State *f_vm);
    static int NetworkSendData(lua_State *f_vm);
    static int NetworkGetState(lua_State *f_vm);
    static int NetworkGetPing(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaManager;
};

}
