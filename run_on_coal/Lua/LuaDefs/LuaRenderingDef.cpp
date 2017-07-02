#include "stdafx.h"

#include "Lua/LuaDefs/LuaRenderingDef.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/RenderManager/RenderManager.h"
#include "Elements/RenderTarget.h"
#include "Elements/Scene.h"
#include "Elements/Shader.h"
#include "Lua/ArgReader.h"
#include "Utils/Utils.h"

namespace ROC
{

const std::vector<std::string> g_PolygonFillTable
{
    "point", "line", "fill"
};

}

void ROC::LuaRenderingDef::Init(lua_State *f_vm)
{
    lua_register(f_vm, "setActiveScene", SetActiveScene);
    lua_register(f_vm, "setActiveShader", SetActiveShader);
    lua_register(f_vm, "setRenderTarget", SetRenderTarget);
    lua_register(f_vm, "clearRenderArea", ClearRenderArea);
    lua_register(f_vm, "setClearColor", SetClearColor);
    lua_register(f_vm, "setRenderArea", SetRenderArea);
    lua_register(f_vm, "setPolygonMode", SetPolygonMode);
}

int ROC::LuaRenderingDef::SetActiveScene(lua_State *f_vm)
{
    Scene *l_scene;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_scene);
    if(!argStream.HasErrors())
    {
        LuaManager::GetCore()->GetRenderManager()->SetActiveScene(l_scene);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaRenderingDef::SetActiveShader(lua_State *f_vm)
{
    Shader *l_shader;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_shader);
    if(!argStream.HasErrors())
    {
        LuaManager::GetCore()->GetRenderManager()->SetActiveShader(l_shader);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaRenderingDef::SetRenderTarget(lua_State *f_vm)
{
    RenderTarget *l_rt = nullptr;
    ArgReader argStream(f_vm);
    argStream.ReadNextElement(l_rt);
    LuaManager::GetCore()->GetRenderManager()->SetRenderTarget(l_rt);
    argStream.PushBoolean(true);
    return argStream.GetReturnValue();
}

int ROC::LuaRenderingDef::ClearRenderArea(lua_State *f_vm)
{
    bool l_depth = true;
    bool l_color = true;
    ArgReader argStream(f_vm);
    argStream.ReadNextBoolean(l_depth);
    argStream.ReadNextBoolean(l_color);
    if(!argStream.HasErrors())
    {
        LuaManager::GetCore()->GetRenderManager()->ClearRenderArea(l_depth, l_color);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaRenderingDef::SetClearColor(lua_State *f_vm)
{
    glm::vec4 l_color;
    ArgReader argStream(f_vm);
    for(int i = 0; i < 4; i++) argStream.ReadNumber(l_color[i]);
    if(!argStream.HasErrors())
    {
        RenderManager::SetClearColour(l_color);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaRenderingDef::SetRenderArea(lua_State *f_vm)
{
    glm::ivec4 l_area;
    ArgReader argStream(f_vm);
    for(int i = 0; i < 4; i++) argStream.ReadInteger(l_area[i]);
    if(!argStream.HasErrors())
    {
        LuaManager::GetCore()->GetRenderManager()->SetViewport(l_area);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaRenderingDef::SetPolygonMode(lua_State *f_vm)
{
    std::string l_mode;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_mode);
    if(!argStream.HasErrors() && !l_mode.empty())
    {
        int l_type = Utils::Enum::ReadEnumVector(g_PolygonFillTable, l_mode);
        if(l_type != -1)
        {
            RenderManager::SetPolygonMode(l_type);
            argStream.PushBoolean(true);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
