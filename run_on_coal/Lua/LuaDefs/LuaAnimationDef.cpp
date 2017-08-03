#include "stdafx.h"

#include "Lua/LuaDefs/LuaAnimationDef.h"
#include "Lua/LuaDefs/LuaElementDef.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Elements/Animation/Animation.h"
#include "Lua/ArgReader.h"
#include "Utils/LuaUtils.h"

void ROC::LuaAnimationDef::Init(lua_State *f_vm)
{
    LuaUtils::AddClass(f_vm, "Animation", Create);
    LuaUtils::AddClassMethod(f_vm, "getBonesCount", GetBonesCount);
    LuaUtils::AddClassMethod(f_vm, "getDuration", GetDuration);
    LuaElementDef::AddHierarchyMethods(f_vm);
    LuaUtils::AddClassFinish(f_vm);
}

int ROC::LuaAnimationDef::Create(lua_State *f_vm)
{
    std::string l_path;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_path);
    if(!argStream.HasErrors() && !l_path.empty())
    {
        Animation *l_anim = ROC::LuaManager::GetCore()->GetElementManager()->CreateAnimation(l_path);
        l_anim ? argStream.PushElement(l_anim) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaAnimationDef::GetBonesCount(lua_State *f_vm)
{
    Animation *l_anim;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_anim);
    !argStream.HasErrors() ? argStream.PushNumber(l_anim->GetBonesCount()) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaAnimationDef::GetDuration(lua_State *f_vm)
{
    Animation *l_anim;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_anim);
    !argStream.HasErrors() ? argStream.PushNumber(l_anim->GetDuration()) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
