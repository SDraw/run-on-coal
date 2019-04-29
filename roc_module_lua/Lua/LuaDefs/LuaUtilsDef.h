#pragma once

class LuaUtilsDef final
{
    static int DisabledFunction(lua_State *f_vm);
    static int LogPrint(lua_State *f_vm);
    static int GetTick(lua_State *f_vm);
    static int GetTime(lua_State *f_vm);
    static int IsTask(lua_State *f_vm);
    static int Base64Encode(lua_State *f_vm);
    static int Base64Decode(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaVM;

};
