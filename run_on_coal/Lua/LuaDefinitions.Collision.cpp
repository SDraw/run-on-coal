#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Collision.h"
#include "Scene/Collision.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Lua
{

int collisionCreate(lua_State *f_vm)
{
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    std::string l_typeString;
    lua_Number l_size[3] = {1.0,1.0,1.0};
    argStream.ReadText(l_typeString);
    for(int i = 0; i<3; i++) argStream.ReadNextNumber(l_size[i]);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    int l_type = Utils::ReadEnumString(l_typeString,"sphere,box,cylinder,capsule,cone");
    if(l_type==-1)
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec3 l_vSize(l_size[0],l_size[1],l_size[2]);
    Collision *l_col = LuaManager::m_corePointer->GetElementManager()->CreateCollision(COLLISION_TYPE_SPHERE+static_cast<unsigned char>(l_type),l_vSize);
    l_col ? lua_pushlightuserdata(f_vm,l_col) : lua_pushboolean(f_vm,0);
    return 1;
}
int collisionDestroy(lua_State *f_vm)
{
    Collision *l_col;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_col,ElementType::CollisionElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    bool l_result = LuaManager::m_corePointer->GetElementManager()->DestroyCollision(l_col);
    lua_pushboolean(f_vm,l_result);
    return 1;
}

int collisionSetPosition(lua_State *f_vm)
{
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    Collision *l_col;
    lua_Number l_val[3];
    argStream.ReadUserdata((void**)&l_col,ElementType::CollisionElement);
    for(int i = 0; i<3; i++) argStream.ReadNextNumber(l_val[i]);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec3 l_pos(l_val[0],l_val[1],l_val[2]);
    l_col->SetPosition(l_pos);
    lua_pushboolean(f_vm,1);
    return 1;
}
int collisionGetPosition(lua_State *f_vm)
{
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    Collision *l_col;
    argStream.ReadUserdata((void**)&l_col,ElementType::CollisionElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec3 l_pos;
    l_col->GetPosition(l_pos);
    lua_pushnumber(f_vm,l_pos.x);
    lua_pushnumber(f_vm,l_pos.y);
    lua_pushnumber(f_vm,l_pos.z);
    return 3;
}

int collisionSetRotation(lua_State *f_vm)
{
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    Collision *l_col;
    lua_Number l_val[4];
    l_val[3] = std::nan("0");
    argStream.ReadUserdata((void**)&l_col,ElementType::CollisionElement);
    for(int i = 0; i<3; i++) argStream.ReadNumber(l_val[i]);
    argStream.ReadNextNumber(l_val[3]);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::quat l_rot;
    if(std::isnan(l_val[3]))
    {
        glm::vec3 l_vRot(l_val[0],l_val[1],l_val[2]);
        l_rot = glm::quat(l_vRot);
    }
    else l_rot = glm::quat(static_cast<float>(l_val[3]),static_cast<float>(l_val[0]),static_cast<float>(l_val[1]),static_cast<float>(l_val[2]));
    l_col->SetRotation(l_rot);
    lua_pushboolean(f_vm,1);
    return 1;
}
int collisionGetRotation(lua_State *f_vm)
{
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    Collision *l_col;
    bool l_reqQuat = false;
    argStream.ReadUserdata((void**)&l_col,ElementType::CollisionElement);
    argStream.ReadNextBoolean(l_reqQuat);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    int l_returnVal = l_reqQuat ? 4 : 3;
    glm::quat l_rot;
    l_col->GetRotation(l_rot);
    if(l_reqQuat)
    {
        lua_pushnumber(f_vm,l_rot.x);
        lua_pushnumber(f_vm,l_rot.y);
        lua_pushnumber(f_vm,l_rot.z);
        lua_pushnumber(f_vm,l_rot.w);
    }
    else
    {
        glm::vec3 l_vRot = glm::eulerAngles(l_rot);
        lua_pushnumber(f_vm,l_vRot.x);
        lua_pushnumber(f_vm,l_vRot.y);
        lua_pushnumber(f_vm,l_vRot.z);
    }
    return l_returnVal;
}

}
}
