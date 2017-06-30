#pragma once

namespace ROC
{

class LuaRenderingDef final
{
    static int SetActiveScene(lua_State *f_vm);
    static int SetActiveShader(lua_State *f_vm);
    static int SetRenderTarget(lua_State *f_vm);
    static int ClearRenderArea(lua_State *f_vm);
    static int SetClearColor(lua_State *f_vm);
    static int SetRenderArea(lua_State *f_vm);
    static int SetPolygonMode(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaManager;
};

}
