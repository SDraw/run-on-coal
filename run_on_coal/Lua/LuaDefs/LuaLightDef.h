#pragma once

namespace ROC
{

class LuaLightDef final
{
    static int Create(lua_State *f_vm);
    static int SetParams(lua_State *f_vm);
    static int GetParams(lua_State *f_vm);
    static int SetColor(lua_State *f_vm);
    static int GetColor(lua_State *f_vm);
    static int SetDirection(lua_State *f_vm);
    static int GetDirection(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaManager;
};

}
