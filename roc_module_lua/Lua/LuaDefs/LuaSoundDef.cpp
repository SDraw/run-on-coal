#include "stdafx.h"

#include "Lua/LuaDefs/LuaSoundDef.h"

#include "Interfaces/ICore.h"
#include "Interfaces/IElementManager.h"
#include "Interfaces/ISoundManager.h"
#include "Interfaces/ISound.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefs/LuaElementDef.h"
#include "Module/LuaModule.h"
#include "Utils/LuaUtils.h"

const std::string g_SoundStatesTable[]
{
    "stopped", "paused", "playing"
};

void LuaSoundDef::Init(lua_State *f_vm)
{
    LuaUtils::AddClass(f_vm, "Sound", Create);
    LuaUtils::AddClassMethod(f_vm, "play", Play);
    LuaUtils::AddClassMethod(f_vm, "pause", Pause);
    LuaUtils::AddClassMethod(f_vm, "stop", Stop);
    LuaUtils::AddClassMethod(f_vm, "isLooped", IsLooped);
    LuaUtils::AddClassMethod(f_vm, "setLoop", SetLoop);
    LuaUtils::AddClassMethod(f_vm, "getState", GetState);
    LuaUtils::AddClassMethod(f_vm, "setSpeed", SetSpeed);
    LuaUtils::AddClassMethod(f_vm, "getSpeed", GetSpeed);
    LuaUtils::AddClassMethod(f_vm, "setVolume", SetVolume);
    LuaUtils::AddClassMethod(f_vm, "getVolume", GetVolume);
    LuaUtils::AddClassMethod(f_vm, "setTime", SetTime);
    LuaUtils::AddClassMethod(f_vm, "getTime", GetTime);
    LuaUtils::AddClassMethod(f_vm, "getDuration", GetDuration);
    LuaUtils::AddClassMethod(f_vm, "set3DEnabled", Set3DEnabled);
    LuaUtils::AddClassMethod(f_vm, "get3DEnabled", Get3DEnabled);
    LuaUtils::AddClassMethod(f_vm, "set3DPosition", Set3DPosition);
    LuaUtils::AddClassMethod(f_vm, "get3DPosition", Get3DPosition);
    LuaUtils::AddClassMethod(f_vm, "set3DDistance", Set3DDistance);
    LuaUtils::AddClassMethod(f_vm, "get3DDistance", Get3DDistance);
    LuaElementDef::AddHierarchyMethods(f_vm);
    LuaUtils::AddClassFinish(f_vm);

    lua_register(f_vm, "soundSetListenerOrientation", SetListenerOrientation);
    lua_register(f_vm, "soundGetListenerOrientation", SetListenerOrientation);
    lua_register(f_vm, "soundSetGlobalVolume", SetGlobalVolume);
    lua_register(f_vm, "soundGetGlobalVolume", GetGlobalVolume);
}

int LuaSoundDef::Create(lua_State *f_vm)
{
    // element Sound(str path)
    std::string l_path;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_path);
    if(!argStream.HasErrors() && !l_path.empty())
    {
        ROC::ISound *l_sound = LuaModule::GetModule()->GetEngineCore()->GetIElementManager()->CreateISound(l_path);
        l_sound ? argStream.PushElement(l_sound) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaSoundDef::Play(lua_State *f_vm)
{
    // bool Sound:play()
    ROC::ISound *l_sound;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_sound);
    if(!argStream.HasErrors())
    {
        l_sound->Play();
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaSoundDef::Pause(lua_State *f_vm)
{
    // bool Sound:pause()
    ROC::ISound *l_sound;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_sound);
    if(!argStream.HasErrors())
    {
        l_sound->Pause();
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaSoundDef::Stop(lua_State *f_vm)
{
    // bool Sound:stop()
    ROC::ISound *l_sound;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_sound);
    if(!argStream.HasErrors())
    {
        l_sound->Stop();
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaSoundDef::IsLooped(lua_State *f_vm)
{
    // bool Sound:isLooped()
    ROC::ISound *l_sound;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_sound);
    argStream.PushBoolean(!argStream.HasErrors() ? l_sound->IsLooped() : false);
    return argStream.GetReturnValue();
}
int LuaSoundDef::SetLoop(lua_State *f_vm)
{
    // bool Sound:setLoop(bool loop)
    ROC::ISound *l_sound;
    bool l_loop;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_sound);
    argStream.ReadBoolean(l_loop);
    if(!argStream.HasErrors())
    {
        bool l_result = l_sound->SetLoop(l_loop);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaSoundDef::GetState(lua_State *f_vm)
{
    // str Sound:getState()
    ROC::ISound *l_sound;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_sound);
    if(!argStream.HasErrors())
    {
        int l_state = l_sound->GetState();
        (l_state != -1) ? argStream.PushText(g_SoundStatesTable[l_state]) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaSoundDef::SetSpeed(lua_State *f_vm)
{
    // bool Sound:setSpeed(float speed)
    ROC::ISound *l_sound;
    float l_val;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_sound);
    argStream.ReadNumber(l_val);
    if(!argStream.HasErrors())
    {
        l_sound->SetSpeed(l_val);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaSoundDef::GetSpeed(lua_State *f_vm)
{
    // float Sound:getSpeed()
    ROC::ISound *l_sound;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_sound);
    if(!argStream.HasErrors())
    {
        float l_speed = l_sound->GetSpeed();
        (l_speed != -1.f) ? argStream.PushNumber(l_speed) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaSoundDef::SetVolume(lua_State *f_vm)
{
    // bool Sound:setVolume(float volume)
    ROC::ISound *l_sound;
    float l_val;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_sound);
    argStream.ReadNumber(l_val);
    if(!argStream.HasErrors())
    {
        l_sound->SetVolume(l_val);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaSoundDef::GetVolume(lua_State *f_vm)
{
    // float Sound:getVolume()
    ROC::ISound *l_sound;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_sound);
    if(!argStream.HasErrors())
    {
        float l_volume = l_sound->GetVolume();
        (l_volume != -1.f) ? argStream.PushNumber(l_volume) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaSoundDef::SetTime(lua_State *f_vm)
{
    // bool Sound:setTime(float time)
    ROC::ISound *l_sound;
    float l_val;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_sound);
    argStream.ReadNumber(l_val);
    if(!argStream.HasErrors())
    {
        l_sound->SetTime(l_val);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaSoundDef::GetTime(lua_State *f_vm)
{
    // float Sound:getTime()
    ROC::ISound *l_sound;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_sound);
    if(!argStream.HasErrors())
    {
        float l_time = l_sound->GetTime();
        (l_time != -1.f) ? argStream.PushNumber(l_time) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaSoundDef::GetDuration(lua_State *f_vm)
{
    // float Sound:getDuration()
    ROC::ISound *l_sound;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_sound);
    if(!argStream.HasErrors())
    {
        float l_duration = l_sound->GetDuration();
        (l_duration != -1.f) ? argStream.PushNumber(l_duration) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaSoundDef::Set3DEnabled(lua_State *f_vm)
{
    // bool Sound:set3DEnabled(bool state)
    ROC::ISound *l_sound;
    bool l_state;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_sound);
    argStream.ReadBoolean(l_state);
    if(!argStream.HasErrors())
    {
        bool l_result = l_sound->Set3DPositionEnabled(l_state);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaSoundDef::Get3DEnabled(lua_State *f_vm)
{
    // bool Sound:get3DEnabled()
    ROC::ISound *l_sound;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_sound);
    argStream.PushBoolean(!argStream.HasErrors() ? l_sound->Get3DPositionEnabled() : false);
    return argStream.GetReturnValue();
}

int LuaSoundDef::Set3DPosition(lua_State *f_vm)
{
    // bool Sound:set3DPosition(float x, float y, float z)
    ROC::ISound *l_sound;
    glm::vec3 l_pos;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_sound);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_pos[i]);
    if(!argStream.HasErrors())
    {
        bool l_result = l_sound->Set3DPosition(l_pos);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaSoundDef::Get3DPosition(lua_State *f_vm)
{
    // float float float Sound:get3DPosition()
    ROC::ISound *l_sound;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_sound);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_pos(0.f);
        l_sound->Get3DPosition(l_pos);
        argStream.PushNumber(l_pos.x);
        argStream.PushNumber(l_pos.y);
        argStream.PushNumber(l_pos.z);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaSoundDef::Set3DDistance(lua_State *f_vm)
{
    // bool Sound:set3DDistance(float distance, float attenuation)
    ROC::ISound *l_sound;
    glm::vec2 l_dist;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_sound);
    for(int i = 0; i < 2; i++) argStream.ReadNumber(l_dist[i]);
    if(!argStream.HasErrors())
    {
        bool l_result = l_sound->Set3DDistance(l_dist);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaSoundDef::Get3DDistance(lua_State *f_vm)
{
    // float float Sound:get3DDistance()
    ROC::ISound *l_sound;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_sound);
    if(!argStream.HasErrors())
    {
        glm::vec2 l_dist(0.f);
        l_sound->Get3DDistance(l_dist);
        argStream.PushNumber(l_dist.x);
        argStream.PushNumber(l_dist.y);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaSoundDef::SetListenerOrientation(lua_State *f_vm)
{
    // bool soundSetListenerOrientation(float x, float y, float z, float dirX, float dirY, float dirZ, float upX, float upY, float upZ)
    glm::vec3 l_pos, l_dir, l_up;
    ArgReader argStream(f_vm);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_pos[i]);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_dir[i]);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_up[i]);
    if(!argStream.HasErrors())
    {
        ROC::ISoundManager *l_soundManager = LuaModule::GetModule()->GetEngineCore()->GetISoundManager();
        l_soundManager->SetListenerPosition(l_pos);
        l_soundManager->SetListenerDirection(l_dir);
        l_soundManager->SetListenerUp(l_up);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaSoundDef::GetListenerOrientation(lua_State *f_vm)
{
    // float float float float float float float float float soundGetListenerOrientation()
    ArgReader argStream(f_vm);
    ROC::ISoundManager *l_soundManager = LuaModule::GetModule()->GetEngineCore()->GetISoundManager();

    const glm::vec3 &l_pos = l_soundManager->GetListenerPosition();
    for(int i = 0; i < 3; i++) argStream.PushNumber(l_pos[i]);

    const glm::vec3 &l_dir = l_soundManager->GetListenerDirection();
    for(int i = 0; i < 3; i++) argStream.PushNumber(l_dir[i]);

    const glm::vec3 &l_up = l_soundManager->GetListenerUp();
    for(int i = 0; i < 3; i++) argStream.PushNumber(l_up[i]);

    return argStream.GetReturnValue();
}

int LuaSoundDef::SetGlobalVolume(lua_State *f_vm)
{
    // bool soundSetGlobalVolume(float volume)
    float l_volume;
    ArgReader argStream(f_vm);
    argStream.ReadNumber(l_volume);
    if(!argStream.HasErrors())
    {
        LuaModule::GetModule()->GetEngineCore()->GetISoundManager()->SetGlobalVolume(l_volume);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaSoundDef::GetGlobalVolume(lua_State *f_vm)
{
    // float soundGetGlobalVolume()
    ArgReader argStream(f_vm);
    argStream.PushNumber(LuaModule::GetModule()->GetEngineCore()->GetISoundManager()->GetGlobalVolume());
    return argStream.GetReturnValue();
}
