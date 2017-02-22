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

namespace ROC
{
namespace Lua
{

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
    RenderTarget *l_rt = NULL;
    ArgReader argStream(f_vm);
    argStream.ReadNextElement(l_rt);
    LuaManager::GetCore()->GetRenderManager()->SetRenderTarget(l_rt);
    argStream.PushBoolean(true);
    return argStream.GetReturnValue();
}

}
}
