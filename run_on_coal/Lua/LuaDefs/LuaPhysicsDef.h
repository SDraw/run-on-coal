#pragma once

namespace ROC
{

class LuaPhysicsDef final
{
    static int SetEnabled(lua_State *f_vm);
    static int GetEnabled(lua_State *f_vm);
    static int SetFloorEnabled(lua_State *f_vm);
    static int GetFloorEnabled(lua_State *f_vm);
    static int SetGravity(lua_State *f_vm);
    static int GetGravity(lua_State *f_vm);
    static int RayCast(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaManager;

};

}
