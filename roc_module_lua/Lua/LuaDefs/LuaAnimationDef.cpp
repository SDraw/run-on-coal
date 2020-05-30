#include "stdafx.h"

#include "Lua/LuaDefs/LuaAnimationDef.h"

#include "Lua/ArgReader.h"
#include "Lua/LuaDefs/LuaElementDef.h"
#include "Module/LuaModule.h"
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
    ArgReader argStream(f_vm);
    argStream.ReadText(l_path);
    if(!argStream.HasErrors() && !l_path.empty())
    {
        ROC::IAnimation *l_anim = LuaModule::GetModule()->GetEngineCore()->GetIElementManager()->CreateIAnimation(l_path);
        l_anim ? argStream.PushElement(l_anim) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaAnimationDef::GetBonesCount(lua_State *f_vm)
{
    // int Animation:getBonesCount()
    ROC::IAnimation *l_anim;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_anim);
    !argStream.HasErrors() ? argStream.PushInteger(l_anim->GetBonesCount()) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaAnimationDef::GetDuration(lua_State *f_vm)
{
    // int Animation:getDuration()
    ROC::IAnimation *l_anim;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_anim);
    !argStream.HasErrors() ? argStream.PushNumber(l_anim->GetDuration()) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
