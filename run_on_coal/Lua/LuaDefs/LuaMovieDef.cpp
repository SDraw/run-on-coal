#include "stdafx.h"

#include "Lua/LuaDefs/LuaMovieDef.h"
#include "Lua/LuaDefs/LuaDrawableDef.h"
#include "Lua/LuaDefs/LuaElementDef.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Elements/Movie.h"
#include "Lua/ArgReader.h"
#include "Utils/Utils.h"

void ROC::LuaMovieDef::Init(lua_State *f_vm)
{
    Utils::Lua::lua_registerClass(f_vm, "Movie", MovieCreate);
    Utils::Lua::lua_registerClassMethod(f_vm, "play", MoviePlay);
    Utils::Lua::lua_registerClassMethod(f_vm, "pause", MoviePause);
    Utils::Lua::lua_registerClassMethod(f_vm, "stop", MovieStop);
    Utils::Lua::lua_registerClassMethod(f_vm, "getSampleRate", MovieGetSampleRate);
    Utils::Lua::lua_registerClassMethod(f_vm, "getChannelCount", MovieGetChannelCount);
    Utils::Lua::lua_registerClassMethod(f_vm, "getFramerate", MovieGetFramerate);
    Utils::Lua::lua_registerClassMethod(f_vm, "getDuration", MovieGetDuration);
    Utils::Lua::lua_registerClassMethod(f_vm, "getVolume", MovieGetVolume);
    Utils::Lua::lua_registerClassMethod(f_vm, "setVolume", MovieSetVolume);
    Utils::Lua::lua_registerClassMethod(f_vm, "getTime", MovieGetTime);
    Utils::Lua::lua_registerClassMethod(f_vm, "setTime", MovieSetTime);
    LuaDrawableDef::AddHierarchyMethods(f_vm);
    LuaElementDef::AddHierarchyMethods(f_vm);
    Utils::Lua::lua_registerClassFinish(f_vm);
}

int ROC::LuaMovieDef::MovieCreate(lua_State *f_vm)
{
    std::string l_path;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_path);
    if(!argStream.HasErrors() && !l_path.empty())
    {
        Movie *l_movie = LuaManager::GetCore()->GetElementManager()->CreateMovie(l_path);
        l_movie ? argStream.PushElement(l_movie) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaMovieDef::MoviePlay(lua_State *f_vm)
{
    Movie *l_movie;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_movie);
    if(!argStream.HasErrors())
    {
        l_movie->Play();
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaMovieDef::MoviePause(lua_State *f_vm)
{
    Movie *l_movie;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_movie);
    if(!argStream.HasErrors())
    {
        l_movie->Pause();
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaMovieDef::MovieStop(lua_State *f_vm)
{
    Movie *l_movie;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_movie);
    if(!argStream.HasErrors())
    {
        l_movie->Stop();
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaMovieDef::MovieGetSampleRate(lua_State *f_vm)
{
    Movie *l_movie;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_movie);
    if(!argStream.HasErrors())
    {
        unsigned int l_rate = l_movie->GetSampleRate();
        (l_rate != 0U) ? argStream.PushInteger(l_rate) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaMovieDef::MovieGetChannelCount(lua_State *f_vm)
{
    Movie *l_movie;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_movie);
    if(!argStream.HasErrors())
    {
        unsigned int l_channels = l_movie->GetChannelCount();
        (l_channels != 0U) ? argStream.PushInteger(l_channels) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaMovieDef::MovieGetFramerate(lua_State *f_vm)
{
    Movie *l_movie;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_movie);
    if(!argStream.HasErrors())
    {
        float l_framerate = l_movie->GetFramerate();
        (l_framerate != -1.f) ? argStream.PushNumber(l_framerate) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaMovieDef::MovieGetDuration(lua_State *f_vm)
{
    Movie *l_movie;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_movie);
    if(!argStream.HasErrors())
    {
        float l_duration = l_movie->GetDuration();
        (l_duration != -1.f) ? argStream.PushNumber(l_duration) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaMovieDef::MovieGetVolume(lua_State *f_vm)
{
    Movie *l_movie;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_movie);
    if(!argStream.HasErrors())
    {
        float l_volume = l_movie->GetVolume();
        (l_volume != -1.f) ? argStream.PushNumber(l_volume) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaMovieDef::MovieSetVolume(lua_State *f_vm)
{
    Movie *l_movie;
    float l_volume;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_movie);
    argStream.ReadNumber(l_volume);
    if(!argStream.HasErrors())
    {
        l_movie->SetVolume(l_volume);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaMovieDef::MovieGetTime(lua_State *f_vm)
{
    Movie *l_movie;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_movie);
    if(!argStream.HasErrors())
    {
        float l_time = l_movie->GetTime();
        (l_time != -1.f) ? argStream.PushNumber(l_time) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaMovieDef::MovieSetTime(lua_State *f_vm)
{
    Movie *l_movie;
    float l_time;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_movie);
    argStream.ReadNumber(l_time);
    if(!argStream.HasErrors())
    {
        l_movie->SetTime(l_time);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
