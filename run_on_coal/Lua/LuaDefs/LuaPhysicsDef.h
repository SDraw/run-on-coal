#pragma once

namespace ROC
{

class LuaPhysicsDef
{
    static int PhysicsSetEnabled(lua_State *f_vm);
    static int PhysicsGetEnabled(lua_State *f_vm);
    static int PhysicsSetFloorEnabled(lua_State *f_vm);
    static int PhysicsGetFloorEnabled(lua_State *f_vm);
    static int PhysicsSetGravity(lua_State *f_vm);
    static int PhysicsGetGravity(lua_State *f_vm);
    static int PhysicsRayCast(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaManager;

};

}
