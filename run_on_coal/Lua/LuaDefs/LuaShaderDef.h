#pragma once

namespace ROC
{

class LuaShaderDef
{
    static int ShaderCreate(lua_State *f_vm);
    static int ShaderSetUniformValue(lua_State *f_vm);
    static int ShaderAttachDrawable(lua_State *f_vm);
    static int ShaderDetachDrawable(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaManager;
};

}
