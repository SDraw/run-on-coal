#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
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
    "depth", "rgb", "rgba", "rgbf"
};

int rtCreate(lua_State *f_vm)
{
    glm::ivec2 l_size;
    unsigned int l_number;
    std::string l_type;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    for(int i = 0; i < 2; i++) argStream.ReadInteger(l_size[i]);
    argStream.ReadInteger(l_number);
    argStream.ReadText(l_type);
    if(!argStream.HasErrors() && (l_size.x >= 1) && (l_size.y >= 1) && !l_type.empty())
    {
        int l_etype = Utils::ReadEnumVector(g_targetTypesTable, l_type);
        if(l_etype != -1)
        {
            RenderTarget *l_rt = LuaManager::m_corePointer->GetElementManager()->CreateRenderTarget(l_number, l_size, RENDERTARGET_TYPE_DEPTH + l_etype);
            l_rt ? argStream.PushPointer(l_rt) : argStream.PushBoolean(false);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int rtDestroy(lua_State *f_vm)
{
    RenderTarget *l_rt;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_rt), ElementType::RenderTargetElement);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::m_corePointer->GetElementManager()->DestroyRenderTarget(l_rt);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int rtDraw(lua_State *f_vm)
{
    RenderTarget *l_rt;
    glm::vec2 l_pos, l_size;
    float l_rot;
    glm::vec4 l_color(1.f);
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_rt), ElementType::RenderTargetElement);
    for(int i = 0; i < 2; i++) argStream.ReadNumber(l_pos[i]);
    for(int i = 0; i < 2; i++) argStream.ReadNumber(l_size[i]);
    argStream.ReadNextNumber(l_rot);
    for(int i = 0; i < 4; i++) argStream.ReadNextNumber(l_color[i]);
    if(!argStream.HasErrors())
    {
        LuaManager::m_corePointer->GetRenderManager()->Render(l_rt, l_pos, l_size, l_rot, l_color);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

}
}
