#pragma once

namespace ROC
{

class LuaElementDef
{
    static int IsElement(lua_State *f_vm);
    static int ElementGetType(lua_State *f_vm);
    static int ElementSetData(lua_State *f_vm);
    static int ElementGetData(lua_State *f_vm);
    static int ElementRemoveData(lua_State *f_vm);
    static int ElementDestroy(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);
    static void AddHierarchyMethods(lua_State *f_vm);

    friend class LuaManager;
    friend class LuaAnimationDef;
    friend class LuaCameraDef;
    friend class LuaCollisionDef;
    friend class LuaFileDef;
    friend class LuaFontDef;
    friend class LuaGeometryDef;
    friend class LuaLightDef;
    friend class LuaModelDef;
    friend class LuaMovieDef;
    friend class LuaRenderTargetDef;
    friend class LuaSceneDef;
    friend class LuaShaderDef;
    friend class LuaSoundDef;
    friend class LuaTextureDef;
};

}
