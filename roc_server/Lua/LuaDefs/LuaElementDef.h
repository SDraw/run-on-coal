#pragma once

namespace ROC
{

class LuaElementDef final
{
    static int IsElement(lua_State *f_vm);
    static int GetType(lua_State *f_vm);
    static int SetData(lua_State *f_vm);
    static int GetData(lua_State *f_vm);
    static int RemoveData(lua_State *f_vm);
    static int Destroy(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);
    static void AddHierarchyMethods(lua_State *f_vm);

    friend class LuaManager;
    friend class LuaClientDef;
    friend class LuaFileDef;
};

}
