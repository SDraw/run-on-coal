#pragma once

namespace ROC
{

class LuaAnimationDef final
{
    static int Create(lua_State *f_vm);
    static int GetBonesCount(lua_State *f_vm);
    static int GetDuration(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaManager;
};

}
