#pragma once

namespace ROC
{

class LuaElementDef final
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
    friend class LuaClientDef;
    friend class LuaFileDef;
};

}
