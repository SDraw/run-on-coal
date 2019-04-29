#pragma once

class LuaGeometryDef final
{
    static int Create(lua_State *f_vm);
    static int CreateAsync(lua_State *f_vm);
    static int GetBoundSphereRadius(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaVM;

};
