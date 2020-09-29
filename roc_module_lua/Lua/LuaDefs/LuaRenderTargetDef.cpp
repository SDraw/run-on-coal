#include "stdafx.h"

#include "Lua/LuaDefs/LuaRenderTargetDef.h"

#include "Lua/LuaArgReader.h"
#include "Lua/LuaDefs/LuaElementDef.h"
#include "Lua/LuaDefs/LuaDrawableDef.h"
#include "Module/LuaModule.h"
#include "Utils/EnumUtils.h"
#include "Utils/LuaUtils.h"

const std::vector<std::string> g_renderTargetTypes
{
    "shadow", "rgb", "rgba", "rgbf", "rgbaf"
};
const std::vector<std::string> g_renderTargetProperties
{
    "clearColor", "depth", "color"
};
extern const std::vector<std::string> g_filteringTypes; // Defined in LuaDrawableDef

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
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadText(l_type);
    for(int i = 0; i < 2; i++) l_argStream.ReadInteger(l_size[i]);
    l_argStream.ReadNextText(l_filtering);
    if(!l_argStream.HasErrors() && (l_size.x > 0) && (l_size.y > 0) && !l_type.empty())
    {
        size_t l_rtType = EnumUtils::ReadEnumVector(l_type, g_renderTargetTypes);
        if(l_rtType != std::numeric_limits<size_t>::max())
        {
            size_t l_filteringType = EnumUtils::ReadEnumVector(l_filtering, g_filteringTypes);
            ROC::IRenderTarget *l_rt = LuaModule::GetModule()->GetEngineCore()->GetIElementManager()->CreateIRenderTarget(static_cast<unsigned char>(l_rtType), l_size, static_cast<unsigned char>(l_filteringType));
            l_rt ? l_argStream.PushElement(l_rt) : l_argStream.PushBoolean(false);
        }
        else l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaRenderTargetDef::SetProperty(lua_State *f_vm)
{
    ROC::IRenderTarget *l_rt;
    std::string l_property;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_rt);
    l_argStream.ReadText(l_property);
    if(!l_argStream.HasErrors() && !l_property.empty())
    {
        size_t l_propertyIndex = EnumUtils::ReadEnumVector(l_property, g_renderTargetProperties);
        if(l_propertyIndex != std::numeric_limits<size_t>::max())
        {
            switch(l_propertyIndex)
            {
                case ROC::IRenderTarget::RTP_ClearColor:
                {
                    glm::vec4 l_color;
                    for(int i = 0; i < 4; i++) l_argStream.ReadNumber(l_color[i]);
                    if(!l_argStream.HasErrors())
                    {
                        bool l_result = l_rt->SetProperty(static_cast<ROC::IRenderTarget::RenderTargetProperty>(l_propertyIndex), &l_color);
                        l_argStream.PushBoolean(l_result);
                    }
                    else l_argStream.PushBoolean(false);
                } break;

                case ROC::IRenderTarget::RTP_Depth:
                {
                    bool l_depth;
                    l_argStream.ReadBoolean(l_depth);
                    if(!l_argStream.HasErrors())
                    {
                        bool l_result = l_rt->SetProperty(static_cast<ROC::IRenderTarget::RenderTargetProperty>(l_propertyIndex), &l_depth);
                        l_argStream.PushBoolean(l_result);
                    }
                    else l_argStream.PushBoolean(false);
                } break;

                case ROC::IRenderTarget::RTP_Color:
                {
                    bool l_color;
                    l_argStream.ReadBoolean(l_color);
                    if(!l_argStream.HasErrors())
                    {
                        bool l_result = l_rt->SetProperty(static_cast<ROC::IRenderTarget::RenderTargetProperty>(l_propertyIndex), &l_color);
                        l_argStream.PushBoolean(l_result);
                    }
                    else l_argStream.PushBoolean(false);
                } break;
            }
        }
        else l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}
