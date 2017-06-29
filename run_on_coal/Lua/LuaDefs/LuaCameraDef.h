#pragma once

namespace ROC
{

class LuaCameraDef
{
    static int CameraCreate(lua_State *f_vm);
    static int CameraSetPosition(lua_State *f_vm);
    static int CameraGetPosition(lua_State *f_vm);
    static int CameraSetDirection(lua_State *f_vm);
    static int CameraGetDirection(lua_State *f_vm);
    static int CameraSetType(lua_State *f_vm);
    static int CameraGetType(lua_State *f_vm);
    static int CameraSetFOV(lua_State *f_vm);
    static int CameraGetFOV(lua_State *f_vm);
    static int CameraSetAspectRatio(lua_State *f_vm);
    static int CameraGetAspectRatio(lua_State *f_vm);
    static int CameraSetOrthoParams(lua_State *f_vm);
    static int CameraGetOrthoParams(lua_State *f_vm);
    static int CameraSetDepth(lua_State *f_vm);
    static int CameraGetDepth(lua_State *f_vm);
    static int CameraGetViewMatrix(lua_State *f_vm);
    static int CameraGetProjectionMatrix(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaManager;
};

}
