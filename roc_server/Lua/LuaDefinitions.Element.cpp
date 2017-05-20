#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/EventManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Elements/Element.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaArguments.h"
#include "Lua/LuaDefinitions.Element.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Lua
{

const std::vector<std::string> g_elementTypeNames
{
    "client", "file"
};

int isElement(lua_State *f_vm)
{
    Element *l_pointer;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_pointer);
    argStream.PushBoolean(!argStream.HasErrors());
    return argStream.GetReturnValue();
}

int elementGetType(lua_State *f_vm)
{
    Element *l_element;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_element);
    !argStream.HasErrors() ? argStream.PushText(g_elementTypeNames[l_element->GetElementType()]) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int elementSetData(lua_State *f_vm)
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

int elementGetData(lua_State *f_vm)
{
    Element *l_element;
    std::string l_key;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_element);
    argStream.ReadText(l_key);
    if(!argStream.HasErrors() && !l_key.empty())
    {
        CustomData l_data;
        l_element->GetCustomData(l_key,l_data);
        (l_data.GetType() != CustomData::DataType::None) ? argStream.PushCustomData(l_data) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int elementRemoveData(lua_State *f_vm)
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

}
}
