#pragma once

class LuaClientDef final
{
    static int Disconnect(lua_State *f_vm);
    static int SendData(lua_State *f_vm);
    static int GetID(lua_State *f_vm);
    static int GetAddress(lua_State *f_vm);
    static int GetPing(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaVM;
};
