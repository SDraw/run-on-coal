#include "stdafx.h"

#include "Lua/LuaDefs/LuaPhysicsDef.h"

#include "Lua/LuaArgReader.h"
#include "Core/Core.h"
#include "Utils/LuaUtils.h"

void LuaPhysicsDef::Init(lua_State *f_vm)
{
    lua_register(f_vm, "physicsSetEnabled", SetEnabled);
    lua_register(f_vm, "physicsGetEnabled", GetEnabled);
    lua_register(f_vm, "physicsSetFloorEnabled", SetFloorEnabled);
    lua_register(f_vm, "physicsGetFloorEnabled", GetFloorEnabled);
    lua_register(f_vm, "physicsSetGravity", SetGravity);
    lua_register(f_vm, "physicsGetGravity", GetGravity);
    lua_register(f_vm, "physicsRayCast", RayCast);
}

int LuaPhysicsDef::SetEnabled(lua_State *f_vm)
{
    // bool physicsSetEnabled(bool state)
    bool l_val;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadBoolean(l_val);
    if(!l_argStream.HasErrors())
    {
        Core::GetInstance()->GetEngineICore()->GetIPhysicsManager()->SetPhysicsEnabled(l_val);
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaPhysicsDef::GetEnabled(lua_State *f_vm)
{
    // bool physicsGetEnabled()
    LuaArgReader l_argStream(f_vm);
    l_argStream.PushBoolean(Core::GetInstance()->GetEngineICore()->GetIPhysicsManager()->GetPhysicsEnabled());
    return l_argStream.GetReturnValue();
}

int LuaPhysicsDef::SetFloorEnabled(lua_State *f_vm)
{
    // bool physicsSetFloorEnabled(bool state)
    bool l_val;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadBoolean(l_val);
    if(!l_argStream.HasErrors())
    {
        Core::GetInstance()->GetEngineICore()->GetIPhysicsManager()->SetFloorEnabled(l_val);
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaPhysicsDef::GetFloorEnabled(lua_State *f_vm)
{
    // bool physicsGetFloorEnabled()
    LuaArgReader l_argStream(f_vm);
    l_argStream.PushBoolean(Core::GetInstance()->GetEngineICore()->GetIPhysicsManager()->GetFloorEnabled());
    return l_argStream.GetReturnValue();
}

int LuaPhysicsDef::SetGravity(lua_State *f_vm)
{
    // bool physicsSetGravity(float x, float y, float z)
    glm::vec3 l_gravity;
    LuaArgReader l_argStream(f_vm);
    for(int i = 0; i < 3; i++) l_argStream.ReadNumber(l_gravity[i]);
    if(!l_argStream.HasErrors())
    {
        Core::GetInstance()->GetEngineICore()->GetIPhysicsManager()->SetGravity(l_gravity);
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaPhysicsDef::GetGravity(lua_State *f_vm)
{
    // float float float physicsGetGravity()
    LuaArgReader l_argStream(f_vm);
    glm::vec3 l_grav;
    Core::GetInstance()->GetEngineICore()->GetIPhysicsManager()->GetGravity(l_grav);
    l_argStream.PushNumber(l_grav.x);
    l_argStream.PushNumber(l_grav.y);
    l_argStream.PushNumber(l_grav.z);
    return l_argStream.GetReturnValue();
}

int LuaPhysicsDef::RayCast(lua_State *f_vm)
{
    // float float float float float float element physicsRayCast(float startX, float startY, float startZ, float endX, float endY, float endZ)
    glm::vec3 l_start, l_end;
    LuaArgReader l_argStream(f_vm);
    for(int i = 0; i < 3; i++) l_argStream.ReadNumber(l_start[i]);
    for(int i = 0; i < 3; i++) l_argStream.ReadNumber(l_end[i]);
    if(!l_argStream.HasErrors())
    {
        glm::vec3 l_hitNormal;
        ROC::IElement *l_hitElement = nullptr;
        if(Core::GetInstance()->GetEngineICore()->GetIPhysicsManager()->RayCast(l_start, l_end, l_hitNormal, l_hitElement))
        {
            l_argStream.PushNumber(l_end.x);
            l_argStream.PushNumber(l_end.y);
            l_argStream.PushNumber(l_end.z);
            l_argStream.PushNumber(l_hitNormal.x);
            l_argStream.PushNumber(l_hitNormal.y);
            l_argStream.PushNumber(l_hitNormal.z);
            l_hitElement ? l_argStream.PushElement(l_hitElement) : l_argStream.PushBoolean(false);
        }
        else l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}
