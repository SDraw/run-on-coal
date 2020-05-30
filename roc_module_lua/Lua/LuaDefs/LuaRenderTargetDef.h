#pragma once

class LuaRenderTargetDef final
{
    static int Create(lua_State *f_vm);
    static int SetProperty(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaVM;
};
