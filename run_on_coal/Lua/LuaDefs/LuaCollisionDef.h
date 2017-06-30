#pragma once

namespace ROC
{

class LuaCollisionDef final
{
    static int CollisionCreate(lua_State *f_vm);
    static int CollisionSetPosition(lua_State *f_vm);
    static int CollisionGetPosition(lua_State *f_vm);
    static int CollisionSetRotation(lua_State *f_vm);
    static int CollisionGetRotation(lua_State *f_vm);
    static int CollisionSetScale(lua_State *f_vm);
    static int CollisionGetScale(lua_State *f_vm);
    static int CollisionGetMass(lua_State *f_vm);
    static int CollisionSetVelocity(lua_State *f_vm);
    static int CollisionGetVelocity(lua_State *f_vm);
    static int CollisionSetAngularVelocity(lua_State *f_vm);
    static int CollisionGetAngularVelocity(lua_State *f_vm);
    static int CollisionSetFriction(lua_State *f_vm);
    static int CollisionGetFriction(lua_State *f_vm);
    static int CollisionSetLinearFactor(lua_State *f_vm);
    static int CollisionGetLinearFactor(lua_State *f_vm);
    static int CollisionSetAngularFactor(lua_State *f_vm);
    static int CollisionGetAngularFactor(lua_State *f_vm);
    static int CollisionApplyForce(lua_State *f_vm);
    static int CollisionApplyImpulse(lua_State *f_vm);
    static int CollisionApplyTorque(lua_State *f_vm);
    static int CollisionSetMotionType(lua_State *f_vm);
    static int CollisionGetMotionType(lua_State *f_vm);
    static int CollisionAttach(lua_State *f_vm);
    static int CollisionDetach(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaManager;

};

}
