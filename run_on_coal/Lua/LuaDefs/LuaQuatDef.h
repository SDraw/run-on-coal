#pragma once

namespace ROC
{

class LuaQuatDef final
{
    static int Create(lua_State *f_vm);
    static int Destroy(lua_State *f_vm);
    static int GetXYZW(lua_State *f_vm);
    static int SetXYZW(lua_State *f_vm);
    static int SetEuler(lua_State *f_vm);
    static int Conjugate(lua_State *f_vm);
    static int Inverse(lua_State *f_vm);
    static int Normalize(lua_State *f_vm);
    static int Length(lua_State *f_vm);
    static int Dot(lua_State *f_vm);
    static int Rotate(lua_State *f_vm);
    static int Lerp(lua_State *f_vm);
    static int Slerp(lua_State *f_vm);
    static int Add(lua_State *f_vm);
    static int Multiply(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaManager;
};

}
