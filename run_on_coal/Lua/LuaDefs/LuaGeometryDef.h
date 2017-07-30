#pragma once

namespace ROC
{

class LuaGeometryDef final
{
    static int GeometryCreate(lua_State *f_vm);
    static int GeometryIsLoaded(lua_State *f_vm);
    static int GeometryGetBoundSphereRadius(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaManager;

};

}
