#include "stdafx.h"

#include "Lua/LuaDefs/LuaAnimationDef.h"

#include "Lua/LuaArgReader.h"
#include "Lua/LuaDefs/LuaElementDef.h"
#include "Core/Core.h"
#include "Utils/LuaUtils.h"

void LuaAnimationDef::Init(lua_State *f_vm)
{
    LuaUtils::AddClass(f_vm, "Animation", Create);
    LuaUtils::AddClassMethod(f_vm, "getBonesCount", GetBonesCount);
    LuaUtils::AddClassMethod(f_vm, "getDuration", GetDuration);
    LuaElementDef::AddHierarchyMethods(f_vm);
    LuaUtils::AddClassFinish(f_vm);
}

int LuaAnimationDef::Create(lua_State *f_vm)
{
    // element Animation(str path)
    std::string l_path;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadText(l_path);
    if(!l_argStream.HasErrors() && !l_path.empty())
    {
        ROC::IAnimation *l_anim = Core::GetInstance()->GetEngineICore()->GetIElementManager()->CreateIAnimation(l_path);
        l_anim ? l_argStream.PushElement(l_anim) : l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaAnimationDef::GetBonesCount(lua_State *f_vm)
{
    // int Animation:getBonesCount()
    ROC::IAnimation *l_anim;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_anim);
    !l_argStream.HasErrors() ? l_argStream.PushInteger(l_anim->GetBonesCount()) : l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaAnimationDef::GetDuration(lua_State *f_vm)
{
    // int Animation:getDuration()
    ROC::IAnimation *l_anim;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_anim);
    !l_argStream.HasErrors() ? l_argStream.PushInteger(l_anim->GetDuration()) : l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}
