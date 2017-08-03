#pragma once

namespace ROC
{

class LuaSceneDef final
{
    static int Create(lua_State *f_vm);
    static int SetCamera(lua_State *f_vm);
    static int GetCamera(lua_State *f_vm);
    static int RemoveCamera(lua_State *f_vm);
    static int SetLight(lua_State *f_vm);
    static int GetLight(lua_State *f_vm);
    static int RemoveLight(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaManager;
};

}
