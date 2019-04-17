#include "stdafx.h"

#include "Lua/LuaDefs/LuaRenderTargetDef.h"
#include "Lua/LuaDefs/LuaDrawableDef.h"
#include "Lua/LuaDefs/LuaElementDef.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager/LuaManager.h"
#include "Elements/RenderTarget.h"
#include "Lua/ArgReader.h"
#include "Utils/EnumUtils.h"
#include "Utils/LuaUtils.h"

namespace ROC
{

const std::vector<std::string> g_RenderTargetTypesTable
{
    "shadow", "rgb", "rgba", "rgbf", "rgbaf"
};
extern const std::vector<std::string> g_FilteringTypesTable;

}

void ROC::LuaRenderTargetDef::Init(lua_State *f_vm)
{
    LuaUtils::AddClass(f_vm, "RenderTarget", Create);
    LuaDrawableDef::AddHierarchyMethods(f_vm);
    LuaElementDef::AddHierarchyMethods(f_vm);
    LuaUtils::AddClassFinish(f_vm);
}

int ROC::LuaRenderTargetDef::Create(lua_State *f_vm)
{
    // element RenderTarget(int width, int height, str type [, str filtering = "nearest"])
    std::string l_type;
    glm::ivec2 l_size;
    std::string l_filtering;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_type);
    for(int i = 0; i < 2; i++) argStream.ReadInteger(l_size[i]);
    argStream.ReadNextText(l_filtering);
    if(!argStream.HasErrors() && (l_size.x > 0) && (l_size.y > 0) && !l_type.empty())
    {
        int l_rtType = EnumUtils::ReadEnumVector(l_type, g_RenderTargetTypesTable);
        if(l_rtType != -1)
        {
            int l_filteringType = EnumUtils::ReadEnumVector(l_filtering, g_FilteringTypesTable);
            RenderTarget *l_rt = Core::GetCore()->GetElementManager()->CreateRenderTarget(l_rtType, l_size, l_filteringType);
            l_rt ? argStream.PushElement(l_rt) : argStream.PushBoolean(false);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
