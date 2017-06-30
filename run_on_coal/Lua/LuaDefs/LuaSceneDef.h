#pragma once

namespace ROC
{

class LuaSceneDef final
{
    static int SceneCreate(lua_State *f_vm);
    static int SceneSetCamera(lua_State *f_vm);
    static int SceneGetCamera(lua_State *f_vm);
    static int SceneRemoveCamera(lua_State *f_vm);
    static int SceneSetLight(lua_State *f_vm);
    static int SceneGetLight(lua_State *f_vm);
    static int SceneRemoveLight(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaManager;
};

}
