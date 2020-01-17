#include "stdafx.h"

#include "Lua/LuaDefs/LuaElementDef.h"

#include "Lua/ArgReader.h"
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
    ArgReader argStream(f_vm);
    argStream.ReadNextElement(l_element);
    argStream.PushBoolean(l_element != nullptr);
    return argStream.GetReturnValue();
}

int LuaElementDef::GetType(lua_State *f_vm)
{
    // str Element:getType()
    ROC::IElement *l_element;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_element);
    !argStream.HasErrors() ? argStream.PushText(l_element->GetElementTypeName()) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaElementDef::Destroy(lua_State *f_vm)
{
    // bool Element:destroy()
    ROC::IElement *l_element;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_element);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaModule::GetModule()->GetEngineCore()->GetIElementManager()->DestroyIElement(l_element);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
