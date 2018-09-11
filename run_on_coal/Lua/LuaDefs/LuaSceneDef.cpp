#include "stdafx.h"

#include "Lua/LuaDefs/LuaSceneDef.h"
#include "Lua/LuaDefs/LuaElementDef.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/InheritanceManager.h"
#include "Managers/LuaManager.h"
#include "Managers/RenderManager/RenderManager.h"
#include "Elements/Camera.h"
#include "Elements/Light.h"
#include "Elements/RenderTarget.h"
#include "Elements/Scene.h"
#include "Elements/Shader/Shader.h"
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
    LuaUtils::AddClassMethod(f_vm, "setRenderTarget", SetRenderTarget);
    LuaUtils::AddClassMethod(f_vm, "getRenderTarget", GetRenderTarget);
    LuaUtils::AddClassMethod(f_vm, "removeRenderTarget", RemoveRenderTarget);
    LuaUtils::AddClassMethod(f_vm, "setShader", SetShader);
    LuaUtils::AddClassMethod(f_vm, "getShader", GetShader);
    LuaUtils::AddClassMethod(f_vm, "removeShader", RemoveShader);
    LuaUtils::AddClassMethod(f_vm, "setSkyGradient", SetSkyGradient);
    LuaUtils::AddClassMethod(f_vm, "getSkyGradient", GetSkyGradient);
    LuaUtils::AddClassMethod(f_vm, "setActive", SetActive);
    LuaElementDef::AddHierarchyMethods(f_vm);
    LuaUtils::AddClassFinish(f_vm);
}

int ROC::LuaSceneDef::Create(lua_State *f_vm)
{
    // element Scene()
    ArgReader argStream(f_vm);
    Scene *l_scene = Core::GetCore()->GetElementManager()->CreateScene();
    l_scene ? argStream.PushElement(l_scene) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaSceneDef::SetCamera(lua_State *f_vm)
{
    // bool Scene:setCamera(element camera)
    Scene *l_scene;
    Camera *l_camera;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_scene);
    argStream.ReadElement(l_camera);
    if(!argStream.HasErrors())
    {
        bool l_result = Core::GetCore()->GetInheritManager()->SetSceneCamera(l_scene, l_camera);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaSceneDef::GetCamera(lua_State *f_vm)
{
    // element Scene:getCamera()
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
    // bool Scene:removeCamera()
    Scene *l_scene;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_scene);
    if(!argStream.HasErrors())
    {
        bool l_result = Core::GetCore()->GetInheritManager()->RemoveSceneCamera(l_scene);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaSceneDef::SetLight(lua_State *f_vm)
{
    // bool Scene:setLight(element light)
    Scene *l_scene;
    Light *l_light;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_scene);
    argStream.ReadElement(l_light);
    if(!argStream.HasErrors())
    {
        bool l_result = Core::GetCore()->GetInheritManager()->SetSceneLight(l_scene, l_light);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaSceneDef::GetLight(lua_State *f_vm)
{
    // element Scene:getLight()
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
    // bool Scene:removeLight()
    Scene *l_scene;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_scene);
    if(!argStream.HasErrors())
    {
        bool l_result = Core::GetCore()->GetInheritManager()->RemoveSceneLight(l_scene);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaSceneDef::SetRenderTarget(lua_State *f_vm)
{
    // bool Scene:setRenderTarget(element rt)
    Scene *l_scene;
    RenderTarget *l_rt;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_scene);
    argStream.ReadElement(l_rt);
    if(!argStream.HasErrors())
    {
        bool l_result = Core::GetCore()->GetInheritManager()->SetSceneRenderTarget(l_scene, l_rt);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaSceneDef::GetRenderTarget(lua_State *f_vm)
{
    // element Scene:getRenderTarget()
    Scene *l_scene;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_scene);
    if(!argStream.HasErrors()) l_scene->HasRenderTarget() ? argStream.PushElement(l_scene->GetRenderTarget()) : argStream.PushBoolean(false);
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaSceneDef::RemoveRenderTarget(lua_State *f_vm)
{
    // bool Scene:removeRenderTarget()
    Scene *l_scene;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_scene);
    if(!argStream.HasErrors())
    {
        bool l_result = Core::GetCore()->GetInheritManager()->RemoveSceneRenderTarget(l_scene);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaSceneDef::SetShader(lua_State *f_vm)
{
    // bool Scene:setShader(element shader)
    Scene *l_scene;
    Shader *l_shader;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_scene);
    argStream.ReadElement(l_shader);
    if(!argStream.HasErrors())
    {
        bool l_result = Core::GetCore()->GetInheritManager()->SetSceneShader(l_scene, l_shader);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaSceneDef::GetShader(lua_State *f_vm)
{
    // element Scene:getShader()
    Scene *l_scene;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_scene);
    if(!argStream.HasErrors()) l_scene->HasShader() ? argStream.PushElement(l_scene->GetShader()) : argStream.PushBoolean(false);
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaSceneDef::RemoveShader(lua_State *f_vm)
{
    // bool Scene:removeShader()
    Scene *l_scene;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_scene);
    if(!argStream.HasErrors())
    {
        bool l_result = Core::GetCore()->GetInheritManager()->RemoveSceneShader(l_scene);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaSceneDef::SetSkyGradient(lua_State *f_vm)
{
    // bool Scene:setSkyGradient(float r1, float g1, float b1, float r2, float g2, float b2)
    Scene *l_scene;
    glm::vec3 l_gradientDown, l_gradientUp;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_scene);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_gradientDown[i]);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_gradientUp[i]);
    if(!argStream.HasErrors())
    {
        l_scene->SetSkyGradient(l_gradientDown, l_gradientUp);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaSceneDef::GetSkyGradient(lua_State *f_vm)
{
    // float float float float float float Scene:getSkyGradient()
    Scene *l_scene;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_scene);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_gradientDown, l_gradientUp;
        l_scene->GetSkyGradient(l_gradientDown, l_gradientUp);
        for(int i = 0; i < 3; i++) argStream.PushNumber(l_gradientDown[i]);
        for(int i = 0; i < 3; i++) argStream.PushNumber(l_gradientUp[i]);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaSceneDef::SetActive(lua_State *f_vm)
{
    // bool Scene:setActive()
    Scene *l_scene;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_scene);
    if(!argStream.HasErrors())
    {
        Core::GetCore()->GetRenderManager()->SetActiveScene(l_scene);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
