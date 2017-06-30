#pragma once

namespace ROC
{

class LuaMovieDef final
{
    static int MovieCreate(lua_State *f_vm);
    static int MoviePlay(lua_State *f_vm);
    static int MoviePause(lua_State *f_vm);
    static int MovieStop(lua_State *f_vm);
    static int MovieGetSampleRate(lua_State *f_vm);
    static int MovieGetChannelCount(lua_State *f_vm);
    static int MovieGetFramerate(lua_State *f_vm);
    static int MovieGetDuration(lua_State *f_vm);
    static int MovieGetVolume(lua_State *f_vm);
    static int MovieSetVolume(lua_State *f_vm);
    static int MovieGetTime(lua_State *f_vm);
    static int MovieSetTime(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaManager;

};

}
