#include "stdafx.h"

#include "Lua/LuaDefs/LuaPhysicsDef.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/PhysicsManager.h"
#include "Elements/Model/Model.h"
#include "Lua/ArgReader.h"

void ROC::LuaPhysicsDef::Init(lua_State *f_vm)
{
    lua_register(f_vm, "physicsSetEnabled", PhysicsSetEnabled);
    lua_register(f_vm, "physicsGetEnabled", PhysicsGetEnabled);
    lua_register(f_vm, "physicsSetFloorEnabled", PhysicsSetFloorEnabled);
    lua_register(f_vm, "physicsGetFloorEnabled", PhysicsGetFloorEnabled);
    lua_register(f_vm, "physicsSetGravity", PhysicsSetGravity);
    lua_register(f_vm, "physicsGetGravity", PhysicsGetGravity);
    lua_register(f_vm, "physicsRayCast", PhysicsRayCast);
}

int ROC::LuaPhysicsDef::PhysicsSetEnabled(lua_State *f_vm)
{
    bool l_val;
    ArgReader argStream(f_vm);
    argStream.ReadBoolean(l_val);
    if(!argStream.HasErrors())
    {
        LuaManager::GetCore()->GetPhysicsManager()->SetPhysicsEnabled(l_val);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaPhysicsDef::PhysicsGetEnabled(lua_State *f_vm)
{
    ArgReader argStream(f_vm);
    argStream.PushBoolean(LuaManager::GetCore()->GetPhysicsManager()->GetPhysicsEnabled());
    return argStream.GetReturnValue();
}

int ROC::LuaPhysicsDef::PhysicsSetFloorEnabled(lua_State *f_vm)
{
    bool l_val;
    ArgReader argStream(f_vm);
    argStream.ReadBoolean(l_val);
    if(!argStream.HasErrors())
    {
        LuaManager::GetCore()->GetPhysicsManager()->SetFloorEnabled(l_val);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaPhysicsDef::PhysicsGetFloorEnabled(lua_State *f_vm)
{
    ArgReader argStream(f_vm);
    argStream.PushBoolean(LuaManager::GetCore()->GetPhysicsManager()->GetFloorEnabled());
    return argStream.GetReturnValue();
}

int ROC::LuaPhysicsDef::PhysicsSetGravity(lua_State *f_vm)
{
    glm::vec3 l_gravity;
    ArgReader argStream(f_vm);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_gravity[i]);
    if(!argStream.HasErrors())
    {
        LuaManager::GetCore()->GetPhysicsManager()->SetGravity(l_gravity);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaPhysicsDef::PhysicsGetGravity(lua_State *f_vm)
{
    ArgReader argStream(f_vm);
    glm::vec3 l_grav;
    LuaManager::GetCore()->GetPhysicsManager()->GetGravity(l_grav);
    argStream.PushNumber(l_grav.x);
    argStream.PushNumber(l_grav.y);
    argStream.PushNumber(l_grav.z);
    return argStream.GetReturnValue();
}

int ROC::LuaPhysicsDef::PhysicsRayCast(lua_State *f_vm)
{
    glm::vec3 l_start, l_end;
    ArgReader argStream(f_vm);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_start[i]);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_end[i]);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_hitNormal;
        Element *l_hitElement = nullptr;
        if(LuaManager::GetCore()->GetPhysicsManager()->RayCast(l_start, l_end, l_hitNormal, l_hitElement))
        {
            argStream.PushNumber(l_end.x);
            argStream.PushNumber(l_end.y);
            argStream.PushNumber(l_end.z);
            argStream.PushNumber(l_hitNormal.x);
            argStream.PushNumber(l_hitNormal.y);
            argStream.PushNumber(l_hitNormal.z);
            l_hitElement ? argStream.PushElement(reinterpret_cast<Element*>(l_hitElement)) : argStream.PushBoolean(false);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
