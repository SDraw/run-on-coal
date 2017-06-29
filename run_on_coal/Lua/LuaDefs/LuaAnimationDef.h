#pragma once

namespace ROC
{

class LuaAnimationDef
{
    static int AnimationCreate(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaManager;
};

}
