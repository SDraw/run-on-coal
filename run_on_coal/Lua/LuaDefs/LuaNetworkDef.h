#pragma once

namespace ROC
{

class LuaNetworkDef final
{
    static int Connect(lua_State *f_vm);
    static int Disconnect(lua_State *f_vm);
    static int SendData(lua_State *f_vm);
    static int GetState(lua_State *f_vm);
    static int GetPing(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaManager;
};

}
