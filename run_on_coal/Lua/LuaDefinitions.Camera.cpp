#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Elements/Camera.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Camera.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Lua
{

const std::vector<std::string> g_cameraTypesTable
{
    "perspective", "orthogonal"
};

int cameraCreate(lua_State *f_vm)
{
    std::string l_text;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_text);
    if(!argStream.HasErrors() && !l_text.empty())
    {
        int l_type = Utils::Enum::ReadEnumVector(g_cameraTypesTable, l_text);
        if(l_type != -1)
        {
            Camera *l_camera = LuaManager::GetCore()->GetElementManager()->CreateCamera(l_type);
            l_camera ? argStream.PushPointer(l_camera) : argStream.PushBoolean(false);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int cameraSetPosition(lua_State *f_vm)
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
int cameraGetPosition(lua_State *f_vm)
{
    Camera *l_camera;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_camera);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_pos;
        l_camera->GetPosition(l_pos);
        argStream.PushNumber(l_pos.x);
        argStream.PushNumber(l_pos.y);
        argStream.PushNumber(l_pos.z);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int cameraSetDirection(lua_State *f_vm)
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
int cameraGetDirection(lua_State *f_vm)
{
    Camera *l_camera;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_camera);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_dir;
        l_camera->GetDirection(l_dir);
        argStream.PushNumber(l_dir.x);
        argStream.PushNumber(l_dir.y);
        argStream.PushNumber(l_dir.z);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int cameraSetType(lua_State *f_vm)
{
    Camera *l_camera;
    std::string l_text;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_camera);
    argStream.ReadText(l_text);
    if(!argStream.HasErrors() && !l_text.empty())
    {
        int l_type = Utils::Enum::ReadEnumVector(g_cameraTypesTable, l_text);
        if(l_type != -1)
        {
            l_camera->SetType(l_type);
            argStream.PushBoolean(true);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int cameraGetType(lua_State *f_vm)
{
    Camera *l_camera;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_camera);
    !argStream.HasErrors() ? argStream.PushText(g_cameraTypesTable[l_camera->GetType()]) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int cameraSetFOV(lua_State *f_vm)
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
int cameraGetFOV(lua_State *f_vm)
{
    Camera *l_camera;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_camera);
    !argStream.HasErrors() ? argStream.PushNumber(l_camera->GetFOV()) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int cameraSetAspectRatio(lua_State *f_vm)
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
int cameraGetAspectRatio(lua_State *f_vm)
{
    Camera *l_camera;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_camera);
    !argStream.HasErrors() ? argStream.PushNumber(l_camera->GetAspectRatio()) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int cameraSetOrthoParams(lua_State *f_vm)
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
int cameraGetOrthoParams(lua_State *f_vm)
{
    Camera *l_camera;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_camera);
    if(!argStream.HasErrors())
    {
        glm::vec4 l_size;
        l_camera->GetOrthoParams(l_size);
        argStream.PushNumber(l_size.x);
        argStream.PushNumber(l_size.y);
        argStream.PushNumber(l_size.z);
        argStream.PushNumber(l_size.w);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int cameraSetDepth(lua_State *f_vm)
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
int cameraGetDepth(lua_State *f_vm)
{
    Camera *l_camera;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_camera);
    if(!argStream.HasErrors())
    {
        glm::vec2 l_depth;
        l_camera->GetDepth(l_depth);
        argStream.PushNumber(l_depth.x);
        argStream.PushNumber(l_depth.y);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int cameraGetViewMatrix(lua_State *f_vm)
{
    Camera *l_camera;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_camera);
    if(!argStream.HasErrors())
    {
        glm::mat4 l_mat;
        l_camera->GetViewMatrix(l_mat);
        argStream.PushMatrix(glm::value_ptr(l_mat), 16);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int cameraGetProjectionMatrix(lua_State *f_vm)
{
    Camera *l_camera;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_camera);
    if(!argStream.HasErrors())
    {
        glm::mat4 l_mat;
        l_camera->GetProjectionMatrix(l_mat);
        argStream.PushMatrix(glm::value_ptr(l_mat), 16);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

}
}
