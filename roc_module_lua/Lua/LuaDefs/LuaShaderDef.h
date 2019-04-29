#pragma once

class LuaShaderDef final
{
    static int Create(lua_State *f_vm);
    static int SetUniformValue(lua_State *f_vm);
    static int Attach(lua_State *f_vm);
    static int Detach(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaVM;
};
