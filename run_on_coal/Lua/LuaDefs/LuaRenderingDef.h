#pragma once

namespace ROC
{

class LuaRenderingDef final
{
    static int ClearRenderArea(lua_State *f_vm);
    static int SetClearColor(lua_State *f_vm);
    static int SetRenderArea(lua_State *f_vm);
    static int SetPolygonMode(lua_State *f_vm);
    static int DrawPhysics(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaManager;
};

}
