#include "stdafx.h"

#include "Lua/LuaDefs/LuaRenderingDef.h"

#include "Lua/LuaArgReader.h"
#include "Module/LuaModule.h"
#include "Utils/EnumUtils.h"
#include "Utils/LuaUtils.h"

const std::vector<std::string> g_polygonFilling
{
    "point", "line", "fill"
};

void LuaRenderingDef::Init(lua_State *f_vm)
{
    lua_register(f_vm, "setViewport", SetViewport);
    lua_register(f_vm, "clearViewport", ClearViewport);
    lua_register(f_vm, "setClearColor", SetClearColor);
    lua_register(f_vm, "setPolygonMode", SetPolygonMode);
    lua_register(f_vm, "drawPhysics", DrawPhysics);
}

int LuaRenderingDef::ClearViewport(lua_State *f_vm)
{
    // bool clearViewport( [bool depth = true, bool color = true] )
    bool l_depth = true;
    bool l_color = true;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadNextBoolean(l_depth);
    l_argStream.ReadNextBoolean(l_color);
    if(!l_argStream.HasErrors())
    {
        bool l_result = LuaModule::GetModule()->GetEngineCore()->GetIRenderManager()->ClearViewport(l_depth, l_color);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaRenderingDef::SetClearColor(lua_State *f_vm)
{
    // bool setClearColor(float colorR, float colorG, float colorB, float colorA)
    glm::vec4 l_color;
    LuaArgReader l_argStream(f_vm);
    for(int i = 0; i < 4; i++) l_argStream.ReadNumber(l_color[i]);
    if(!l_argStream.HasErrors())
    {
        bool l_result = LuaModule::GetModule()->GetEngineCore()->GetIRenderManager()->SetClearColor(l_color);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaRenderingDef::SetViewport(lua_State *f_vm)
{
    // bool setViewport(int x, int y, int width, int height)
    glm::ivec4 l_area;
    LuaArgReader l_argStream(f_vm);
    for(int i = 0; i < 4; i++) l_argStream.ReadInteger(l_area[i]);
    if(!l_argStream.HasErrors())
    {
        bool l_result = LuaModule::GetModule()->GetEngineCore()->GetIRenderManager()->SetViewport(l_area);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaRenderingDef::SetPolygonMode(lua_State *f_vm)
{
    // bool setPolygonMode(str mode)
    std::string l_mode;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadText(l_mode);
    if(!l_argStream.HasErrors() && !l_mode.empty())
    {
        size_t l_type = EnumUtils::ReadEnumVector(l_mode, g_polygonFilling);
        if(l_type != std::numeric_limits<size_t>::max())
        {
            bool l_result = LuaModule::GetModule()->GetEngineCore()->GetIRenderManager()->SetPolygonMode(static_cast<int>(l_type));
            l_argStream.PushBoolean(l_result);
        }
        else l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaRenderingDef::DrawPhysics(lua_State *f_vm)
{
    // bool drawPhysics([float width = 1.0, str layer = "physics"])
    float l_width = 1.f;
    std::string l_layer("physics");
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadNextNumber(l_width);
    l_argStream.ReadNextText(l_layer);
    bool l_result = LuaModule::GetModule()->GetEngineCore()->GetIRenderManager()->DrawPhysics(l_width, l_layer);
    l_argStream.PushBoolean(l_result);
    return l_argStream.GetReturnValue();
}
