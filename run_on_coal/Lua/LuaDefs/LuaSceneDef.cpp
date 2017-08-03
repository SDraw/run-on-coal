#include "stdafx.h"

#include "Lua/LuaDefs/LuaSceneDef.h"
#include "Lua/LuaDefs/LuaElementDef.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/InheritanceManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/LuaManager.h"
#include "Elements/Camera.h"
#include "Elements/Light.h"
#include "Elements/Scene.h"
#include "Lua/ArgReader.h"
#include "Utils/LuaUtils.h"

void ROC::LuaSceneDef::Init(lua_State *f_vm)
{
    LuaUtils::AddClass(f_vm, "Scene", Create);
    LuaUtils::AddClassMethod(f_vm, "setCamera", SetCamera);
    LuaUtils::AddClassMethod(f_vm, "getCamera", GetCamera);
    LuaUtils::AddClassMethod(f_vm, "removeCamera", RemoveCamera);
    LuaUtils::AddClassMethod(f_vm, "setLight", SetLight);
    LuaUtils::AddClassMethod(f_vm, "getLight", GetLight);
    LuaUtils::AddClassMethod(f_vm, "removeLight", RemoveLight);
    LuaElementDef::AddHierarchyMethods(f_vm);
    LuaUtils::AddClassFinish(f_vm);
}

int ROC::LuaSceneDef::Create(lua_State *f_vm)
{
    ArgReader argStream(f_vm);
    Scene *l_scene = LuaManager::GetCore()->GetElementManager()->CreateScene();
    l_scene ? argStream.PushElement(l_scene) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaSceneDef::SetCamera(lua_State *f_vm)
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
int ROC::LuaSceneDef::GetCamera(lua_State *f_vm)
{
    Scene *l_scene;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_scene);
    if(!argStream.HasErrors())
    {
        Camera *l_camera = l_scene->GetCamera();
        l_camera ? argStream.PushElement(l_camera) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaSceneDef::RemoveCamera(lua_State *f_vm)
{
    Scene *l_scene;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_scene);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::GetCore()->GetInheritManager()->RemoveSceneCamera(l_scene);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaSceneDef::SetLight(lua_State *f_vm)
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
int ROC::LuaSceneDef::GetLight(lua_State *f_vm)
{
    Scene *l_scene;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_scene);
    if(!argStream.HasErrors())
    {
        Light *l_light = l_scene->GetLight();
        l_light ? argStream.PushElement(l_light) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaSceneDef::RemoveLight(lua_State *f_vm)
{
    Scene *l_scene;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_scene);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::GetCore()->GetInheritManager()->RemoveSceneLight(l_scene);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
