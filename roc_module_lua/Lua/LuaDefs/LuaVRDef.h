#pragma once

class LuaVRDef final
{
    static int IsVREnabled(lua_State *f_vm);

    static int VRIsDeviceConnected(lua_State *f_vm);
    static int VRIsDeviceActive(lua_State *f_vm);
    static int VRGetDevicePosition(lua_State *f_vm);
    static int VRGetDeviceRotation(lua_State *f_vm);
    static int VRGetDeviceVelocity(lua_State *f_vm);
    static int VRGetDeviceAngularVelocity(lua_State *f_vm);
    static int VRGetDeviceType(lua_State *f_vm);
    static int VRGetDeviceHandRole(lua_State *f_vm);
    static int VRGetDeviceByHandRole(lua_State *f_vm);
    static int VRGetRenderSize(lua_State *f_vm);
    static int VRDrawEyeImage(lua_State *f_vm);
    static int VRDrawEyeImage3D(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaVM;

};
