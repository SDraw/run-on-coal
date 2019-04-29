#pragma once

class LuaCameraDef final
{
    static int Create(lua_State *f_vm);
    static int SetPosition(lua_State *f_vm);
    static int GetPosition(lua_State *f_vm);
    static int SetDirection(lua_State *f_vm);
    static int GetDirection(lua_State *f_vm);
    static int SetUpDirection(lua_State *f_vm);
    static int GetUpDirection(lua_State *f_vm);
    static int SetProjectionType(lua_State *f_vm);
    static int GetProjectionType(lua_State *f_vm);
    static int SetFOV(lua_State *f_vm);
    static int GetFOV(lua_State *f_vm);
    static int SetAspectRatio(lua_State *f_vm);
    static int GetAspectRatio(lua_State *f_vm);
    static int SetOrthoParams(lua_State *f_vm);
    static int GetOrthoParams(lua_State *f_vm);
    static int SetDepth(lua_State *f_vm);
    static int GetDepth(lua_State *f_vm);
    static int GetViewMatrix(lua_State *f_vm);
    static int GetProjectionMatrix(lua_State *f_vm);
    static int GetViewProjectionMatrix(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaVM;
};
