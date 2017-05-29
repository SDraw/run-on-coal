#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Elements/Movie.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Movie.h"

namespace ROC
{
namespace Lua
{

int movieCreate(lua_State *f_vm)
{
    std::string l_path;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_path);
    if(!argStream.HasErrors() && !l_path.empty())
    {
        Movie *l_movie = LuaManager::GetCore()->GetElementManager()->CreateMovie(l_path);
        l_movie ? argStream.PushPointer(l_movie) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int moviePlay(lua_State *f_vm)
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
int moviePause(lua_State *f_vm)
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
int movieStop(lua_State *f_vm)
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
int movieGetSampleRate(lua_State *f_vm)
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
int movieGetChannelCount(lua_State *f_vm)
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
int movieGetFramerate(lua_State *f_vm)
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
int movieGetDuration(lua_State *f_vm)
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
int movieGetVolume(lua_State *f_vm)
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
int movieSetVolume(lua_State *f_vm)
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
int movieGetTime(lua_State *f_vm)
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
int movieSetTime(lua_State *f_vm)
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

}
}
