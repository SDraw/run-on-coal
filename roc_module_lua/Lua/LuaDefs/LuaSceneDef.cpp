#include "stdafx.h"

#include "Lua/LuaDefs/LuaSceneDef.h"

#include "Lua/LuaArgReader.h"
#include "Lua/LuaDefs/LuaElementDef.h"
#include "Module/LuaModule.h"
#include "Utils/LuaUtils.h"

void LuaSceneDef::Init(lua_State *f_vm)
{
    LuaUtils::AddClass(f_vm, "Scene", Create);
    LuaUtils::AddClassMethod(f_vm, "addModel", AddModel);
    LuaUtils::AddClassMethod(f_vm, "removeModel", RemoveModel);
    LuaUtils::AddClassMethod(f_vm, "setModelLayer", SetModelLayer);
    LuaUtils::AddClassMethod(f_vm, "setCamera", SetCamera);
    LuaUtils::AddClassMethod(f_vm, "getCamera", GetCamera);
    LuaUtils::AddClassMethod(f_vm, "removeCamera", RemoveCamera);
    LuaUtils::AddClassMethod(f_vm, "addLight", AddLight);
    LuaUtils::AddClassMethod(f_vm, "removeLight", RemoveLight);
    LuaUtils::AddClassMethod(f_vm, "setRenderTarget", SetRenderTarget);
    LuaUtils::AddClassMethod(f_vm, "getRenderTarget", GetRenderTarget);
    LuaUtils::AddClassMethod(f_vm, "removeRenderTarget", RemoveRenderTarget);
    LuaUtils::AddClassMethod(f_vm, "addShader", AddShader);
    LuaUtils::AddClassMethod(f_vm, "removeShader", RemoveShader);
    LuaUtils::AddClassMethod(f_vm, "setActive", SetActive);
    LuaUtils::AddClassMethod(f_vm, "draw", Draw);
    LuaElementDef::AddHierarchyMethods(f_vm);
    LuaUtils::AddClassFinish(f_vm);
}

int LuaSceneDef::Create(lua_State *f_vm)
{
    // element Scene()
    LuaArgReader l_argStream(f_vm);
    ROC::IScene *l_scene = LuaModule::GetModule()->GetEngineCore()->GetIElementManager()->CreateIScene();
    l_scene ? l_argStream.PushElement(l_scene) : l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSceneDef::AddModel(lua_State *f_vm)
{
    // bool Scene:addModel(element model [, str layer = "default"])
    ROC::IScene *l_scene;
    ROC::IModel *l_model;
    std::string l_group("default");
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_scene);
    l_argStream.ReadElement(l_model);
    l_argStream.ReadNextText(l_group);
    if(!l_argStream.HasErrors())
    {
        bool l_result = l_scene->AddIModel(l_model, l_group);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSceneDef::RemoveModel(lua_State *f_vm)
{
    // bool Scene:removeModel(element model)
    ROC::IScene *l_scene;
    ROC::IModel *l_model;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_scene);
    l_argStream.ReadElement(l_model);
    if(!l_argStream.HasErrors())
    {
        bool l_result = l_scene->RemoveIModel(l_model);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSceneDef::SetModelLayer(lua_State *f_vm)
{
    // bool Scene:setModelLayer(element model, str layer)
    ROC::IScene *l_scene;
    ROC::IModel *l_model;
    std::string l_layer;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_scene);
    l_argStream.ReadElement(l_model);
    l_argStream.ReadText(l_layer);
    if(!l_argStream.HasErrors() && !l_layer.empty())
    {
        bool l_result = l_scene->SetIModelLayer(l_model, l_layer);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSceneDef::SetCamera(lua_State *f_vm)
{
    // bool Scene:setCamera(element camera)
    ROC::IScene *l_scene;
    ROC::ICamera *l_camera;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_scene);
    l_argStream.ReadElement(l_camera);
    if(!l_argStream.HasErrors())
    {
        bool l_result = l_scene->SetICamera(l_camera);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSceneDef::GetCamera(lua_State *f_vm)
{
    // element Scene:getCamera()
    ROC::IScene *l_scene;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_scene);
    if(!l_argStream.HasErrors())
    {
        ROC::ICamera *l_camera = l_scene->GetICamera();
        l_camera ? l_argStream.PushElement(l_camera) : l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSceneDef::RemoveCamera(lua_State *f_vm)
{
    // bool Scene:removeCamera()
    ROC::IScene *l_scene;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_scene);
    if(!l_argStream.HasErrors())
    {
        bool l_result = l_scene->RemoveCamera();
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSceneDef::AddLight(lua_State *f_vm)
{
    // bool Scene:setLight(element light)
    ROC::IScene *l_scene;
    ROC::ILight *l_light;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_scene);
    l_argStream.ReadElement(l_light);
    if(!l_argStream.HasErrors())
    {
        bool l_result = l_scene->AddILight(l_light);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSceneDef::RemoveLight(lua_State *f_vm)
{
    // bool Scene:removeLight()
    ROC::IScene *l_scene;
    ROC::ILight *l_light;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_scene);
    l_argStream.ReadElement(l_light);
    if(!l_argStream.HasErrors())
    {
        bool l_result = l_scene->RemoveILight(l_light);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSceneDef::SetRenderTarget(lua_State *f_vm)
{
    // bool Scene:setRenderTarget(element rt)
    ROC::IScene *l_scene;
    ROC::IRenderTarget *l_rt;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_scene);
    l_argStream.ReadElement(l_rt);
    if(!l_argStream.HasErrors())
    {
        bool l_result = l_scene->SetIRenderTarget(l_rt);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSceneDef::GetRenderTarget(lua_State *f_vm)
{
    // element Scene:getRenderTarget()
    ROC::IScene *l_scene;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_scene);
    if(!l_argStream.HasErrors())
    {
        ROC::IRenderTarget *l_rt = l_scene->GetIRenderTarget();
        l_rt ? l_argStream.PushElement(l_rt) : l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSceneDef::RemoveRenderTarget(lua_State *f_vm)
{
    // bool Scene:removeRenderTarget()
    ROC::IScene *l_scene;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_scene);
    if(!l_argStream.HasErrors())
    {
        bool l_result = l_scene->RemoveRenderTarget();
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSceneDef::AddShader(lua_State *f_vm)
{
    // bool Scene:addShader(element shader [, str layer = "default", int priority = 1])
    ROC::IScene *l_scene;
    ROC::IShader *l_shader;
    std::string l_layer("default");
    unsigned char l_priority = 1U;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_scene);
    l_argStream.ReadElement(l_shader);
    l_argStream.ReadNextText(l_layer);
    l_argStream.ReadNextInteger(l_priority);
    if(!l_argStream.HasErrors())
    {
        bool l_result = l_scene->AddIShader(l_shader, l_layer, l_priority);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSceneDef::RemoveShader(lua_State *f_vm)
{
    // bool Scene:removeShader(element shader)
    ROC::IScene *l_scene;
    ROC::IShader *l_shader;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_scene);
    l_argStream.ReadElement(l_shader);
    if(!l_argStream.HasErrors())
    {
        bool l_result = l_scene->RemoveIShader(l_shader);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSceneDef::SetActive(lua_State *f_vm)
{
    // bool Scene:setActive()
    ROC::IScene *l_scene;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_scene);
    if(!l_argStream.HasErrors())
    {
        LuaModule::GetModule()->GetEngineCore()->GetIRenderManager()->SetActiveScene(l_scene);
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaSceneDef::Draw(lua_State *f_vm)
{
    // bool Scene:draw()
    ROC::IScene *l_scene;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_scene);
    if(!l_argStream.HasErrors())
    {
        bool l_result = LuaModule::GetModule()->GetEngineCore()->GetIRenderManager()->DrawScene(l_scene);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}
