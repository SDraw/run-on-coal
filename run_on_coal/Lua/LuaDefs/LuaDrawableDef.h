#pragma once

namespace ROC
{

class LuaDrawableDef final
{
    static int IsDrawable(lua_State *f_vm);
    static int DrawableDraw(lua_State *f_vm);
    static int DrawableDraw3D(lua_State *f_vm);
    static int DrawableGetSize(lua_State *f_vm);
    static int DrawableGetFiltering(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);
    static void AddHierarchyMethods(lua_State *f_vm);

    friend class LuaManager;
    friend class LuaMovieDef;
    friend class LuaRenderTargetDef;
    friend class LuaTextureDef;
};

}
