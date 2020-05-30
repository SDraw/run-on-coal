#include "stdafx.h"

#include "Lua/LuaDefs/LuaRenderTargetDef.h"

#include "Lua/ArgReader.h"
#include "Lua/LuaDefs/LuaElementDef.h"
#include "Lua/LuaDefs/LuaDrawableDef.h"
#include "Module/LuaModule.h"
#include "Utils/EnumUtils.h"
#include "Utils/LuaUtils.h"

const std::vector<std::string> g_RenderTargetTypes
{
    "shadow", "rgb", "rgba", "rgbf", "rgbaf"
};
const std::vector<std::string> g_RenderTargetProperties
{
    "clearColor", "depth", "color"
};
extern const std::vector<std::string> g_FilteringTypes; // Defined in LuaDrawableDef

void LuaRenderTargetDef::Init(lua_State *f_vm)
{
    LuaUtils::AddClass(f_vm, "RenderTarget", Create);
    LuaUtils::AddClassMethod(f_vm, "setProperty", SetProperty);
    LuaDrawableDef::AddHierarchyMethods(f_vm);
    LuaElementDef::AddHierarchyMethods(f_vm);
    LuaUtils::AddClassFinish(f_vm);
}

int LuaRenderTargetDef::Create(lua_State *f_vm)
{
    // element RenderTarget(int width, int height, str type [, str filtering = "nearest"])
    std::string l_type;
    glm::ivec2 l_size;
    std::string l_filtering("nearest");
    ArgReader argStream(f_vm);
    argStream.ReadText(l_type);
    for(int i = 0; i < 2; i++) argStream.ReadInteger(l_size[i]);
    argStream.ReadNextText(l_filtering);
    if(!argStream.HasErrors() && (l_size.x > 0) && (l_size.y > 0) && !l_type.empty())
    {
        size_t l_rtType = EnumUtils::ReadEnumVector(l_type, g_RenderTargetTypes);
        if(l_rtType != std::numeric_limits<size_t>::max())
        {
            size_t l_filteringType = EnumUtils::ReadEnumVector(l_filtering, g_FilteringTypes);
            ROC::IRenderTarget *l_rt = LuaModule::GetModule()->GetEngineCore()->GetIElementManager()->CreateIRenderTarget(static_cast<unsigned char>(l_rtType), l_size, static_cast<unsigned char>(l_filteringType));
            l_rt ? argStream.PushElement(l_rt) : argStream.PushBoolean(false);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaRenderTargetDef::SetProperty(lua_State *f_vm)
{
    ROC::IRenderTarget *l_rt;
    std::string l_property;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_rt);
    argStream.ReadText(l_property);
    if(!argStream.HasErrors() && !l_property.empty())
    {
        size_t l_propertyIndex = EnumUtils::ReadEnumVector(l_property, g_RenderTargetProperties);
        if(l_propertyIndex != std::numeric_limits<size_t>::max())
        {
            switch(l_propertyIndex)
            {
                case ROC::IRenderTarget::RTP_ClearColor:
                {
                    glm::vec4 l_color;
                    for(int i = 0; i < 4; i++) argStream.ReadNumber(l_color[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_rt->SetProperty(static_cast<ROC::IRenderTarget::RenderTargetProperty>(l_propertyIndex), &l_color);
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;

                case ROC::IRenderTarget::RTP_Depth:
                {
                    bool l_depth;
                    argStream.ReadBoolean(l_depth);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_rt->SetProperty(static_cast<ROC::IRenderTarget::RenderTargetProperty>(l_propertyIndex), &l_depth);
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;

                case ROC::IRenderTarget::RTP_Color:
                {
                    bool l_color;
                    argStream.ReadBoolean(l_color);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_rt->SetProperty(static_cast<ROC::IRenderTarget::RenderTargetProperty>(l_propertyIndex), &l_color);
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
            }
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
