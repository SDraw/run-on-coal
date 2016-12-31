
#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/SoundManager.h"
#include "Scene/Sound.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Sound.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Lua
{

int soundCreate(lua_State *f_vm)
{
    std::string l_path;
    bool l_loop = false;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadText(l_path);
    argStream.ReadNextBoolean(l_loop);
    if(!argStream.HasErrors() && !l_path.empty())
    {
        Sound *l_sound = LuaManager::m_corePointer->GetElementManager()->CreateSound(l_path, l_loop);
        l_sound ? lua_pushlightuserdata(f_vm, l_sound) : lua_pushboolean(f_vm, 0);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int soundDestroy(lua_State *f_vm)
{
    Sound *l_sound;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_sound), ElementType::SoundElement);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::m_corePointer->GetElementManager()->DestroySound(l_sound);
        lua_pushboolean(f_vm, l_result);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}

int soundPlay(lua_State *f_vm)
{
    Sound *l_sound;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_sound), ElementType::SoundElement);
    if(!argStream.HasErrors())
    {
        l_sound->Play();
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int soundPause(lua_State *f_vm)
{
    Sound *l_sound;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_sound), ElementType::SoundElement);
    if(!argStream.HasErrors())
    {
        l_sound->Pause();
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int soundStop(lua_State *f_vm)
{
    Sound *l_sound;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_sound), ElementType::SoundElement);
    if(!argStream.HasErrors())
    {
        l_sound->Stop();
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}

int soundIsLooped(lua_State *f_vm)
{
    Sound *l_sound;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_sound), ElementType::SoundElement);
    if(!argStream.HasErrors()) lua_pushboolean(f_vm, l_sound->IsLooped());
    else lua_pushboolean(f_vm, 0);
    return 1;
}

int soundGetState(lua_State *f_vm)
{
    Sound *l_sound;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_sound), ElementType::SoundElement);
    if(!argStream.HasErrors())
    {
        int l_state = l_sound->GetState();
        !l_state ? lua_pushstring(f_vm, "stopped") : ((l_state == 1) ? lua_pushstring(f_vm, "paused") : lua_pushstring(f_vm, "playing"));
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}

int soundSetSpeed(lua_State *f_vm)
{
    Sound *l_sound;
    float l_val;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_sound), ElementType::SoundElement);
    argStream.ReadNumber(l_val);
    if(!argStream.HasErrors())
    {
        l_sound->SetSpeed(l_val);
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int soundGetSpeed(lua_State *f_vm)
{
    Sound *l_sound;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_sound), ElementType::SoundElement);
    if(!argStream.HasErrors()) lua_pushnumber(f_vm, l_sound->GetSpeed());
    else lua_pushboolean(f_vm, 0);
    return 1;
}

int soundSetVolume(lua_State *f_vm)
{
    Sound *l_sound;
    float l_val;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_sound), ElementType::SoundElement);
    argStream.ReadNumber(l_val);
    if(!argStream.HasErrors())
    {
        l_sound->SetVolume(l_val);
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int soundGetVolume(lua_State *f_vm)
{
    Sound *l_sound;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_sound), ElementType::SoundElement);
    if(!argStream.HasErrors()) lua_pushnumber(f_vm, l_sound->GetVolume());
    else lua_pushboolean(f_vm, 0);
    return 1;
}

int soundSetTime(lua_State *f_vm)
{
    Sound *l_sound;
    float l_val;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_sound), ElementType::SoundElement);
    argStream.ReadNumber(l_val);
    if(!argStream.HasErrors())
    {
        l_sound->SetTime(l_val);
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int soundGetTime(lua_State *f_vm)
{
    Sound *l_sound;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_sound), ElementType::SoundElement);
    if(!argStream.HasErrors())
    {
        float l_time = l_sound->GetTime();
        lua_pushnumber(f_vm, l_time);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int soundGetDuration(lua_State *f_vm)
{
    Sound *l_sound;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_sound), ElementType::SoundElement);
    if(!argStream.HasErrors())
    {
        float l_duration = l_sound->GetDuration();
        lua_pushnumber(f_vm, l_duration);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}

int soundSet3DEnabled(lua_State *f_vm)
{
    Sound *l_sound;
    bool l_state;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_sound), ElementType::SoundElement);
    argStream.ReadBoolean(l_state);
    if(!argStream.HasErrors())
    {
        bool l_result = l_sound->Set3DPositionEnabled(l_state);
        lua_pushboolean(f_vm, l_result);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int soundGet3DEnabled(lua_State *f_vm)
{
    Sound *l_sound;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_sound), ElementType::SoundElement);
    if(!argStream.HasErrors()) lua_pushboolean(f_vm, l_sound->Get3DPositionEnabled());
    else lua_pushboolean(f_vm, 0);
    return 1;
}

int soundSet3DPosition(lua_State *f_vm)
{
    Sound *l_sound;
    glm::vec3 l_pos;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_sound), ElementType::SoundElement);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_pos[i]);
    if(!argStream.HasErrors())
    {
        bool l_result = l_sound->Set3DPosition(l_pos);
        lua_pushboolean(f_vm, l_result);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int soundGet3DPosition(lua_State *f_vm)
{
    Sound *l_sound;
    int l_returnVal = 1;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_sound), ElementType::SoundElement);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_pos(0.f);
        l_sound->Get3DPosition(l_pos);
        lua_pushnumber(f_vm, l_pos.x);
        lua_pushnumber(f_vm, l_pos.y);
        lua_pushnumber(f_vm, l_pos.z);
        l_returnVal = 3;
    }
    else lua_pushboolean(f_vm, 0);
    return l_returnVal;
}

int soundSet3DDistance(lua_State *f_vm)
{
    Sound *l_sound;
    glm::vec2 l_dist;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_sound), ElementType::SoundElement);
    for(int i = 0; i < 2; i++) argStream.ReadNumber(l_dist[i]);
    if(!argStream.HasErrors())
    {
        bool l_result = l_sound->Set3DDistance(l_dist);
        lua_pushboolean(f_vm, l_result);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int soundGet3DDistance(lua_State *f_vm)
{
    Sound *l_sound;
    int l_returnVal = 1;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_sound), ElementType::SoundElement);
    if(!argStream.HasErrors())
    {
        glm::vec2 l_dist(0.f);
        l_sound->Get3DDistance(l_dist);
        lua_pushnumber(f_vm, l_dist.x);
        lua_pushnumber(f_vm, l_dist.y);
        l_returnVal++;
    }
    else lua_pushboolean(f_vm, 0);
    return l_returnVal;
}

int soundSetListenerOrientation(lua_State *f_vm)
{
    glm::vec3 l_pos, l_dir, l_up;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_pos[i]);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_dir[i]);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_up[i]);
    if(!argStream.HasErrors())
    {
        LuaManager::m_corePointer->GetSoundManager()->SetListenerPosition(l_pos);
        LuaManager::m_corePointer->GetSoundManager()->SetListenerDirection(l_dir);
        LuaManager::m_corePointer->GetSoundManager()->SetListenerUp(l_up);
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}

int soundGetListenerOrientation(lua_State *f_vm)
{
    glm::vec3 l_pos, l_dir, l_up;
    LuaManager::m_corePointer->GetSoundManager()->GetListenerPosition(l_pos);
    lua_pushnumber(f_vm, l_pos.x);
    lua_pushnumber(f_vm, l_pos.y);
    lua_pushnumber(f_vm, l_pos.z);
    LuaManager::m_corePointer->GetSoundManager()->GetListenerDirection(l_dir);
    lua_pushnumber(f_vm, l_dir.x);
    lua_pushnumber(f_vm, l_dir.y);
    lua_pushnumber(f_vm, l_dir.z);
    LuaManager::m_corePointer->GetSoundManager()->GetListenerUp(l_up);
    lua_pushnumber(f_vm, l_up.x);
    lua_pushnumber(f_vm, l_up.y);
    lua_pushnumber(f_vm, l_up.z);
    return 9;
}

int soundSetGlobalVolume(lua_State *f_vm)
{
    float l_volume;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadNumber(l_volume);
    if(!argStream.HasErrors())
    {
        LuaManager::m_corePointer->GetSoundManager()->SetGlobalVolume(l_volume);
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int soundGetGlobalVolume(lua_State *f_vm)
{
    lua_pushnumber(f_vm, LuaManager::m_corePointer->GetSoundManager()->GetGlobalVolume());
    return 1;
}

}
}