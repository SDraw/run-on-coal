#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Elements/Collision.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Collision.h"
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
            l_col ? argStream.PushPointer(l_col) : argStream.PushBoolean(false);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int collisionDestroy(lua_State *f_vm)
{
    Collision *l_col;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_col), ElementType::CollisionElement);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::m_corePointer->GetElementManager()->DestroyCollision(l_col);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
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
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int collisionGetPosition(lua_State *f_vm)
{
    Collision *l_col;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_col), ElementType::CollisionElement);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_pos;
        l_col->GetPosition(l_pos);
        argStream.PushNumber(l_pos.x);
        argStream.PushNumber(l_pos.y);
        argStream.PushNumber(l_pos.z);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
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
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int collisionGetRotation(lua_State *f_vm)
{
    Collision *l_col;
    bool l_reqQuat = false;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_col), ElementType::CollisionElement);
    argStream.ReadNextBoolean(l_reqQuat);
    if(!argStream.HasErrors())
    {
        glm::quat l_rot;
        l_col->GetRotation(l_rot);
        if(l_reqQuat)
        {
            argStream.PushNumber(l_rot.x);
            argStream.PushNumber(l_rot.y);
            argStream.PushNumber(l_rot.z);
            argStream.PushNumber(l_rot.w);
        }
        else
        {
            glm::vec3 l_vRot = glm::eulerAngles(l_rot);
            argStream.PushNumber(l_vRot.x);
            argStream.PushNumber(l_vRot.y);
            argStream.PushNumber(l_vRot.z);
        }
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

}
}
