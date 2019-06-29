#pragma once

class LuaVRDef final
{
    static int IsVREnabled(lua_State *f_vm);
    static int VRGetHeadPosition(lua_State *f_vm);
    static int VRGetHeadRotation(lua_State *f_vm);
    static int VRGetEyesPosition(lua_State *f_vm);
    static int VRGetEyesImageSize(lua_State *f_vm);
    static int VRIsControllerConnected(lua_State *f_vm);
    static int VRIsControllerActive(lua_State *f_vm);
    static int VRGetControllerHand(lua_State *f_vm);
    static int VRGetControllerPosition(lua_State *f_vm);
    static int VRGetControllerRotation(lua_State *f_vm);
    static int VRGetControllerVelocity(lua_State *f_vm);
    static int VRGetControllerAngularVelocity(lua_State *f_vm);
    static int VRShowNotification(lua_State *f_vm);
    static int VRDrawEyeImage(lua_State *f_vm);
    static int VRDrawEyeImage3D(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaVM;

};
