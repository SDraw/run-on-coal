#include "stdafx.h"

#include "Lua/LuaDefs/LuaCameraDef.h"
#include "Lua/LuaDefs/LuaElementDef.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Elements/Camera.h"
#include "Lua/ArgReader.h"
#include "Utils/Utils.h"

namespace ROC
{

const std::vector<std::string> g_CameraTypesTable
{
    "perspective", "orthogonal"
};

}

void ROC::LuaCameraDef::Init(lua_State *f_vm)
{
    Utils::Lua::lua_registerClass(f_vm, "Camera", CameraCreate);
    Utils::Lua::lua_registerClassMethod(f_vm, "setPosition", CameraSetPosition);
    Utils::Lua::lua_registerClassMethod(f_vm, "getPosition", CameraGetPosition);
    Utils::Lua::lua_registerClassMethod(f_vm, "setDirection", CameraSetDirection);
    Utils::Lua::lua_registerClassMethod(f_vm, "getDirection", CameraGetDirection);
    Utils::Lua::lua_registerClassMethod(f_vm, "setProjectionType", CameraSetProjectionType);
    Utils::Lua::lua_registerClassMethod(f_vm, "getProjectionType", CameraGetProjectionType);
    Utils::Lua::lua_registerClassMethod(f_vm, "setFOV", CameraSetFOV);
    Utils::Lua::lua_registerClassMethod(f_vm, "getFOV", CameraGetFOV);
    Utils::Lua::lua_registerClassMethod(f_vm, "setAspectRatio", CameraSetAspectRatio);
    Utils::Lua::lua_registerClassMethod(f_vm, "getAspectRatio", CameraGetAspectRatio);
    Utils::Lua::lua_registerClassMethod(f_vm, "setOrthoParams", CameraSetOrthoParams);
    Utils::Lua::lua_registerClassMethod(f_vm, "getOrthoParams", CameraGetOrthoParams);
    Utils::Lua::lua_registerClassMethod(f_vm, "setDepth", CameraSetDepth);
    Utils::Lua::lua_registerClassMethod(f_vm, "getDepth", CameraGetDepth);
    Utils::Lua::lua_registerClassMethod(f_vm, "getViewMatrix", CameraGetViewMatrix);
    Utils::Lua::lua_registerClassMethod(f_vm, "getProjectionMatrix", CameraGetProjectionMatrix);
    LuaElementDef::AddHierarchyMethods(f_vm);
    Utils::Lua::lua_registerClassFinish(f_vm);
}

int ROC::LuaCameraDef::CameraCreate(lua_State *f_vm)
{
    std::string l_text;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_text);
    if(!argStream.HasErrors() && !l_text.empty())
    {
        int l_type = Utils::Enum::ReadEnumVector(g_CameraTypesTable, l_text);
        if(l_type != -1)
        {
            Camera *l_Camera = LuaManager::GetCore()->GetElementManager()->CreateCamera(l_type);
            l_Camera ? argStream.PushElement(l_Camera) : argStream.PushBoolean(false);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaCameraDef::CameraSetPosition(lua_State *f_vm)
{
    Camera *l_Camera;
    glm::vec3 l_pos;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_Camera);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_pos[i]);
    if(!argStream.HasErrors())
    {
        l_Camera->SetPosition(l_pos);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaCameraDef::CameraGetPosition(lua_State *f_vm)
{
    Camera *l_Camera;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_Camera);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_pos;
        l_Camera->GetPosition(l_pos);
        argStream.PushNumber(l_pos.x);
        argStream.PushNumber(l_pos.y);
        argStream.PushNumber(l_pos.z);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaCameraDef::CameraSetDirection(lua_State *f_vm)
{
    Camera *l_Camera;
    glm::vec3 l_dir;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_Camera);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_dir[i]);
    if(!argStream.HasErrors())
    {
        l_Camera->SetDirection(l_dir);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaCameraDef::CameraGetDirection(lua_State *f_vm)
{
    Camera *l_Camera;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_Camera);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_dir;
        l_Camera->GetDirection(l_dir);
        argStream.PushNumber(l_dir.x);
        argStream.PushNumber(l_dir.y);
        argStream.PushNumber(l_dir.z);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaCameraDef::CameraSetProjectionType(lua_State *f_vm)
{
    Camera *l_Camera;
    std::string l_text;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_Camera);
    argStream.ReadText(l_text);
    if(!argStream.HasErrors() && !l_text.empty())
    {
        int l_type = Utils::Enum::ReadEnumVector(g_CameraTypesTable, l_text);
        if(l_type != -1)
        {
            l_Camera->SetProjectionType(l_type);
            argStream.PushBoolean(true);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaCameraDef::CameraGetProjectionType(lua_State *f_vm)
{
    Camera *l_Camera;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_Camera);
    !argStream.HasErrors() ? argStream.PushText(g_CameraTypesTable[l_Camera->GetProjectionType()]) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaCameraDef::CameraSetFOV(lua_State *f_vm)
{
    Camera *l_Camera;
    float l_fov;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_Camera);
    argStream.ReadNumber(l_fov);
    if(!argStream.HasErrors())
    {
        l_Camera->SetFOV(l_fov);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaCameraDef::CameraGetFOV(lua_State *f_vm)
{
    Camera *l_Camera;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_Camera);
    !argStream.HasErrors() ? argStream.PushNumber(l_Camera->GetFOV()) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaCameraDef::CameraSetAspectRatio(lua_State *f_vm)
{
    Camera *l_Camera;
    float l_ratio;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_Camera);
    argStream.ReadNumber(l_ratio);
    if(!argStream.HasErrors())
    {
        l_Camera->SetAspectRatio(l_ratio);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaCameraDef::CameraGetAspectRatio(lua_State *f_vm)
{
    Camera *l_Camera;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_Camera);
    !argStream.HasErrors() ? argStream.PushNumber(l_Camera->GetAspectRatio()) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaCameraDef::CameraSetOrthoParams(lua_State *f_vm)
{
    Camera *l_Camera;
    glm::vec4 l_params;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_Camera);
    for(int i = 0; i < 4; i++) argStream.ReadNumber(l_params[i]);
    if(!argStream.HasErrors())
    {
        l_Camera->SetOrthoParams(l_params);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaCameraDef::CameraGetOrthoParams(lua_State *f_vm)
{
    Camera *l_Camera;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_Camera);
    if(!argStream.HasErrors())
    {
        glm::vec4 l_size;
        l_Camera->GetOrthoParams(l_size);
        argStream.PushNumber(l_size.x);
        argStream.PushNumber(l_size.y);
        argStream.PushNumber(l_size.z);
        argStream.PushNumber(l_size.w);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaCameraDef::CameraSetDepth(lua_State *f_vm)
{
    Camera *l_Camera;
    glm::vec2 l_depth;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_Camera);
    for(int i = 0; i < 2; i++) argStream.ReadNumber(l_depth[i]);
    if(!argStream.HasErrors())
    {
        l_Camera->SetDepth(l_depth);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaCameraDef::CameraGetDepth(lua_State *f_vm)
{
    Camera *l_Camera;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_Camera);
    if(!argStream.HasErrors())
    {
        glm::vec2 l_depth;
        l_Camera->GetDepth(l_depth);
        argStream.PushNumber(l_depth.x);
        argStream.PushNumber(l_depth.y);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaCameraDef::CameraGetViewMatrix(lua_State *f_vm)
{
    Camera *l_Camera;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_Camera);
    if(!argStream.HasErrors())
    {
        glm::mat4 l_mat;
        l_Camera->GetViewMatrix(l_mat);
        argStream.PushVector(glm::value_ptr(l_mat), 16);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaCameraDef::CameraGetProjectionMatrix(lua_State *f_vm)
{
    Camera *l_Camera;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_Camera);
    if(!argStream.HasErrors())
    {
        glm::mat4 l_mat;
        l_Camera->GetProjectionMatrix(l_mat);
        argStream.PushVector(glm::value_ptr(l_mat), 16);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
