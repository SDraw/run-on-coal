#pragma once

namespace ROC
{

class LuaAnimationDef final
{
    static int AnimationCreate(lua_State *f_vm);
    static int AnimationGetBonesCount(lua_State *f_vm);
    static int AnimationGetDuration(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaManager;
};

}
