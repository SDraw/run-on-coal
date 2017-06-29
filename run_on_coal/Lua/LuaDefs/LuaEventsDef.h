#pragma once

namespace ROC
{

class LuaEventsDef
{
    static int AddEvent(lua_State *f_vm);
    static int AddEventHandler(lua_State *f_vm);
    static int RemoveEvent(lua_State *f_vm);
    static int RemoveEventHandler(lua_State *f_vm);
    static int CallEvent(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaManager;

};

}
