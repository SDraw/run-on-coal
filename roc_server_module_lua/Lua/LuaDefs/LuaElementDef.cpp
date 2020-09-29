#include "stdafx.h"

#include "Lua/LuaDefs/LuaElementDef.h"

#include "Lua/LuaArgReader.h"
#include "Module/LuaModule.h"
#include "Utils/LuaUtils.h"

void LuaElementDef::Init(lua_State *f_vm)
{
    lua_register(f_vm, "isElement", IsElement);
}

void LuaElementDef::AddHierarchyMethods(lua_State *f_vm)
{
    LuaUtils::AddClassMethod(f_vm, "getType", GetType);
    LuaUtils::AddClassMethod(f_vm, "destroy", Destroy);
}

int LuaElementDef::IsElement(lua_State *f_vm)
{
    // bool isElement(element element)
    ROC::IElement *l_element = nullptr;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadNextElement(l_element);
    l_argStream.PushBoolean(l_element != nullptr);
    return l_argStream.GetReturnValue();
}

int LuaElementDef::GetType(lua_State *f_vm)
{
    // str Element:getType()
    ROC::IElement *l_element;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_element);
    !l_argStream.HasErrors() ? l_argStream.PushText(l_element->GetElementTypeName()) : l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaElementDef::Destroy(lua_State *f_vm)
{
    // bool Element:destroy()
    ROC::IElement *l_element;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_element);
    if(!l_argStream.HasErrors())
    {
        bool l_result = LuaModule::GetModule()->GetEngineCore()->GetIElementManager()->DestroyIElement(l_element);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}
