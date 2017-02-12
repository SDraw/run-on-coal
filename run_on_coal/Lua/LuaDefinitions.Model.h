#pragma once

namespace ROC
{
namespace Lua
{

int modelCreate(lua_State *f_vm);
int modelDestroy(lua_State *f_vm);
int modelGetGeometry(lua_State *f_vm);
int modelGetType(lua_State *f_vm);
int modelSetPosition(lua_State *f_vm);
int modelGetPosition(lua_State *f_vm);
int modelSetRotation(lua_State *f_vm);
int modelGetRotation(lua_State *f_vm);
int modelSetScale(lua_State *f_vm);
int modelGetScale(lua_State *f_vm);
int modelDraw(lua_State *f_vm);
int modelAttach(lua_State *f_vm);
int modelDettach(lua_State *f_vm);
int modelGetParent(lua_State *f_vm);
int modelSetAnimation(lua_State *f_vm);
int modelGetAnimation(lua_State *f_vm);
int modelPlayAnimation(lua_State *f_vm);
int modelPauseAnimation(lua_State *f_vm);
int modelResetAnimation(lua_State *f_vm);
int modelSetAnimationProperty(lua_State *f_vm);
int modelGetAnimationProperty(lua_State *f_vm);
int modelSetCollision(lua_State *f_vm);
int modelRemoveCollision(lua_State *f_vm);
int modelSetCollisionProperty(lua_State *f_vm);
int modelGetCollisionProperty(lua_State *f_vm);

}
}
