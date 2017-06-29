#include "stdafx.h"

#include "Lua/LuaDefs/LuaElementDef.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/EventManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Elements/Element.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaArguments.h"
#include "Utils/Utils.h"

namespace ROC
{

extern const std::vector<std::string> g_ElementTypeNames;

}

void ROC::LuaElementDef::Init(lua_State *f_vm)
{
    lua_register(f_vm, "isElement", IsElement);
}
void ROC::LuaElementDef::AddHierarchyMethods(lua_State *f_vm)
{
    Utils::Lua::lua_registerClassMethod(f_vm, "getType", ElementGetType);
    Utils::Lua::lua_registerClassMethod(f_vm, "setData", ElementSetData);
    Utils::Lua::lua_registerClassMethod(f_vm, "getData", ElementGetData);
    Utils::Lua::lua_registerClassMethod(f_vm, "removeData", ElementRemoveData);
    Utils::Lua::lua_registerClassMethod(f_vm, "destroy", ElementDestroy);
}

int ROC::LuaElementDef::IsElement(lua_State *f_vm)
{
    Element *l_element = nullptr;
    ArgReader argStream(f_vm);
    argStream.ReadNextElement(l_element);
    argStream.PushBoolean(l_element != nullptr);
    return argStream.GetReturnValue();
}

int ROC::LuaElementDef::ElementGetType(lua_State *f_vm)
{
    Element *l_element;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_element);
    !argStream.HasErrors() ? argStream.PushText(g_ElementTypeNames[l_element->GetElementType()]) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaElementDef::ElementSetData(lua_State *f_vm)
{
    Element *l_element;
    std::string l_key;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_element);
    argStream.ReadText(l_key);
    if(!argStream.HasErrors() && !l_key.empty())
    {
        CustomData l_data;
        argStream.ReadCustomData(l_data);
        if(!argStream.HasErrors())
        {
            bool l_result = l_element->SetCustomData(l_key, l_data);
            argStream.PushBoolean(l_result);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaElementDef::ElementGetData(lua_State *f_vm)
{
    Element *l_element;
    std::string l_key;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_element);
    argStream.ReadText(l_key);
    if(!argStream.HasErrors() && !l_key.empty())
    {
        CustomData l_data;
        l_element->GetCustomData(l_key, l_data);
        (l_data.GetType() != CustomData::DataType::None) ? argStream.PushCustomData(l_data) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaElementDef::ElementRemoveData(lua_State *f_vm)
{
    Element *l_element;
    std::string l_key;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_element);
    argStream.ReadText(l_key);
    if(!argStream.HasErrors() && !l_key.empty())
    {
        bool l_result = l_element->RemoveCustomData(l_key);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaElementDef::ElementDestroy(lua_State *f_vm)
{
    Element *l_element;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_element);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::GetCore()->GetElementManager()->DestroyElement(l_element);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
