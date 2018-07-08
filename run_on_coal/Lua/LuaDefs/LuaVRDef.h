#pragma once

namespace ROC
{

class LuaVRDef final
{
    static int VRGetHeadPosition(lua_State *f_vm);
    static int VRGetHeadRotation(lua_State *f_vm);
    static int VRGetEyesPosition(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaManager;

};

}
