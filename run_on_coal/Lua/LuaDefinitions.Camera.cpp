#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Scene/Camera.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Camera.h"

namespace ROC
{
namespace Lua 
{

int cameraCreate(lua_State *f_vm)
{
    Camera *l_camera = LuaManager::m_corePointer->GetElementManager()->CreateCamera();
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
    for(int i=0; i < 3; i++) argStream.ReadNumber(l_pos[i]);
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
    for(int i=0; i < 3; i++) argStream.ReadNumber(l_dir[i]);
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
int cameraSetPerspective(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    lua_Number l_fov,l_near,l_far,l_width,l_height;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_camera,ElementType::CameraElement);
    argStream.ReadNumber(l_fov);
    argStream.ReadNumber(l_width);
    argStream.ReadNumber(l_height);
    argStream.ReadNumber(l_near);
    argStream.ReadNumber(l_far);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    l_camera->SetPerspective(static_cast<float>(l_fov),static_cast<float>(l_width),static_cast<float>(l_height),static_cast<float>(l_near),static_cast<float>(l_far));
    lua_pushboolean(f_vm,1);
    return 1;
}
int cameraSetOrtho(lua_State *f_vm)
{
    Camera *l_camera = NULL;
    lua_Number l_val[6];
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_camera,ElementType::CameraElement);
    for(int i=0; i < 6; i++) argStream.ReadNumber(l_val[i]);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    l_camera->SetOrtho(static_cast<float>(l_val[0]),static_cast<float>(l_val[1]),static_cast<float>(l_val[2]),static_cast<float>(l_val[3]),static_cast<float>(l_val[4]),static_cast<float>(l_val[5]));
    lua_pushboolean(f_vm,1);
    return 1;
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
    for(int i=0; i < 16; i++)
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
    for(int i=0; i < 16; i++)
    {
        lua_pushnumber(f_vm,i+1);
        lua_pushnumber(f_vm,l_matPointer[i]);
        lua_settable(f_vm,-3);
    }
    return 1;
}

}
}
