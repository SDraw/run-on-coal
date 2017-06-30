#pragma once

namespace ROC
{

class LuaModelDef final
{
    static int ModelCreate(lua_State *f_vm);
    static int ModelGetGeometry(lua_State *f_vm);
    static int ModelSetPosition(lua_State *f_vm);
    static int ModelGetPosition(lua_State *f_vm);
    static int ModelSetRotation(lua_State *f_vm);
    static int ModelGetRotation(lua_State *f_vm);
    static int ModelSetScale(lua_State *f_vm);
    static int ModelGetScale(lua_State *f_vm);
    static int ModelDraw(lua_State *f_vm);
    static int ModelAttach(lua_State *f_vm);
    static int ModelDetach(lua_State *f_vm);
    static int ModelGetParent(lua_State *f_vm);
    static int ModelSetAnimation(lua_State *f_vm);
    static int ModelGetAnimation(lua_State *f_vm);
    static int ModelRemoveAnimation(lua_State *f_vm);
    static int ModelPlayAnimation(lua_State *f_vm);
    static int ModelPauseAnimation(lua_State *f_vm);
    static int ModelResetAnimation(lua_State *f_vm);
    static int ModelSetAnimationProperty(lua_State *f_vm);
    static int ModelGetAnimationProperty(lua_State *f_vm);
    static int ModelGetCollision(lua_State *f_vm);
    static int ModelSetCollidable(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaManager;
};

}
