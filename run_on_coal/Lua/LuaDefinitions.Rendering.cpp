#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/RenderManager/RenderManager.h"
#include "Elements/RenderTarget.h"
#include "Elements/Scene.h"
#include "Elements/Shader.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Rendering.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Lua
{

const std::vector<std::string> g_buffersTable
{
    "color", "depth"
};
const std::vector<std::string> g_polygonFillTable
{
    "point", "line", "fill"
};

int setActiveScene(lua_State *f_vm)
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
int setActiveShader(lua_State *f_vm)
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
int setRenderTarget(lua_State *f_vm)
{
    RenderTarget *l_rt = nullptr;
    ArgReader argStream(f_vm);
    argStream.ReadNextElement(l_rt);
    LuaManager::GetCore()->GetRenderManager()->SetRenderTarget(l_rt);
    argStream.PushBoolean(true);
    return argStream.GetReturnValue();
}

int clearRenderArea(lua_State *f_vm)
{
    std::string l_param;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_param);
    if(!argStream.HasErrors() && !l_param.empty())
    {
        int l_type = Utils::Enum::ReadEnumVector(g_buffersTable, l_param);
        if(l_type != -1)
        {
            GLbitfield l_buffer = (l_type == 0) ? GL_COLOR_BUFFER_BIT : GL_DEPTH_BUFFER_BIT;
            LuaManager::GetCore()->GetRenderManager()->ClearRenderArea(l_buffer);
            argStream.PushBoolean(true);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int setClearColor(lua_State *f_vm)
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
int setRenderArea(lua_State *f_vm)
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
int setPolygonMode(lua_State *f_vm)
{
    std::string l_mode;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_mode);
    if(!argStream.HasErrors() && !l_mode.empty())
    {
        int l_type = Utils::Enum::ReadEnumVector(g_polygonFillTable, l_mode);
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

}
}
