#pragma once

class LuaCollisionDef final
{
    static int Create(lua_State *f_vm);
    static int SetPosition(lua_State *f_vm);
    static int GetPosition(lua_State *f_vm);
    static int SetRotation(lua_State *f_vm);
    static int GetRotation(lua_State *f_vm);
    static int SetScale(lua_State *f_vm);
    static int GetScale(lua_State *f_vm);
    static int GetMass(lua_State *f_vm);
    static int SetVelocity(lua_State *f_vm);
    static int GetVelocity(lua_State *f_vm);
    static int SetAngularVelocity(lua_State *f_vm);
    static int GetAngularVelocity(lua_State *f_vm);
    static int SetFriction(lua_State *f_vm);
    static int GetFriction(lua_State *f_vm);
    static int SetLinearFactor(lua_State *f_vm);
    static int GetLinearFactor(lua_State *f_vm);
    static int SetAngularFactor(lua_State *f_vm);
    static int GetAngularFactor(lua_State *f_vm);
    static int ApplyForce(lua_State *f_vm);
    static int ApplyImpulse(lua_State *f_vm);
    static int ApplyTorque(lua_State *f_vm);
    static int SetMotionType(lua_State *f_vm);
    static int GetMotionType(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaVM;

};
