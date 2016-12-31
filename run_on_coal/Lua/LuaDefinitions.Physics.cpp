#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/PhysicsManager.h"
#include "Lua/LuaDefinitions.Physics.h"
#include "Model/Model.h"
#include "Lua/ArgReader.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Lua
{

int physicsSetEnabled(lua_State *f_vm)
{
    bool l_val;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadBoolean(l_val);
    if(!argStream.HasErrors())
    {
        LuaManager::m_corePointer->GetPhysicsManager()->SetPhysicsEnabled(l_val);
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int physicsGetEnabled(lua_State *f_vm)
{
    bool result = LuaManager::m_corePointer->GetPhysicsManager()->GetPhysicsEnabled();
    lua_pushboolean(f_vm, result);
    return 1;
}

int physicsSetFloorEnabled(lua_State *f_vm)
{
    bool l_val;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadBoolean(l_val);
    if(!argStream.HasErrors())
    {
        LuaManager::m_corePointer->GetPhysicsManager()->SetFloorEnabled(l_val);
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int physicsGetFloorEnabled(lua_State *f_vm)
{
    bool result = LuaManager::m_corePointer->GetPhysicsManager()->GetFloorEnabled();
    lua_pushboolean(f_vm, result);
    return 1;
}

int physicsSetGravity(lua_State *f_vm)
{
    glm::vec3 l_gravity;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_gravity[i]);
    if(!argStream.HasErrors())
    {
        LuaManager::m_corePointer->GetPhysicsManager()->SetGravity(l_gravity);
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int physicsGetGravity(lua_State *f_vm)
{
    glm::vec3 l_grav;
    LuaManager::m_corePointer->GetPhysicsManager()->GetGravity(l_grav);
    lua_pushnumber(f_vm, l_grav.x);
    lua_pushnumber(f_vm, l_grav.y);
    lua_pushnumber(f_vm, l_grav.z);
    return 3;
}

int physicsRayCast(lua_State *f_vm)
{
    glm::vec3 l_start, l_end;
    int l_returnVal = 1;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_start[i]);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_end[i]);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_hitNormal;
        void *l_hitElement = NULL;
        if(LuaManager::m_corePointer->GetPhysicsManager()->RayCast(l_start, l_end, l_hitNormal, &l_hitElement))
        {
            lua_pushnumber(f_vm, l_end.x);
            lua_pushnumber(f_vm, l_end.y);
            lua_pushnumber(f_vm, l_end.z);
            lua_pushnumber(f_vm, l_hitNormal.x);
            lua_pushnumber(f_vm, l_hitNormal.y);
            lua_pushnumber(f_vm, l_hitNormal.z);
            l_hitElement ? lua_pushlightuserdata(f_vm, l_hitElement) : lua_pushstring(f_vm, "floor");
            l_returnVal = 7;
        }
        else lua_pushboolean(f_vm, 0);
    }
    else lua_pushboolean(f_vm, 0);
    return l_returnVal;
}

}
}