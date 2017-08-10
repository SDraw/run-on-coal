#include "stdafx.h"

#include "Lua/LuaDefs/LuaCameraDef.h"
#include "Lua/LuaDefs/LuaElementDef.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Elements/Camera.h"
#include "Lua/ArgReader.h"
#include "Utils/EnumUtils.h"
#include "Utils/LuaUtils.h"

namespace ROC
{

const std::vector<std::string> g_CameraTypesTable
{
    "perspective", "orthogonal"
};

}

void ROC::LuaCameraDef::Init(lua_State *f_vm)
{
    LuaUtils::AddClass(f_vm, "Camera", Create);
    LuaUtils::AddClassMethod(f_vm, "setPosition", SetPosition);
    LuaUtils::AddClassMethod(f_vm, "getPosition", GetPosition);
    LuaUtils::AddClassMethod(f_vm, "setDirection", SetDirection);
    LuaUtils::AddClassMethod(f_vm, "getDirection", GetDirection);
    LuaUtils::AddClassMethod(f_vm, "setProjectionType", SetProjectionType);
    LuaUtils::AddClassMethod(f_vm, "getProjectionType", GetProjectionType);
    LuaUtils::AddClassMethod(f_vm, "setFOV", SetFOV);
    LuaUtils::AddClassMethod(f_vm, "getFOV", GetFOV);
    LuaUtils::AddClassMethod(f_vm, "setAspectRatio", SetAspectRatio);
    LuaUtils::AddClassMethod(f_vm, "getAspectRatio", GetAspectRatio);
    LuaUtils::AddClassMethod(f_vm, "setOrthoParams", SetOrthoParams);
    LuaUtils::AddClassMethod(f_vm, "getOrthoParams", GetOrthoParams);
    LuaUtils::AddClassMethod(f_vm, "setDepth", SetDepth);
    LuaUtils::AddClassMethod(f_vm, "getDepth", GetDepth);
    LuaUtils::AddClassMethod(f_vm, "getViewMatrix", GetViewMatrix);
    LuaUtils::AddClassMethod(f_vm, "getProjectionMatrix", GetProjectionMatrix);
    LuaUtils::AddClassMethod(f_vm, "getViewProjectionMatrix", GetViewProjectionMatrix);
    LuaElementDef::AddHierarchyMethods(f_vm);
    LuaUtils::AddClassFinish(f_vm);
}

int ROC::LuaCameraDef::Create(lua_State *f_vm)
{
    std::string l_text;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_text);
    if(!argStream.HasErrors() && !l_text.empty())
    {
        int l_type = EnumUtils::ReadEnumVector(l_text, g_CameraTypesTable);
        if(l_type != -1)
        {
            Camera *l_camera = LuaManager::GetCore()->GetElementManager()->CreateCamera(l_type);
            l_camera ? argStream.PushElement(l_camera) : argStream.PushBoolean(false);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaCameraDef::SetPosition(lua_State *f_vm)
{
    Camera *l_camera;
    glm::vec3 l_pos;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_camera);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_pos[i]);
    if(!argStream.HasErrors())
    {
        l_camera->SetPosition(l_pos);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaCameraDef::GetPosition(lua_State *f_vm)
{
    Camera *l_camera;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_camera);
    if(!argStream.HasErrors())
    {
        const glm::vec3 &l_pos = l_camera->GetPosition();
        for(int i =0; i < 3; i++) argStream.PushNumber(l_pos[i]);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaCameraDef::SetDirection(lua_State *f_vm)
{
    Camera *l_camera;
    glm::vec3 l_dir;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_camera);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_dir[i]);
    if(!argStream.HasErrors())
    {
        l_camera->SetDirection(l_dir);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaCameraDef::GetDirection(lua_State *f_vm)
{
    Camera *l_camera;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_camera);
    if(!argStream.HasErrors())
    {
        const glm::vec3 &l_dir = l_camera->GetDirection();
        for(int i=0; i < 3;i++) argStream.PushNumber(l_dir[i]);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaCameraDef::SetProjectionType(lua_State *f_vm)
{
    Camera *l_camera;
    std::string l_text;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_camera);
    argStream.ReadText(l_text);
    if(!argStream.HasErrors() && !l_text.empty())
    {
        int l_type = EnumUtils::ReadEnumVector(l_text, g_CameraTypesTable);
        if(l_type != -1)
        {
            l_camera->SetProjectionType(l_type);
            argStream.PushBoolean(true);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaCameraDef::GetProjectionType(lua_State *f_vm)
{
    Camera *l_camera;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_camera);
    !argStream.HasErrors() ? argStream.PushText(g_CameraTypesTable[l_camera->GetProjectionType()]) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaCameraDef::SetFOV(lua_State *f_vm)
{
    Camera *l_camera;
    float l_fov;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_camera);
    argStream.ReadNumber(l_fov);
    if(!argStream.HasErrors())
    {
        l_camera->SetFOV(l_fov);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaCameraDef::GetFOV(lua_State *f_vm)
{
    Camera *l_camera;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_camera);
    !argStream.HasErrors() ? argStream.PushNumber(l_camera->GetFOV()) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaCameraDef::SetAspectRatio(lua_State *f_vm)
{
    Camera *l_camera;
    float l_ratio;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_camera);
    argStream.ReadNumber(l_ratio);
    if(!argStream.HasErrors())
    {
        l_camera->SetAspectRatio(l_ratio);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaCameraDef::GetAspectRatio(lua_State *f_vm)
{
    Camera *l_camera;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_camera);
    !argStream.HasErrors() ? argStream.PushNumber(l_camera->GetAspectRatio()) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaCameraDef::SetOrthoParams(lua_State *f_vm)
{
    Camera *l_camera;
    glm::vec4 l_params;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_camera);
    for(int i = 0; i < 4; i++) argStream.ReadNumber(l_params[i]);
    if(!argStream.HasErrors())
    {
        l_camera->SetOrthoParams(l_params);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaCameraDef::GetOrthoParams(lua_State *f_vm)
{
    Camera *l_camera;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_camera);
    if(!argStream.HasErrors())
    {
        const glm::vec4 &l_size = l_camera->GetOrthoParams();
        for(int i = 0; i < 4; i++) argStream.PushNumber(l_size[i]);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaCameraDef::SetDepth(lua_State *f_vm)
{
    Camera *l_camera;
    glm::vec2 l_depth;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_camera);
    for(int i = 0; i < 2; i++) argStream.ReadNumber(l_depth[i]);
    if(!argStream.HasErrors())
    {
        l_camera->SetDepth(l_depth);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaCameraDef::GetDepth(lua_State *f_vm)
{
    Camera *l_camera;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_camera);
    if(!argStream.HasErrors())
    {
        const glm::vec2 &l_depth = l_camera->GetDepth();
        for(int i=0; i < 2;i++) argStream.PushNumber(l_depth[i]);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaCameraDef::GetViewMatrix(lua_State *f_vm)
{
    Camera *l_camera;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_camera);
    if(!argStream.HasErrors())
    {
        const glm::mat4 &l_mat = l_camera->GetViewMatrix();
        const float *l_matPtr = glm::value_ptr(l_mat);
        for(int i = 0; i < 16; i++) argStream.PushNumber(l_matPtr[i]);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaCameraDef::GetProjectionMatrix(lua_State *f_vm)
{
    Camera *l_camera;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_camera);
    if(!argStream.HasErrors())
    {
        const glm::mat4 &l_mat = l_camera->GetProjectionMatrix();
        const float *l_matPtr = glm::value_ptr(l_mat);
        for(int i = 0; i < 16; i++) argStream.PushNumber(l_matPtr[i]);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaCameraDef::GetViewProjectionMatrix(lua_State *f_vm)
{
    Camera *l_camera;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_camera);
    if(!argStream.HasErrors())
    {
        const glm::mat4 &l_mat = l_camera->GeViewProjectionMatrix();
        const float *l_matPtr = glm::value_ptr(l_mat);
        for(int i = 0; i < 16; i++) argStream.PushNumber(l_matPtr[i]);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
