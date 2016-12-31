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

const std::vector<std::string> g_collisionTypesTable
{
    "sphere", "box", "cylinder", "capsule", "cone"
};

int collisionCreate(lua_State *f_vm)
{
    std::string l_typeString;
    glm::vec3 l_size(1.f);
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadText(l_typeString);
    for(int i = 0; i < 3; i++) argStream.ReadNextNumber(l_size[i]);
    if(!argStream.HasErrors() && !l_typeString.empty())
    {
        int l_type = Utils::ReadEnumVector(g_collisionTypesTable, l_typeString);
        if(l_type != -1)
        {
            Collision *l_col = LuaManager::m_corePointer->GetElementManager()->CreateCollision(COLLISION_TYPE_SPHERE + static_cast<unsigned char>(l_type), l_size);
            l_col ? lua_pushlightuserdata(f_vm, l_col) : lua_pushboolean(f_vm, 0);
        }
        else lua_pushboolean(f_vm, 0);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int collisionDestroy(lua_State *f_vm)
{
    Collision *l_col;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_col), ElementType::CollisionElement);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::m_corePointer->GetElementManager()->DestroyCollision(l_col);
        lua_pushboolean(f_vm, l_result);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}

int collisionSetPosition(lua_State *f_vm)
{
    Collision *l_col;
    glm::vec3 l_pos;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_col), ElementType::CollisionElement);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_pos[i]);
    if(!argStream.HasErrors())
    {
        l_col->SetPosition(l_pos);
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int collisionGetPosition(lua_State *f_vm)
{
    Collision *l_col;
    int l_returnVal = 1;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_col), ElementType::CollisionElement);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_pos;
        l_col->GetPosition(l_pos);
        lua_pushnumber(f_vm, l_pos.x);
        lua_pushnumber(f_vm, l_pos.y);
        lua_pushnumber(f_vm, l_pos.z);
        l_returnVal = 3;
    }
    else lua_pushboolean(f_vm, 0);
    return l_returnVal;
}

int collisionSetRotation(lua_State *f_vm)
{
    Collision *l_col;
    glm::vec4 l_val(0.f, 0.f, 0.f, std::nanf("0"));
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_col), ElementType::CollisionElement);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_val[i]);
    argStream.ReadNextNumber(l_val.w);
    if(!argStream.HasErrors())
    {
        glm::quat l_rot = std::isnan(l_val.w) ? glm::quat(l_val) : glm::quat(l_val.w, l_val.x, l_val.y, l_val.z);
        l_col->SetRotation(l_rot);
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int collisionGetRotation(lua_State *f_vm)
{
    Collision *l_col;
    bool l_reqQuat = false;
    int l_returnVal = 1;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_col), ElementType::CollisionElement);
    argStream.ReadNextBoolean(l_reqQuat);
    if(!argStream.HasErrors())
    {
        glm::quat l_rot;
        l_col->GetRotation(l_rot);
        if(l_reqQuat)
        {
            lua_pushnumber(f_vm, l_rot.x);
            lua_pushnumber(f_vm, l_rot.y);
            lua_pushnumber(f_vm, l_rot.z);
            lua_pushnumber(f_vm, l_rot.w);
            l_returnVal = 4;
        }
        else
        {
            glm::vec3 l_vRot = glm::eulerAngles(l_rot);
            lua_pushnumber(f_vm, l_vRot.x);
            lua_pushnumber(f_vm, l_vRot.y);
            lua_pushnumber(f_vm, l_vRot.z);
            l_returnVal = 3;
        }
    }
    else lua_pushboolean(f_vm, 0);
    return l_returnVal;
}

}
}