#pragma once

class LuaEventsDef final
{
    static int Add(lua_State *f_vm);
    static int AddHandler(lua_State *f_vm);
    static int Remove(lua_State *f_vm);
    static int RemoveHandler(lua_State *f_vm);
    static int Call(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaVM;

};
