#pragma once

namespace ROC
{

class LuaSoundDef
{
    static int SoundCreate(lua_State *f_vm);
    static int SoundPlay(lua_State *f_vm);
    static int SoundPause(lua_State *f_vm);
    static int SoundStop(lua_State *f_vm);
    static int SoundIsLooped(lua_State *f_vm);
    static int SoundGetState(lua_State *f_vm);
    static int SoundSetSpeed(lua_State *f_vm);
    static int SoundGetSpeed(lua_State *f_vm);
    static int SoundSetVolume(lua_State *f_vm);
    static int SoundGetVolume(lua_State *f_vm);
    static int SoundSetTime(lua_State *f_vm);
    static int SoundGetTime(lua_State *f_vm);
    static int SoundGetDuration(lua_State *f_vm);
    static int SoundSet3DEnabled(lua_State *f_vm);
    static int SoundGet3DEnabled(lua_State *f_vm);
    static int SoundSet3DPosition(lua_State *f_vm);
    static int SoundGet3DPosition(lua_State *f_vm);
    static int SoundSet3DDistance(lua_State *f_vm);
    static int SoundGet3DDistance(lua_State *f_vm);
    static int SoundSetListenerOrientation(lua_State *f_vm);
    static int SoundGetListenerOrientation(lua_State *f_vm);
    static int SoundSetGlobalVolume(lua_State *f_vm);
    static int SoundGetGlobalVolume(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaManager;
};

}
