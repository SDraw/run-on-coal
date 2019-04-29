#pragma once

class LuaLightDef final
{
    static int Create(lua_State *f_vm);
    static int SetPosition(lua_State *f_vm);
    static int GetPosition(lua_State *f_vm);
    static int SetDirection(lua_State *f_vm);
    static int GetDirection(lua_State *f_vm);
    static int SetColor(lua_State *f_vm);
    static int GetColor(lua_State *f_vm);
    static int SetCutoff(lua_State *f_vm);
    static int GetCutoff(lua_State *f_vm);
    static int SetFalloff(lua_State *f_vm);
    static int GetFalloff(lua_State *f_vm);
    static int SetType(lua_State *f_vm);
    static int GetType(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaVM;
};
