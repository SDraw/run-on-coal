#pragma once

namespace ROC
{

class LuaMovieDef final
{
    static int Create(lua_State *f_vm);
    static int Play(lua_State *f_vm);
    static int Pause(lua_State *f_vm);
    static int Stop(lua_State *f_vm);
    static int GetSampleRate(lua_State *f_vm);
    static int GetChannelCount(lua_State *f_vm);
    static int GetFramerate(lua_State *f_vm);
    static int GetDuration(lua_State *f_vm);
    static int GetVolume(lua_State *f_vm);
    static int SetVolume(lua_State *f_vm);
    static int GetTime(lua_State *f_vm);
    static int SetTime(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaManager;

};

}
