#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/InheritanceManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/LuaManager.h"
#include "Elements/Camera.h"
#include "Elements/Light.h"
#include "Elements/Scene.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Scene.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Lua
{

int sceneCreate(lua_State *f_vm)
{
    ArgReader argStream(f_vm);
    Scene *l_scene = LuaManager::GetCore()->GetElementManager()->CreateScene();
    l_scene ? argStream.PushPointer(l_scene) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int sceneSetCamera(lua_State *f_vm)
{
    Scene *l_scene;
    Camera *l_camera;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_scene);
    argStream.ReadElement(l_camera);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::GetCore()->GetInheritManager()->SetSceneCamera(l_scene, l_camera);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int sceneGetCamera(lua_State *f_vm)
{
    Scene *l_scene;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_scene);
    if(!argStream.HasErrors())
    {
        Camera *l_camera = l_scene->GetCamera();
        l_camera ? argStream.PushPointer(l_camera) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int sceneSetLight(lua_State *f_vm)
{
    Scene *l_scene;
    Light *l_light;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_scene);
    argStream.ReadElement(l_light);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::GetCore()->GetInheritManager()->SetSceneLight(l_scene, l_light);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int sceneGetLight(lua_State *f_vm)
{
    Scene *l_scene;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_scene);
    if(!argStream.HasErrors())
    {
        Light *l_light = l_scene->GetLight();
        l_light ? argStream.PushPointer(l_light) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

}
}
