#pragma once

class LuaSoundDef final
{
    static int Create(lua_State *f_vm);
    static int Play(lua_State *f_vm);
    static int Pause(lua_State *f_vm);
    static int Stop(lua_State *f_vm);
    static int IsLooped(lua_State *f_vm);
    static int SetLoop(lua_State *f_vm);
    static int GetState(lua_State *f_vm);
    static int SetSpeed(lua_State *f_vm);
    static int GetSpeed(lua_State *f_vm);
    static int SetVolume(lua_State *f_vm);
    static int GetVolume(lua_State *f_vm);
    static int SetTime(lua_State *f_vm);
    static int GetTime(lua_State *f_vm);
    static int GetDuration(lua_State *f_vm);
    static int Set3DEnabled(lua_State *f_vm);
    static int Get3DEnabled(lua_State *f_vm);
    static int Set3DPosition(lua_State *f_vm);
    static int Get3DPosition(lua_State *f_vm);
    static int Set3DDistance(lua_State *f_vm);
    static int Get3DDistance(lua_State *f_vm);
    static int SetListenerOrientation(lua_State *f_vm);
    static int GetListenerOrientation(lua_State *f_vm);
    static int SetGlobalVolume(lua_State *f_vm);
    static int GetGlobalVolume(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaVM;
};
