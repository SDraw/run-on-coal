#include "stdafx.h"

#include "Lua/LuaDefs/LuaElementDef.h"

#include "Module/LuaModule.h"
#include "Lua/ArgReader.h"
#include "Utils/CustomArgument.h"
#include "Utils/LuaUtils.h"

void LuaElementDef::Init(lua_State *f_vm)
{
    lua_register(f_vm, "isElement", IsElement);
}
void LuaElementDef::AddHierarchyMethods(lua_State *f_vm)
{
    LuaUtils::AddClassMethod(f_vm, "getType", GetType);
    LuaUtils::AddClassMethod(f_vm, "setData", SetData);
    LuaUtils::AddClassMethod(f_vm, "getData", GetData);
    LuaUtils::AddClassMethod(f_vm, "removeData", RemoveData);
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

int LuaElementDef::SetData(lua_State *f_vm)
{
    // bool Element:setData(str key, var value)
    ROC::IElement *l_element;
    std::string l_key;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_element);
    argStream.ReadText(l_key);
    if(!argStream.HasErrors() && !l_key.empty())
    {
        CustomArgument l_data;
        argStream.ReadArgument(l_data);
        if(!argStream.HasErrors())
        {
            bool l_result = l_element->SetData(l_key, l_data);
            argStream.PushBoolean(l_result);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaElementDef::GetData(lua_State *f_vm)
{
    // var Element:getData(str key)
    ROC::IElement *l_element;
    std::string l_key;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_element);
    argStream.ReadText(l_key);
    if(!argStream.HasErrors() && !l_key.empty())
    {
        CustomArgument l_data;
        l_element->GetData(l_key, l_data) ? argStream.PushArgument(l_data) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaElementDef::RemoveData(lua_State *f_vm)
{
    // bool Element:removeData(str key)
    ROC::IElement *l_element;
    std::string l_key;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_element);
    argStream.ReadText(l_key);
    if(!argStream.HasErrors() && !l_key.empty())
    {
        bool l_result = l_element->RemoveData(l_key);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
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
        bool l_result = LuaModule::GetModule()->GetEngineCore()->GetElementManager()->DestroyElement(l_element);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
