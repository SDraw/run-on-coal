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
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadText(l_path);
    argStream.ReadNextBoolean(l_loop);
    if(argStream.HasErrors() || !l_path.length())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    Sound *l_sound = LuaManager::m_corePointer->GetElementManager()->CreateSound(l_path,l_loop);
    l_sound ? lua_pushlightuserdata(f_vm,l_sound) : lua_pushboolean(f_vm,0);
    return 1;
}
int soundDestroy(lua_State *f_vm)
{
    Sound *l_sound;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_sound,ElementType::SoundElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    bool result = LuaManager::m_corePointer->GetElementManager()->DestroySound(l_sound);
    lua_pushboolean(f_vm,result);
    return 1;
}

int soundPlay(lua_State *f_vm)
{
    Sound *l_sound;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_sound,ElementType::SoundElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    l_sound->Play();
    lua_pushboolean(f_vm,1);
    return 1;
}
int soundPause(lua_State *f_vm)
{
    Sound *l_sound;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_sound,ElementType::SoundElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    l_sound->Pause();
    lua_pushboolean(f_vm,1);
    return 1;
}
int soundStop(lua_State *f_vm)
{
    Sound *l_sound;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_sound,ElementType::SoundElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    l_sound->Stop();
    lua_pushboolean(f_vm,1);
    return 1;
}

int soundIsLooped(lua_State *f_vm)
{
    Sound *l_sound;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_sound,ElementType::SoundElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    bool l_loop = l_sound->IsLooped();
    lua_pushboolean(f_vm,l_loop);
    return 1;
}

int soundGetState(lua_State *f_vm)
{
    Sound *l_sound;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_sound,ElementType::SoundElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    int l_state = l_sound->GetState();
    !l_state ? lua_pushstring(f_vm,"stopped") : ((l_state == 1) ? lua_pushstring(f_vm,"paused") : lua_pushstring(f_vm,"playing"));
    return 1;
}

int soundSetSpeed(lua_State *f_vm)
{
    Sound *l_sound;
    lua_Number l_val;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_sound,ElementType::SoundElement);
    argStream.ReadNumber(l_val);
    if(argStream.HasErrors() || l_val <= 0.0)
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    l_sound->SetSpeed(static_cast<float>(l_val));
    lua_pushboolean(f_vm,1);
    return 1;
}
int soundGetSpeed(lua_State *f_vm)
{
    Sound *l_sound;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_sound,ElementType::SoundElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    float l_speed = l_sound->GetSpeed();
    lua_pushnumber(f_vm,l_speed);
    return 1;
}

int soundSetVolume(lua_State *f_vm)
{
    Sound *l_sound;
    lua_Number l_val;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_sound,ElementType::SoundElement);
    argStream.ReadNumber(l_val);
    if(argStream.HasErrors() || l_val < 0.0)
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    l_sound->SetVolume(static_cast<float>(l_val));
    lua_pushboolean(f_vm,1);
    return 1;
}
int soundGetVolume(lua_State *f_vm)
{
    Sound *l_sound;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_sound,ElementType::SoundElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    float l_volume = l_sound->GetVolume();
    lua_pushnumber(f_vm,l_volume);
    return 1;
}

int soundSetTime(lua_State *f_vm)
{
    Sound *l_sound;
    lua_Number l_val;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_sound,ElementType::SoundElement);
    argStream.ReadNumber(l_val);
    if(argStream.HasErrors() || l_val < 0.0)
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    l_sound->SetTime(static_cast<float>(l_val));
    lua_pushboolean(f_vm,1);
    return 1;
}
int soundGetTime(lua_State *f_vm)
{
    Sound *l_sound;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_sound,ElementType::SoundElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    float l_time = l_sound->GetTime();
    lua_pushnumber(f_vm,l_time);
    return 1;
}

int soundSet3DEnabled(lua_State *f_vm)
{
    Sound *l_sound;
    bool l_state;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_sound,ElementType::SoundElement);
    argStream.ReadBoolean(l_state);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    bool result = l_sound->Set3DPositionEnabled(l_state);
    lua_pushboolean(f_vm,result);
    return 1;
}
int soundGet3DEnabled(lua_State *f_vm)
{
    Sound *l_sound;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_sound,ElementType::SoundElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    bool result = l_sound->Get3DPositionEnabled();
    lua_pushboolean(f_vm,result);
    return 1;
}

int soundSet3DPosition(lua_State *f_vm)
{
    Sound *l_sound;
    lua_Number l_val[3];
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_sound,ElementType::SoundElement);
    for(int i=0; i < 3; i++) argStream.ReadNumber(l_val[i]);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec3 l_pos(l_val[0],l_val[1],l_val[2]);
    bool result = l_sound->Set3DPosition(l_pos);
    lua_pushboolean(f_vm,result);
    return 1;
}
int soundGet3DPosition(lua_State *f_vm)
{
    Sound *l_sound;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_sound,ElementType::SoundElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec3 l_pos(0.f);
    l_sound->Get3DPosition(l_pos);
    lua_pushnumber(f_vm,l_pos.x);
    lua_pushnumber(f_vm,l_pos.y);
    lua_pushnumber(f_vm,l_pos.z);
    return 3;
}

int soundSet3DDistance(lua_State *f_vm)
{
    Sound *l_sound;
    lua_Number l_val[2];
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_sound,ElementType::SoundElement);
    for(int i=0; i < 2; i++) argStream.ReadNumber(l_val[i]);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec2 l_pos(l_val[0],l_val[1]);
    bool result = l_sound->Set3DDistance(l_pos);
    lua_pushboolean(f_vm,result);
    return 1;
}
int soundGet3DDistance(lua_State *f_vm)
{
    Sound *l_sound;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_sound,ElementType::SoundElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec2 l_pos(0.f);
    l_sound->Get3DDistance(l_pos);
    lua_pushnumber(f_vm,l_pos.x);
    lua_pushnumber(f_vm,l_pos.y);
    return 2;
}

int soundSetListenerOrientation(lua_State *f_vm)
{
    lua_Number l_val[9];
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    for(int i=0; i < 9; i++) argStream.ReadNumber(l_val[i]);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec3 l_pos(l_val[0],l_val[1],l_val[2]);
    glm::vec3 l_dir(l_val[3],l_val[4],l_val[5]);
    glm::vec3 l_up(l_val[6],l_val[7],l_val[8]);
    SoundManager *l_soundManager = LuaManager::m_corePointer->GetSoundManager();
    l_soundManager->SetListenerPosition(l_pos);
    l_soundManager->SetListenerDirection(l_dir);
    l_soundManager->SetListenerUp(l_up);
    lua_pushboolean(f_vm,1);
    return 1;
}

int soundGetListenerOrientation(lua_State *f_vm)
{
    glm::vec3 l_vec[3];
    SoundManager *l_soundManager = LuaManager::m_corePointer->GetSoundManager();
    l_soundManager->GetListenerPosition(l_vec[0]);
    l_soundManager->GetListenerDirection(l_vec[1]);
    l_soundManager->GetListenerUp(l_vec[2]);
    for(int i=0; i < 3; i++)
    {
        lua_pushnumber(f_vm,l_vec[i].x);
        lua_pushnumber(f_vm,l_vec[i].y);
        lua_pushnumber(f_vm,l_vec[i].z);
    }
    return 9;
}

}
}
