#pragma once

class LuaDrawableDef final
{
    static int IsDrawable(lua_State *f_vm);
    static int Draw(lua_State *f_vm);
    static int Draw3D(lua_State *f_vm);
    static int GetSize(lua_State *f_vm);
    static int GetFiltering(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);
    static void AddHierarchyMethods(lua_State *f_vm);

    friend class LuaVM;
    friend class LuaRenderTargetDef;
    friend class LuaTextureDef;
};
