#pragma once

class LuaSceneDef final
{
    static int Create(lua_State *f_vm);
    static int AddModel(lua_State *f_vm);
    static int RemoveModel(lua_State *f_vm);
    static int SetModelLayer(lua_State *f_vm);
    static int SetCamera(lua_State *f_vm);
    static int GetCamera(lua_State *f_vm);
    static int RemoveCamera(lua_State *f_vm);
    static int AddLight(lua_State *f_vm);
    static int RemoveLight(lua_State *f_vm);
    static int SetRenderTarget(lua_State *f_vm);
    static int GetRenderTarget(lua_State *f_vm);
    static int RemoveRenderTarget(lua_State *f_vm);
    static int AddShader(lua_State *f_vm);
    static int RemoveShader(lua_State *f_vm);
    static int SetActive(lua_State *f_vm);
    static int Draw(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaVM;
};
