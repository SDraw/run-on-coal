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
    static int SetRenderTarget(lua_State *f_vm);
    static int GetRenderTarget(lua_State *f_vm);
    static int RemoveRenderTarget(lua_State *f_vm);
    static int SetShader(lua_State *f_vm);
    static int GetShader(lua_State *f_vm);
    static int RemoveShader(lua_State *f_vm);
    static int SetSkyGradient(lua_State *f_vm);
    static int GetSkyGradient(lua_State *f_vm);
    static int SetActive(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaManager;
};

}
