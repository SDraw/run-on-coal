#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Scene/Camera.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Camera.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Lua
{

int cameraCreate(lua_State *f_vm)
{
    std::string l_text;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadText(l_text);
    if(argStream.HasErrors()||!l_text.length())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    int l_type = Utils::ReadEnumString(l_text,"perspective,orthogonal");
    if(l_type==-1)
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    Camera *l_camera = LuaManager::m_corePointer->GetElementManager()->CreateCamera(CAMERA_PROJECTION_PERSPECTIVE+static_cast<unsigned char>(l_type));
    l_camera ? lua_pushlightuserdata(f_vm,l_camera) : lua_pushboolean(f_vm,0);
    return 1;
}
int cameraDestroy(lua_State *f_vm)
{
    Camera *l_camera;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_camera,ElementType::CameraElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    LuaManager::m_corePointer->GetElementManager()->DestroyCamera(l_camera);
    lua_pushboolean(f_vm,1);
    return 1;
}

int cameraSetPosition(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    lua_Number l_pos[3];
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_camera,ElementType::CameraElement);
    for(int i = 0; i<3; i++) argStream.ReadNumber(l_pos[i]);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec3 l_vPos(l_pos[0],l_pos[1],l_pos[2]);
    l_camera->SetPosition(l_vPos);
    lua_pushboolean(f_vm,1);
    return 1;
}
int cameraGetPosition(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_camera,ElementType::CameraElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec3 l_vPos;
    l_camera->GetPosition(l_vPos);
    lua_pushnumber(f_vm,l_vPos.x);
    lua_pushnumber(f_vm,l_vPos.y);
    lua_pushnumber(f_vm,l_vPos.z);
    return 3;
}

int cameraSetDirection(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    lua_Number l_dir[3];
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_camera,ElementType::CameraElement);
    for(int i = 0; i<3; i++) argStream.ReadNumber(l_dir[i]);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec3 l_vDir(l_dir[0],l_dir[1],l_dir[2]);
    l_camera->SetDirection(l_vDir);
    lua_pushboolean(f_vm,1);
    return 1;
}
int cameraGetDirection(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_camera,ElementType::CameraElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec3 l_vDir;
    l_camera->GetDirection(l_vDir);
    lua_pushnumber(f_vm,l_vDir.x);
    lua_pushnumber(f_vm,l_vDir.y);
    lua_pushnumber(f_vm,l_vDir.z);
    return 3;
}

int cameraSetType(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    std::string l_text;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_camera,ElementType::CameraElement);
    argStream.ReadText(l_text);
    if(argStream.HasErrors()||!l_text.length())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    int l_type = Utils::ReadEnumString(l_text,"perspective,orthogonal");
    if(l_type==-1)
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    l_camera->SetType(CAMERA_PROJECTION_PERSPECTIVE+static_cast<unsigned char>(l_type));
    lua_pushboolean(f_vm,1);
    return 1;
}
int cameraGetType(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_camera,ElementType::CameraElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    switch(l_camera->GetType())
    {
        case CAMERA_PROJECTION_PERSPECTIVE:
            lua_pushstring(f_vm,"perspective");
            break;
        case CAMERA_PROJECTION_ORTHOGONAL:
            lua_pushstring(f_vm,"orthogonal");
            break;
    }
    return 1;
}

int cameraSetFOV(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    lua_Number l_fov;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_camera,ElementType::CameraElement);
    argStream.ReadNumber(l_fov);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    l_camera->SetFOV(static_cast<float>(l_fov));
    lua_pushboolean(f_vm,1);
    return 1;
}
int cameraGetFOV(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_camera,ElementType::CameraElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    float l_fov = l_camera->GetFOV();
    lua_pushnumber(f_vm,l_fov);
    return 1;
}

int cameraSetAspectRatio(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    lua_Number l_ratio;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_camera,ElementType::CameraElement);
    argStream.ReadNumber(l_ratio);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    l_camera->SetAspectRatio(static_cast<float>(l_ratio));
    lua_pushboolean(f_vm,1);
    return 1;
}
int cameraGetAspectRatio(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_camera,ElementType::CameraElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    float l_ratio = l_camera->GetAspectRatio();
    lua_pushnumber(f_vm,l_ratio);
    return 1;
}

int cameraSetOrthoParams(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    lua_Number l_size[4];
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_camera,ElementType::CameraElement);
    for(int i = 0; i<4; i++) argStream.ReadNumber(l_size[i]);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec4 l_vSize(l_size[0],l_size[1],l_size[2],l_size[3]);
    l_camera->SetOrthoParams(l_vSize);
    lua_pushboolean(f_vm,1);
    return 1;
}
int cameraGetOrthoParams(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_camera,ElementType::CameraElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec4 l_size;
    l_camera->GetOrthoParams(l_size);
    lua_pushnumber(f_vm,l_size.x);
    lua_pushnumber(f_vm,l_size.y);
    lua_pushnumber(f_vm,l_size.z);
    lua_pushnumber(f_vm,l_size.w);
    return 4;
}

int cameraSetDepth(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    lua_Number l_depth[2];
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_camera,ElementType::CameraElement);
    for(int i = 0; i<2; i++) argStream.ReadNumber(l_depth[i]);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec2 l_vDepth(l_depth[0],l_depth[1]);
    l_camera->SetDepth(l_vDepth);
    lua_pushboolean(f_vm,1);
    return 1;
}
int cameraGetDepth(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_camera,ElementType::CameraElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec2 l_depth;
    l_camera->GetDepth(l_depth);
    lua_pushnumber(f_vm,l_depth.x);
    lua_pushnumber(f_vm,l_depth.y);
    return 2;
}

int cameraGetViewMatrix(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_camera,ElementType::CameraElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::mat4 l_mat;
    l_camera->GetViewMatrix(l_mat);
    float *l_matPointer = (float*)&l_mat;
    lua_newtable(f_vm);
    for(int i = 0; i<16; i++)
    {
        lua_pushnumber(f_vm,i+1);
        lua_pushnumber(f_vm,l_matPointer[i]);
        lua_settable(f_vm,-3);
    }
    return 1;
}

int cameraGetProjectionMatrix(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_camera,ElementType::CameraElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::mat4 l_mat;
    l_camera->GetProjectionMatrix(l_mat);
    float *l_matPointer = (float*)&l_mat;
    lua_newtable(f_vm);
    for(int i = 0; i<16; i++)
    {
        lua_pushnumber(f_vm,i+1);
        lua_pushnumber(f_vm,l_matPointer[i]);
        lua_settable(f_vm,-3);
    }
    return 1;
}

}
}
