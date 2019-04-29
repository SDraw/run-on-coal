#pragma once

class LuaFontDef final
{
    static int Create(lua_State *f_vm);
    static int Draw(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaVM;
};
