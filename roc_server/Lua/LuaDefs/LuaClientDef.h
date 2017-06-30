#pragma once

namespace ROC
{

class LuaClientDef final
{
    static int ClientCreateDummy(lua_State *f_vm);
    static int ClientDisconnect(lua_State *f_vm);
    static int ClientSendData(lua_State *f_vm);
    static int ClientGetID(lua_State *f_vm);
    static int ClientGetAddress(lua_State *f_vm);
    static int ClientGetPing(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaManager;
};

}
