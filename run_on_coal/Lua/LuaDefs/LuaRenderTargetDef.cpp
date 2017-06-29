#include "stdafx.h"

#include "Lua/LuaDefs/LuaRenderTargetDef.h"
#include "Lua/LuaDefs/LuaDrawableDef.h"
#include "Lua/LuaDefs/LuaElementDef.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Elements/RenderTarget.h"
#include "Lua/ArgReader.h"
#include "Utils/Utils.h"

namespace ROC
{

const std::vector<std::string> g_RenderTargetTypesTable
{
    "depth", "rgb", "rgba", "rgbf", "rgbaf"
};
extern const std::vector<std::string> g_FilteringTypesTable;

}

void ROC::LuaRenderTargetDef::Init(lua_State *f_vm)
{
    Utils::Lua::lua_registerClass(f_vm, "RenderTarget", RenderTargetCreate);
    LuaDrawableDef::AddHierarchyMethods(f_vm);
    LuaElementDef::AddHierarchyMethods(f_vm);
    Utils::Lua::lua_registerClassFinish(f_vm);
}

int ROC::LuaRenderTargetDef::RenderTargetCreate(lua_State *f_vm)
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
        int l_etype = Utils::Enum::ReadEnumVector(g_RenderTargetTypesTable, l_type);
        if(l_etype != -1)
        {
            int l_filteringType = Utils::Enum::ReadEnumVector(g_FilteringTypesTable, l_filtering);
            if(l_filteringType == -1) l_filteringType = 0;
            RenderTarget *l_rt = LuaManager::GetCore()->GetElementManager()->CreateRenderTarget(l_number, l_size, l_etype, l_filteringType);
            l_rt ? argStream.PushElement(l_rt) : argStream.PushBoolean(false);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
