#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/RenderManager.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Rendering.h"

namespace ROC
{
namespace Lua 
{

int setActiveScene(lua_State *f_vm)
{
    Scene *l_scene;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_scene,ElementType::SceneElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    LuaManager::m_corePointer->GetRenderManager()->SetCurrentScene(l_scene);
    lua_pushboolean(f_vm,1);
    return 1;
}
int setActiveShader(lua_State *f_vm)
{
    Shader *l_shader;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_shader,ElementType::ShaderElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    LuaManager::m_corePointer->GetRenderManager()->SetCurrentShader(l_shader);
    lua_pushboolean(f_vm,1);
    return 1;
}
int setRenderTarget(lua_State *f_vm)
{
    RenderTarget *l_rt = NULL;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadNextUserdata((void**)&l_rt,ElementType::RenderTargetElement);
    LuaManager::m_corePointer->GetRenderManager()->SetRenderTarget(l_rt);
    lua_pushboolean(f_vm,1);
    return 1;
}

}
}
