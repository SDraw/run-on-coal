#pragma once

namespace ROC
{
namespace Lua
{

int modelCreate(lua_State *f_vm);
int modelDestroy(lua_State *f_vm);
int modelGetGeometry(lua_State *f_vm);
int modelSetPosition(lua_State *f_vm);
int modelGetPosition(lua_State *f_vm);
int modelSetRotation(lua_State *f_vm);
int modelGetRotation(lua_State *f_vm);
int modelSetScale(lua_State *f_vm);
int modelGetScale(lua_State *f_vm);
int modelDraw(lua_State *f_vm);
int modelAttach(lua_State *f_vm);
int modelDetach(lua_State *f_vm);
int modelGetParent(lua_State *f_vm);
int modelSetAnimation(lua_State *f_vm);
int modelGetAnimation(lua_State *f_vm);
int modelPlayAnimation(lua_State *f_vm);
int modelPauseAnimation(lua_State *f_vm);
int modelResetAnimation(lua_State *f_vm);
int modelSetAnimationProperty(lua_State *f_vm);
int modelGetAnimationProperty(lua_State *f_vm);
int modelGetCollision(lua_State *f_vm);

}
}
