#include "stdafx.h"

#include "Lua/LuaDefs/LuaSoundDef.h"

#include "Lua/LuaArgReader.h"
#include "Lua/LuaDefs/LuaElementDef.h"
#include "Core/Core.h"
#include "Utils/LuaUtils.h"

const std::string g_soundStates[]
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
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadText(l_path);
    if(!l_argStream.HasErrors() && !l_path.empty())
    {
        ROC::ISound *l_sound = Core::GetInstance()->GetEngineICore()->GetIElementManager()->CreateISound(l_path);
        l_sound ? l_argStream.PushElement(l_sound) : l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSoundDef::Play(lua_State *f_vm)
{
    // bool Sound:play()
    ROC::ISound *l_sound;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_sound);
    if(!l_argStream.HasErrors())
    {
        l_sound->Play();
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSoundDef::Pause(lua_State *f_vm)
{
    // bool Sound:pause()
    ROC::ISound *l_sound;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_sound);
    if(!l_argStream.HasErrors())
    {
        l_sound->Pause();
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSoundDef::Stop(lua_State *f_vm)
{
    // bool Sound:stop()
    ROC::ISound *l_sound;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_sound);
    if(!l_argStream.HasErrors())
    {
        l_sound->Stop();
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSoundDef::IsLooped(lua_State *f_vm)
{
    // bool Sound:isLooped()
    ROC::ISound *l_sound;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_sound);
    l_argStream.PushBoolean(!l_argStream.HasErrors() ? l_sound->IsLooped() : false);
    return l_argStream.GetReturnValue();
}

int LuaSoundDef::SetLoop(lua_State *f_vm)
{
    // bool Sound:setLoop(bool loop)
    ROC::ISound *l_sound;
    bool l_loop;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_sound);
    l_argStream.ReadBoolean(l_loop);
    if(!l_argStream.HasErrors())
    {
        bool l_result = l_sound->SetLoop(l_loop);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSoundDef::GetState(lua_State *f_vm)
{
    // str Sound:getState()
    ROC::ISound *l_sound;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_sound);
    if(!l_argStream.HasErrors())
    {
        int l_state = l_sound->GetState();
        (l_state != -1) ? l_argStream.PushText(g_soundStates[l_state]) : l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSoundDef::SetSpeed(lua_State *f_vm)
{
    // bool Sound:setSpeed(float speed)
    ROC::ISound *l_sound;
    float l_val;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_sound);
    l_argStream.ReadNumber(l_val);
    if(!l_argStream.HasErrors())
    {
        l_sound->SetSpeed(l_val);
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSoundDef::GetSpeed(lua_State *f_vm)
{
    // float Sound:getSpeed()
    ROC::ISound *l_sound;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_sound);
    if(!l_argStream.HasErrors())
    {
        float l_speed = l_sound->GetSpeed();
        (l_speed != -1.f) ? l_argStream.PushNumber(l_speed) : l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSoundDef::SetVolume(lua_State *f_vm)
{
    // bool Sound:setVolume(float volume)
    ROC::ISound *l_sound;
    float l_val;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_sound);
    l_argStream.ReadNumber(l_val);
    if(!l_argStream.HasErrors())
    {
        l_sound->SetVolume(l_val);
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSoundDef::GetVolume(lua_State *f_vm)
{
    // float Sound:getVolume()
    ROC::ISound *l_sound;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_sound);
    if(!l_argStream.HasErrors())
    {
        float l_volume = l_sound->GetVolume();
        (l_volume != -1.f) ? l_argStream.PushNumber(l_volume) : l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSoundDef::SetTime(lua_State *f_vm)
{
    // bool Sound:setTime(float time)
    ROC::ISound *l_sound;
    float l_val;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_sound);
    l_argStream.ReadNumber(l_val);
    if(!l_argStream.HasErrors())
    {
        l_sound->SetTime(l_val);
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSoundDef::GetTime(lua_State *f_vm)
{
    // float Sound:getTime()
    ROC::ISound *l_sound;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_sound);
    if(!l_argStream.HasErrors())
    {
        float l_time = l_sound->GetTime();
        (l_time != -1.f) ? l_argStream.PushNumber(l_time) : l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSoundDef::GetDuration(lua_State *f_vm)
{
    // float Sound:getDuration()
    ROC::ISound *l_sound;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_sound);
    if(!l_argStream.HasErrors())
    {
        float l_duration = l_sound->GetDuration();
        (l_duration != -1.f) ? l_argStream.PushNumber(l_duration) : l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSoundDef::Set3DEnabled(lua_State *f_vm)
{
    // bool Sound:set3DEnabled(bool state)
    ROC::ISound *l_sound;
    bool l_state;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_sound);
    l_argStream.ReadBoolean(l_state);
    if(!l_argStream.HasErrors())
    {
        bool l_result = l_sound->Set3DPositionEnabled(l_state);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSoundDef::Get3DEnabled(lua_State *f_vm)
{
    // bool Sound:get3DEnabled()
    ROC::ISound *l_sound;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_sound);
    l_argStream.PushBoolean(!l_argStream.HasErrors() ? l_sound->Get3DPositionEnabled() : false);
    return l_argStream.GetReturnValue();
}

int LuaSoundDef::Set3DPosition(lua_State *f_vm)
{
    // bool Sound:set3DPosition(float x, float y, float z)
    ROC::ISound *l_sound;
    glm::vec3 l_pos;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_sound);
    for(int i = 0; i < 3; i++) l_argStream.ReadNumber(l_pos[i]);
    if(!l_argStream.HasErrors())
    {
        bool l_result = l_sound->Set3DPosition(l_pos);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSoundDef::Get3DPosition(lua_State *f_vm)
{
    // float float float Sound:get3DPosition()
    ROC::ISound *l_sound;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_sound);
    if(!l_argStream.HasErrors())
    {
        glm::vec3 l_pos(0.f);
        l_sound->Get3DPosition(l_pos);
        l_argStream.PushNumber(l_pos.x);
        l_argStream.PushNumber(l_pos.y);
        l_argStream.PushNumber(l_pos.z);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSoundDef::Set3DDistance(lua_State *f_vm)
{
    // bool Sound:set3DDistance(float distance, float attenuation)
    ROC::ISound *l_sound;
    glm::vec2 l_dist;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_sound);
    for(int i = 0; i < 2; i++) l_argStream.ReadNumber(l_dist[i]);
    if(!l_argStream.HasErrors())
    {
        bool l_result = l_sound->Set3DDistance(l_dist);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSoundDef::Get3DDistance(lua_State *f_vm)
{
    // float float Sound:get3DDistance()
    ROC::ISound *l_sound;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_sound);
    if(!l_argStream.HasErrors())
    {
        glm::vec2 l_dist(0.f);
        l_sound->Get3DDistance(l_dist);
        l_argStream.PushNumber(l_dist.x);
        l_argStream.PushNumber(l_dist.y);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSoundDef::SetListenerOrientation(lua_State *f_vm)
{
    // bool soundSetListenerOrientation(float x, float y, float z, float dirX, float dirY, float dirZ, float upX, float upY, float upZ)
    glm::vec3 l_pos, l_dir, l_up;
    LuaArgReader l_argStream(f_vm);
    for(int i = 0; i < 3; i++) l_argStream.ReadNumber(l_pos[i]);
    for(int i = 0; i < 3; i++) l_argStream.ReadNumber(l_dir[i]);
    for(int i = 0; i < 3; i++) l_argStream.ReadNumber(l_up[i]);
    if(!l_argStream.HasErrors())
    {
        ROC::ISoundManager *l_soundManager = Core::GetInstance()->GetEngineICore()->GetISoundManager();
        l_soundManager->SetListenerPosition(l_pos);
        l_soundManager->SetListenerDirection(l_dir);
        l_soundManager->SetListenerUp(l_up);
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSoundDef::GetListenerOrientation(lua_State *f_vm)
{
    // float float float float float float float float float soundGetListenerOrientation()
    LuaArgReader l_argStream(f_vm);
    ROC::ISoundManager *l_soundManager = Core::GetInstance()->GetEngineICore()->GetISoundManager();

    const glm::vec3 &l_pos = l_soundManager->GetListenerPosition();
    for(int i = 0; i < 3; i++) l_argStream.PushNumber(l_pos[i]);

    const glm::vec3 &l_dir = l_soundManager->GetListenerDirection();
    for(int i = 0; i < 3; i++) l_argStream.PushNumber(l_dir[i]);

    const glm::vec3 &l_up = l_soundManager->GetListenerUp();
    for(int i = 0; i < 3; i++) l_argStream.PushNumber(l_up[i]);

    return l_argStream.GetReturnValue();
}

int LuaSoundDef::SetGlobalVolume(lua_State *f_vm)
{
    // bool soundSetGlobalVolume(float volume)
    float l_volume;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadNumber(l_volume);
    if(!l_argStream.HasErrors())
    {
        Core::GetInstance()->GetEngineICore()->GetISoundManager()->SetGlobalVolume(l_volume);
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}
int LuaSoundDef::GetGlobalVolume(lua_State *f_vm)
{
    // float soundGetGlobalVolume()
    LuaArgReader l_argStream(f_vm);
    l_argStream.PushNumber(Core::GetInstance()->GetEngineICore()->GetISoundManager()->GetGlobalVolume());
    return l_argStream.GetReturnValue();
}
