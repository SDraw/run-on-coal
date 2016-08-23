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
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadBoolean(l_val);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    LuaManager::m_corePointer->GetPhysicsManager()->SetPhysicsEnabled(l_val);
    lua_pushboolean(f_vm,1);
    return 1;
}
int physicsGetEnabled(lua_State *f_vm)
{
    bool result = LuaManager::m_corePointer->GetPhysicsManager()->GetPhysicsEnabled();
    lua_pushboolean(f_vm,result);
    return 1;
}

int physicsSetFloorEnabled(lua_State *f_vm)
{
    bool l_val;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadBoolean(l_val);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    LuaManager::m_corePointer->GetPhysicsManager()->SetFloorEnabled(l_val);
    lua_pushboolean(f_vm,1);
    return 1;
}
int physicsGetFloorEnabled(lua_State *f_vm)
{
    bool result = LuaManager::m_corePointer->GetPhysicsManager()->GetFloorEnabled();
    lua_pushboolean(f_vm,result);
    return 1;
}

int physicsSetGravity(lua_State *f_vm)
{
    lua_Number l_val[3];
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    for(int i = 0; i<3; i++) argStream.ReadNumber(l_val[i]);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec3 l_vec(l_val[0],l_val[1],l_val[2]);
    LuaManager::m_corePointer->GetPhysicsManager()->SetGravity(l_vec);
    lua_pushboolean(f_vm,1);
    return 1;
}
int physicsGetGravity(lua_State *f_vm)
{
    glm::vec3 l_grav;
    LuaManager::m_corePointer->GetPhysicsManager()->GetGravity(l_grav);
    lua_pushnumber(f_vm,l_grav.x);
    lua_pushnumber(f_vm,l_grav.y);
    lua_pushnumber(f_vm,l_grav.z);
    return 3;
}

int physicsRayCast(lua_State *f_vm)
{
    lua_Number l_val[6];
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    for(int i = 0; i<6; i++) argStream.ReadNumber(l_val[i]);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec3 l_rayStart(l_val[0],l_val[1],l_val[2]);
    glm::vec3 l_rayEnd(l_val[3],l_val[4],l_val[5]);
    glm::vec3 l_hitNormal;
    void *l_hitElement = NULL;
    if(!LuaManager::m_corePointer->GetPhysicsManager()->RayCast(l_rayStart,l_rayEnd,l_hitNormal,&l_hitElement))
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    lua_pushnumber(f_vm,l_rayEnd.x);
    lua_pushnumber(f_vm,l_rayEnd.y);
    lua_pushnumber(f_vm,l_rayEnd.z);
    lua_pushnumber(f_vm,l_hitNormal.x);
    lua_pushnumber(f_vm,l_hitNormal.y);
    lua_pushnumber(f_vm,l_hitNormal.z);
    if(l_hitElement!=NULL) lua_pushlightuserdata(f_vm,l_hitElement);
    else lua_pushstring(f_vm,"floor");
    return 7;
}

}
}
