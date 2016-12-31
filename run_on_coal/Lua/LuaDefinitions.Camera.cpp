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

const std::vector<std::string> g_cameraTypesTable
{
    "perspective", "orthogonal"
};

int cameraCreate(lua_State *f_vm)
{
    std::string l_text;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadText(l_text);
    if(!argStream.HasErrors() && !l_text.empty())
    {
        int l_type = Utils::ReadEnumVector(g_cameraTypesTable, l_text);
        if(l_type != -1)
        {
            Camera *l_camera = LuaManager::m_corePointer->GetElementManager()->CreateCamera(CAMERA_PROJECTION_PERSPECTIVE + static_cast<unsigned char>(l_type));
            l_camera ? lua_pushlightuserdata(f_vm, l_camera) : lua_pushboolean(f_vm, 0);
        }
        else lua_pushboolean(f_vm, 0);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int cameraDestroy(lua_State *f_vm)
{
    Camera *l_camera;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_camera), ElementType::CameraElement);
    if(!argStream.HasErrors())
    {
        LuaManager::m_corePointer->GetElementManager()->DestroyCamera(l_camera);
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}

int cameraSetPosition(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    glm::vec3 l_pos;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_camera), ElementType::CameraElement);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_pos[i]);
    if(!argStream.HasErrors())
    {
        l_camera->SetPosition(l_pos);
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int cameraGetPosition(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    int l_returnVal = 1;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_camera), ElementType::CameraElement);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_vPos;
        l_camera->GetPosition(l_vPos);
        lua_pushnumber(f_vm, l_vPos.x);
        lua_pushnumber(f_vm, l_vPos.y);
        lua_pushnumber(f_vm, l_vPos.z);
        l_returnVal = 3;
    }
    else lua_pushboolean(f_vm, 0);
    return l_returnVal;
}

int cameraSetDirection(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    glm::vec3 l_dir;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_camera), ElementType::CameraElement);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_dir[i]);
    if(!argStream.HasErrors())
    {
        l_camera->SetDirection(l_dir);
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int cameraGetDirection(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    int l_returnVal = 1;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_camera), ElementType::CameraElement);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_vDir;
        l_camera->GetDirection(l_vDir);
        lua_pushnumber(f_vm, l_vDir.x);
        lua_pushnumber(f_vm, l_vDir.y);
        lua_pushnumber(f_vm, l_vDir.z);
        l_returnVal = 3;
    }
    else lua_pushboolean(f_vm, 0);
    return l_returnVal;
}

int cameraSetType(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    std::string l_text;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_camera), ElementType::CameraElement);
    argStream.ReadText(l_text);
    if(!argStream.HasErrors() && !l_text.empty())
    {
        int l_type = Utils::ReadEnumVector(g_cameraTypesTable, l_text);
        if(l_type != -1)
        {
            l_camera->SetType(CAMERA_PROJECTION_PERSPECTIVE + static_cast<unsigned char>(l_type));
            lua_pushboolean(f_vm, 1);
        }
        else lua_pushboolean(f_vm, 0);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int cameraGetType(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_camera), ElementType::CameraElement);
    if(!argStream.HasErrors())
    {
        switch(l_camera->GetType())
        {
            case CAMERA_PROJECTION_PERSPECTIVE:
                lua_pushstring(f_vm, "perspective");
                break;
            case CAMERA_PROJECTION_ORTHOGONAL:
                lua_pushstring(f_vm, "orthogonal");
                break;
            default:
                lua_pushstring(f_vm, "unknown");
        }
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}

int cameraSetFOV(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    float l_fov;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_camera), ElementType::CameraElement);
    argStream.ReadNumber(l_fov);
    if(!argStream.HasErrors())
    {
        l_camera->SetFOV(l_fov);
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int cameraGetFOV(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_camera), ElementType::CameraElement);
    if(!argStream.HasErrors()) lua_pushnumber(f_vm, l_camera->GetFOV());
    else lua_pushboolean(f_vm, 0);
    return 1;
}

int cameraSetAspectRatio(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    float l_ratio;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_camera), ElementType::CameraElement);
    argStream.ReadNumber(l_ratio);
    if(!argStream.HasErrors())
    {
        l_camera->SetAspectRatio(l_ratio);
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int cameraGetAspectRatio(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_camera), ElementType::CameraElement);
    if(!argStream.HasErrors()) lua_pushnumber(f_vm, l_camera->GetAspectRatio());
    else lua_pushboolean(f_vm, 0);
    return 1;
}

int cameraSetOrthoParams(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    glm::vec4 l_params;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_camera), ElementType::CameraElement);
    for(int i = 0; i < 4; i++) argStream.ReadNumber(l_params[i]);
    if(!argStream.HasErrors())
    {
        l_camera->SetOrthoParams(l_params);
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int cameraGetOrthoParams(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    int l_returnVal = 1;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_camera), ElementType::CameraElement);
    if(!argStream.HasErrors())
    {
        glm::vec4 l_size;
        l_camera->GetOrthoParams(l_size);
        lua_pushnumber(f_vm, l_size.x);
        lua_pushnumber(f_vm, l_size.y);
        lua_pushnumber(f_vm, l_size.z);
        lua_pushnumber(f_vm, l_size.w);
        l_returnVal = 4;
    }
    else lua_pushboolean(f_vm, 0);
    return l_returnVal;
}

int cameraSetDepth(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    glm::vec2 l_depth;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_camera), ElementType::CameraElement);
    for(int i = 0; i < 2; i++) argStream.ReadNumber(l_depth[i]);
    if(!argStream.HasErrors())
    {
        l_camera->SetDepth(l_depth);
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int cameraGetDepth(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    int l_returnVal = 1;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_camera), ElementType::CameraElement);
    if(!argStream.HasErrors())
    {
        glm::vec2 l_depth;
        l_camera->GetDepth(l_depth);
        lua_pushnumber(f_vm, l_depth.x);
        lua_pushnumber(f_vm, l_depth.y);
        l_returnVal++;
    }
    else lua_pushboolean(f_vm, 0);
    return 2;
}

int cameraGetViewMatrix(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_camera), ElementType::CameraElement);
    if(!argStream.HasErrors())
    {
        glm::mat4 l_mat;
        l_camera->GetViewMatrix(l_mat);
        float *l_matPointer = reinterpret_cast<float*>(&l_mat);
        lua_newtable(f_vm);
        for(int i = 0; i < 16; i++)
        {
            lua_pushnumber(f_vm, i + 1);
            lua_pushnumber(f_vm, l_matPointer[i]);
            lua_settable(f_vm, -3);
        }
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}

int cameraGetProjectionMatrix(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_camera), ElementType::CameraElement);
    if(!argStream.HasErrors())
    {
        glm::mat4 l_mat;
        l_camera->GetProjectionMatrix(l_mat);
        float *l_matPointer = reinterpret_cast<float*>(&l_mat);
        lua_newtable(f_vm);
        for(int i = 0; i < 16; i++)
        {
            lua_pushnumber(f_vm, i + 1);
            lua_pushnumber(f_vm, l_matPointer[i]);
            lua_settable(f_vm, -3);
        }
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}

}
}