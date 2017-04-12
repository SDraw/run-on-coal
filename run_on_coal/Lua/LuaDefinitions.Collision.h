#pragma once

namespace ROC
{
namespace Lua
{

int collisionCreate(lua_State *f_vm);
int collisionDestroy(lua_State *f_vm);
int collisionSetPosition(lua_State *f_vm);
int collisionGetPosition(lua_State *f_vm);
int collisionSetRotation(lua_State *f_vm);
int collisionGetRotation(lua_State *f_vm);
int collisionSetScale(lua_State *f_vm);
int collisionGetScale(lua_State *f_vm);
int collisionGetMass(lua_State *f_vm);
int collisionSetVelocity(lua_State *f_vm);
int collisionGetVelocity(lua_State *f_vm);
int collisionSetAngularVelocity(lua_State *f_vm);
int collisionGetAngularVelocity(lua_State *f_vm);
int collisionSetFriction(lua_State *f_vm);
int collisionGetFriction(lua_State *f_vm);
int collisionSetLinearFactor(lua_State *f_vm);
int collisionGetLinearFactor(lua_State *f_vm);
int collisionSetAngularFactor(lua_State *f_vm);
int collisionGetAngularFactor(lua_State *f_vm);
int collisionApplyForce(lua_State *f_vm);
int collisionApplyImpulse(lua_State *f_vm);
int collisionApplyTorque(lua_State *f_vm);
int collisionAttach(lua_State *f_vm);
int collisionDetach(lua_State *f_vm);

}
}
