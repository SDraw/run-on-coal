#pragma once

class LuaCollidableDef final
{
    static int IsCollidable(lua_State *f_vm);
    static int SetCollidableWith(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);
    static void AddHierarchyMethods(lua_State *f_vm);

    friend class LuaVM;
    friend class LuaCollisionDef;
    friend class LuaModelDef;
};

