#include "stdafx.h"

#include "Lua/LuaDefs/LuaCameraDef.h"

#include "Lua/LuaArgReader.h"
#include "Lua/LuaDefs/LuaElementDef.h"
#include "Module/LuaModule.h"
#include "Utils/EnumUtils.h"
#include "Utils/LuaUtils.h"

const std::vector<std::string> g_cameraTypes
{
    "perspective", "orthogonal", "screen", "vr_left", "vr_right"
};

void LuaCameraDef::Init(lua_State *f_vm)
{
    LuaUtils::AddClass(f_vm, "Camera", Create);
    LuaUtils::AddClassMethod(f_vm, "setPosition", SetPosition);
    LuaUtils::AddClassMethod(f_vm, "getPosition", GetPosition);
    LuaUtils::AddClassMethod(f_vm, "setDirection", SetDirection);
    LuaUtils::AddClassMethod(f_vm, "getDirection", GetDirection);
    LuaUtils::AddClassMethod(f_vm, "setUpDirection", SetUpDirection);
    LuaUtils::AddClassMethod(f_vm, "getUpDirection", GetUpDirection);
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

int LuaCameraDef::Create(lua_State *f_vm)
{
    // element Camera(str type)
    std::string l_text;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadText(l_text);
    if(!l_argStream.HasErrors() && !l_text.empty())
    {
        size_t l_type = EnumUtils::ReadEnumVector(l_text, g_cameraTypes);
        if(l_type != std::numeric_limits<size_t>::max())
        {
            ROC::ICamera *l_camera = LuaModule::GetModule()->GetEngineCore()->GetIElementManager()->CreateICamera(static_cast<unsigned char>(l_type));
            l_camera ? l_argStream.PushElement(l_camera) : l_argStream.PushBoolean(false);
        }
        else l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaCameraDef::SetPosition(lua_State *f_vm)
{
    // bool Camera:setPosition(float x, float y, float z)
    ROC::ICamera *l_camera;
    glm::vec3 l_pos;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_camera);
    for(int i = 0; i < 3; i++) l_argStream.ReadNumber(l_pos[i]);
    if(!l_argStream.HasErrors())
    {
        l_camera->SetPosition(l_pos);
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaCameraDef::GetPosition(lua_State *f_vm)
{
    // float float float Camera:getPosition()
    ROC::ICamera *l_camera;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_camera);
    if(!l_argStream.HasErrors())
    {
        const glm::vec3 &l_pos = l_camera->GetPosition();
        for(int i = 0; i < 3; i++) l_argStream.PushNumber(l_pos[i]);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaCameraDef::SetDirection(lua_State *f_vm)
{
    // bool Camera:setDirection(float x, float y, float z [, float w = nil ])
    ROC::ICamera *l_camera;
    glm::vec3 l_dir;
    float l_quatW = std::nanf("0");
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_camera);
    for(int i = 0; i < 3; i++) l_argStream.ReadNumber(l_dir[i]);
    l_argStream.ReadNextNumber(l_quatW);
    if(!l_argStream.HasErrors())
    {
        if(std::isnan(l_quatW)) l_camera->SetDirection(l_dir);
        else
        {
            glm::quat l_dirRot(l_quatW, l_dir.x, l_dir.y, l_dir.z);
            l_camera->SetDirection(l_dirRot);
        }
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaCameraDef::GetDirection(lua_State *f_vm)
{
    // float float float Camera:getDirection()
    ROC::ICamera *l_camera;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_camera);
    if(!l_argStream.HasErrors())
    {
        const glm::vec3 &l_dir = l_camera->GetDirection();
        for(int i = 0; i < 3; i++) l_argStream.PushNumber(l_dir[i]);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaCameraDef::SetUpDirection(lua_State *f_vm)
{
    // bool Camera:setUpDirection(float x, float y, float z)
    ROC::ICamera *l_camera;
    glm::vec3 l_dir;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_camera);
    for(int i = 0; i < 3; i++) l_argStream.ReadNumber(l_dir[i]);
    if(!l_argStream.HasErrors())
    {
        l_camera->SetUpDirection(l_dir);
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaCameraDef::GetUpDirection(lua_State *f_vm)
{
    // float float float Camera:getUpDirection()
    ROC::ICamera *l_camera;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_camera);
    if(!l_argStream.HasErrors())
    {
        const glm::vec3 &l_dir = l_camera->GetUpDirection();
        for(int i = 0; i < 3; i++) l_argStream.PushNumber(l_dir[i]);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaCameraDef::SetProjectionType(lua_State *f_vm)
{
    // bool Camera:setProjectionType(str type)
    ROC::ICamera *l_camera;
    std::string l_text;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_camera);
    l_argStream.ReadText(l_text);
    if(!l_argStream.HasErrors() && !l_text.empty())
    {
        size_t l_type = EnumUtils::ReadEnumVector(l_text, g_cameraTypes);
        if(l_type != std::numeric_limits<size_t>::max())
        {
            l_camera->SetProjectionType(static_cast<unsigned char>(l_type));
            l_argStream.PushBoolean(true);
        }
        else l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaCameraDef::GetProjectionType(lua_State *f_vm)
{
    // str Camera:getProjectionType()
    ROC::ICamera *l_camera;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_camera);
    !l_argStream.HasErrors() ? l_argStream.PushText(g_cameraTypes[l_camera->GetProjectionType()]) : l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaCameraDef::SetFOV(lua_State *f_vm)
{
    // bool Camera:setFOV(float fov)
    ROC::ICamera *l_camera;
    float l_fov;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_camera);
    l_argStream.ReadNumber(l_fov);
    if(!l_argStream.HasErrors())
    {
        l_camera->SetFOV(l_fov);
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaCameraDef::GetFOV(lua_State *f_vm)
{
    // float Camera:getFOV()
    ROC::ICamera *l_camera;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_camera);
    !l_argStream.HasErrors() ? l_argStream.PushNumber(l_camera->GetFOV()) : l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaCameraDef::SetAspectRatio(lua_State *f_vm)
{
    // bool Camera:setAspectRation(float ratio)
    ROC::ICamera *l_camera;
    float l_ratio;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_camera);
    l_argStream.ReadNumber(l_ratio);
    if(!l_argStream.HasErrors())
    {
        l_camera->SetAspectRatio(l_ratio);
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaCameraDef::GetAspectRatio(lua_State *f_vm)
{
    // float Camera:getAspectRation()
    ROC::ICamera *l_camera;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_camera);
    !l_argStream.HasErrors() ? l_argStream.PushNumber(l_camera->GetAspectRatio()) : l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaCameraDef::SetOrthoParams(lua_State *f_vm)
{
    // bool Camera:setOrthoParams(float left, float right, float bottom, float top)
    ROC::ICamera *l_camera;
    glm::vec4 l_params;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_camera);
    for(int i = 0; i < 4; i++) l_argStream.ReadNumber(l_params[i]);
    if(!l_argStream.HasErrors())
    {
        l_camera->SetOrthoParams(l_params);
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaCameraDef::GetOrthoParams(lua_State *f_vm)
{
    // float float float float Camera:getOrthoParams()
    ROC::ICamera *l_camera;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_camera);
    if(!l_argStream.HasErrors())
    {
        const glm::vec4 &l_size = l_camera->GetOrthoParams();
        for(int i = 0; i < 4; i++) l_argStream.PushNumber(l_size[i]);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaCameraDef::SetDepth(lua_State *f_vm)
{
    // bool Camera:setDepth(float near, float far)
    ROC::ICamera *l_camera;
    glm::vec2 l_depth;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_camera);
    for(int i = 0; i < 2; i++) l_argStream.ReadNumber(l_depth[i]);
    if(!l_argStream.HasErrors())
    {
        l_camera->SetDepth(l_depth);
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaCameraDef::GetDepth(lua_State *f_vm)
{
    // float float Camera:getDepth()
    ROC::ICamera *l_camera;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_camera);
    if(!l_argStream.HasErrors())
    {
        const glm::vec2 &l_depth = l_camera->GetDepth();
        for(int i = 0; i < 2; i++) l_argStream.PushNumber(l_depth[i]);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaCameraDef::GetViewMatrix(lua_State *f_vm)
{
    // float float float float float float float float float float float float float float float float Camera:getViewMatrix()
    ROC::ICamera *l_camera;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_camera);
    if(!l_argStream.HasErrors())
    {
        const glm::mat4 &l_mat = l_camera->GetViewMatrix();
        const float *l_matPtr = glm::value_ptr(l_mat);
        for(int i = 0; i < 16; i++) l_argStream.PushNumber(l_matPtr[i]);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaCameraDef::GetProjectionMatrix(lua_State *f_vm)
{
    // float float float float float float float float float float float float float float float float Camera:getProjectionMatrix()
    ROC::ICamera *l_camera;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_camera);
    if(!l_argStream.HasErrors())
    {
        const glm::mat4 &l_mat = l_camera->GetProjectionMatrix();
        const float *l_matPtr = glm::value_ptr(l_mat);
        for(int i = 0; i < 16; i++) l_argStream.PushNumber(l_matPtr[i]);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaCameraDef::GetViewProjectionMatrix(lua_State *f_vm)
{
    // float float float float float float float float float float float float float float float float Camera:getViewProjectionMatrix()
    ROC::ICamera *l_camera;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_camera);
    if(!l_argStream.HasErrors())
    {
        const glm::mat4 &l_mat = l_camera->GetViewProjectionMatrix();
        const float *l_matPtr = glm::value_ptr(l_mat);
        for(int i = 0; i < 16; i++) l_argStream.PushNumber(l_matPtr[i]);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}
