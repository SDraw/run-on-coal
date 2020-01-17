#include "stdafx.h"

#include "Lua/LuaDefs/LuaPhysicsDef.h"

#include "Lua/ArgReader.h"
#include "Module/LuaModule.h"
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
    ArgReader argStream(f_vm);
    argStream.ReadBoolean(l_val);
    if(!argStream.HasErrors())
    {
        LuaModule::GetModule()->GetEngineCore()->GetIPhysicsManager()->SetPhysicsEnabled(l_val);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaPhysicsDef::GetEnabled(lua_State *f_vm)
{
    // bool physicsGetEnabled()
    ArgReader argStream(f_vm);
    argStream.PushBoolean(LuaModule::GetModule()->GetEngineCore()->GetIPhysicsManager()->GetPhysicsEnabled());
    return argStream.GetReturnValue();
}

int LuaPhysicsDef::SetFloorEnabled(lua_State *f_vm)
{
    // bool physicsSetFloorEnabled(bool state)
    bool l_val;
    ArgReader argStream(f_vm);
    argStream.ReadBoolean(l_val);
    if(!argStream.HasErrors())
    {
        LuaModule::GetModule()->GetEngineCore()->GetIPhysicsManager()->SetFloorEnabled(l_val);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaPhysicsDef::GetFloorEnabled(lua_State *f_vm)
{
    // bool physicsGetFloorEnabled()
    ArgReader argStream(f_vm);
    argStream.PushBoolean(LuaModule::GetModule()->GetEngineCore()->GetIPhysicsManager()->GetFloorEnabled());
    return argStream.GetReturnValue();
}

int LuaPhysicsDef::SetGravity(lua_State *f_vm)
{
    // bool physicsSetGravity(float x, float y, float z)
    glm::vec3 l_gravity;
    ArgReader argStream(f_vm);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_gravity[i]);
    if(!argStream.HasErrors())
    {
        LuaModule::GetModule()->GetEngineCore()->GetIPhysicsManager()->SetGravity(l_gravity);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaPhysicsDef::GetGravity(lua_State *f_vm)
{
    // float float float physicsGetGravity()
    ArgReader argStream(f_vm);
    glm::vec3 l_grav;
    LuaModule::GetModule()->GetEngineCore()->GetIPhysicsManager()->GetGravity(l_grav);
    argStream.PushNumber(l_grav.x);
    argStream.PushNumber(l_grav.y);
    argStream.PushNumber(l_grav.z);
    return argStream.GetReturnValue();
}

int LuaPhysicsDef::RayCast(lua_State *f_vm)
{
    // float float float float float float element physicsRayCast(float startX, float startY, float startZ, float endX, float endY, float endZ)
    glm::vec3 l_start, l_end;
    ArgReader argStream(f_vm);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_start[i]);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_end[i]);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_hitNormal;
        ROC::IElement *l_hitElement = nullptr;
        if(LuaModule::GetModule()->GetEngineCore()->GetIPhysicsManager()->RayCast(l_start, l_end, l_hitNormal, l_hitElement))
        {
            argStream.PushNumber(l_end.x);
            argStream.PushNumber(l_end.y);
            argStream.PushNumber(l_end.z);
            argStream.PushNumber(l_hitNormal.x);
            argStream.PushNumber(l_hitNormal.y);
            argStream.PushNumber(l_hitNormal.z);
            l_hitElement ? argStream.PushElement(l_hitElement) : argStream.PushBoolean(false);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
