#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/InheritanceManager.h"
#include "Managers/LuaManager.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Scene.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Lua 
{

int sceneCreate(lua_State *f_vm)
{
    Scene *l_scene = LuaManager::m_corePointer->GetElementManager()->CreateScene();
    l_scene ? lua_pushlightuserdata(f_vm,l_scene) : lua_pushboolean(f_vm,0);
    return 1;
}
int sceneDestroy(lua_State *f_vm)
{
    Scene *l_scene = NULL;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_scene,ElementType::SceneElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    bool result = LuaManager::m_corePointer->GetElementManager()->DestroyScene(l_scene);
    lua_pushboolean(f_vm,result);
    return 1;
}
int sceneSetCamera(lua_State *f_vm)
{
    Scene *l_scene;
    Camera *l_camera;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_scene,ElementType::SceneElement);
    argStream.ReadUserdata((void**)&l_camera,ElementType::CameraElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    bool result = LuaManager::m_corePointer->GetInheritManager()->SetSceneCamera(l_scene,l_camera);
    lua_pushboolean(f_vm,result);
    return 1;
}
int sceneSetLight(lua_State *f_vm)
{
    Scene *l_scene;
    Light *l_light;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_scene,ElementType::SceneElement);
    argStream.ReadUserdata((void**)&l_light,ElementType::LightElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    bool result = LuaManager::m_corePointer->GetInheritManager()->SetSceneLight(l_scene,l_light);
    lua_pushboolean(f_vm,result);
    return 1;
}

}
}
