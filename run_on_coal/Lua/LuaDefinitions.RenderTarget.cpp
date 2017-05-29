#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/RenderManager/RenderManager.h"
#include "Elements/RenderTarget.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.RenderTarget.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Lua
{

const std::vector<std::string> g_targetTypesTable
{
    "depth", "rgb", "rgba", "rgbf", "rgbaf"
};
const std::vector<std::string> g_rtFilteringTypesTable
{
    "nearest", "linear"
};

int rtCreate(lua_State *f_vm)
{
    glm::ivec2 l_size;
    unsigned int l_number;
    std::string l_type, l_filtering;
    ArgReader argStream(f_vm);
    for(int i = 0; i < 2; i++) argStream.ReadInteger(l_size[i]);
    argStream.ReadInteger(l_number);
    argStream.ReadText(l_type);
    argStream.ReadNextText(l_filtering);
    if(!argStream.HasErrors() && (l_size.x >= 1) && (l_size.y >= 1) && !l_type.empty())
    {
        int l_etype = Utils::ReadEnumVector(g_targetTypesTable, l_type);
        if(l_etype != -1)
        {
            int l_filteringType = Utils::ReadEnumVector(g_rtFilteringTypesTable, l_filtering);
            if(l_filteringType == -1) l_filteringType = 0;
            RenderTarget *l_rt = LuaManager::GetCore()->GetElementManager()->CreateRenderTarget(l_number, l_size, l_etype, l_filteringType);
            l_rt ? argStream.PushPointer(l_rt) : argStream.PushBoolean(false);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int rtGetSize(lua_State *f_vm)
{
    RenderTarget *l_rt;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_rt);
    if(!argStream.HasErrors())
    {
        glm::ivec2 l_size;
        l_rt->GetSize(l_size);
        argStream.PushInteger(l_size.x);
        argStream.PushInteger(l_size.y);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int rtGetFiltering(lua_State *f_vm)
{
    RenderTarget *l_rt;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_rt);
    !argStream.HasErrors() ? argStream.PushText(g_rtFilteringTypesTable[l_rt->GetFiltering()]) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

}
}
