#include "stdafx.h"

#include "Lua/LuaDefs/LuaVRDef.h"

#include "Core/Core.h"
#include "Lua/LuaArgReader.h"
#include "Utils/EnumUtils.h"
#include "Utils/LuaUtils.h"

const std::vector<std::string> g_vrSides
{
    "left", "right"
};
const std::string g_deviceTypes[]
{
    "hmd", "controller", "tracker", "base_station", "display_redirect"
};
const std::vector<std::string> g_deviceHandRoles
{
    "left", "right", "opt_out", "treadmill", "stylus"
};

void LuaVRDef::Init(lua_State *f_vm)
{
    lua_register(f_vm, "isVREnabled", IsVREnabled);
    lua_register(f_vm, "vrIsDeviceConnected", VRIsDeviceConnected);
    lua_register(f_vm, "vrIsDeviceActive", VRIsDeviceActive);
    lua_register(f_vm, "vrGetDevicePosition", VRGetDevicePosition);
    lua_register(f_vm, "vrGetDeviceRotation", VRGetDeviceRotation);
    lua_register(f_vm, "vrGetDeviceVelocity", VRGetDeviceVelocity);
    lua_register(f_vm, "vrGetDeviceAngularVelocity", VRGetDeviceAngularVelocity);
    lua_register(f_vm, "vrGetDeviceType", VRGetDeviceType);
    lua_register(f_vm, "vrGetDeviceHandRole", VRGetDeviceHandRole);
    lua_register(f_vm, "vrGetDeviceByHandRole", VRGetDeviceByHandRole);
    lua_register(f_vm, "vrGetRenderSize", VRGetRenderSize);
    lua_register(f_vm, "vrDrawEyeImage", VRDrawEyeImage);
    lua_register(f_vm, "vrDrawEyeImage3D", VRDrawEyeImage3D);
}

int LuaVRDef::IsVREnabled(lua_State *f_vm)
{
    // bool isVREnabled()
    LuaArgReader l_argStream(f_vm);
    l_argStream.PushBoolean(Core::GetInstance()->GetEngineICore()->GetIVRManager()->IsVREnabled());
    return l_argStream.GetReturnValue();
}

int LuaVRDef::VRIsDeviceConnected(lua_State *f_vm)
{
    // bool vrIsDeviceConnected( int id )
    LuaArgReader l_argStream(f_vm);
    unsigned int l_id;
    l_argStream.ReadInteger(l_id);
    if(!l_argStream.HasErrors())
    {
        bool l_result = Core::GetInstance()->GetEngineICore()->GetIVRManager()->IsDeviceConnected(l_id);
        l_argStream.PushBoolean(l_result);
    }
    return l_argStream.GetReturnValue();
}

int LuaVRDef::VRIsDeviceActive(lua_State *f_vm)
{
    // bool vrIsDeviceActive( int id )
    LuaArgReader l_argStream(f_vm);
    unsigned int l_id;
    l_argStream.ReadInteger(l_id);
    if(!l_argStream.HasErrors())
    {
        bool l_result = Core::GetInstance()->GetEngineICore()->GetIVRManager()->IsDeviceActive(l_id);
        l_argStream.PushBoolean(l_result);
    }
    return l_argStream.GetReturnValue();
}

int LuaVRDef::VRGetDevicePosition(lua_State *f_vm)
{
    // float float float vrGetDevicePosition(int index)
    unsigned int l_device;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadInteger(l_device);
    if(!l_argStream.HasErrors())
    {
        glm::vec3 l_pos;
        if(Core::GetInstance()->GetEngineICore()->GetIVRManager()->GetDevicePosition(l_device, l_pos))
        {
            for(int i = 0; i < 3; i++) l_argStream.PushNumber(l_pos[i]);
        }
        else l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaVRDef::VRGetDeviceRotation(lua_State *f_vm)
{
    // float float float float vrGetDeviceRotation(int index)
    unsigned int l_device;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadInteger(l_device);
    if(!l_argStream.HasErrors())
    {
        glm::quat l_rot;
        if(Core::GetInstance()->GetEngineICore()->GetIVRManager()->GetDeviceRotation(l_device, l_rot))
        {
            for(int i = 0; i < 4; i++) l_argStream.PushNumber(l_rot[i]);
        }
        else l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaVRDef::VRGetDeviceVelocity(lua_State *f_vm)
{
    // float float float vrGetDeviceVelocity(int index)
    unsigned int l_device;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadInteger(l_device);
    if(!l_argStream.HasErrors())
    {
        glm::vec3 l_vel;
        if(Core::GetInstance()->GetEngineICore()->GetIVRManager()->GetDeviceVelocity(l_device, l_vel))
        {
            for(int i = 0; i < 3; i++) l_argStream.PushNumber(l_vel[i]);
        }
        else l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaVRDef::VRGetDeviceAngularVelocity(lua_State *f_vm)
{
    // float float float vrGetDeviceAngularVelocity(int index)
    unsigned int l_device;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadInteger(l_device);
    if(!l_argStream.HasErrors())
    {
        glm::vec3 l_vel;
        if(Core::GetInstance()->GetEngineICore()->GetIVRManager()->GetDeviceAngularVelocity(l_device, l_vel))
        {
            for(int i = 0; i < 3; i++) l_argStream.PushNumber(l_vel[i]);
        }
        else l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaVRDef::VRGetDeviceType(lua_State *f_vm)
{
    // str vrGetDeviceType(int index)
    unsigned int l_device;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadInteger(l_device);
    if(!l_argStream.HasErrors())
    {
        unsigned char l_type;
        if(Core::GetInstance()->GetEngineICore()->GetIVRManager()->GetDeviceType(l_device, l_type))
        {
            l_argStream.PushText(g_deviceTypes[l_type]);
        }
        else l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaVRDef::VRGetDeviceHandRole(lua_State *f_vm)
{
    // str vrGetDeviceHandRole(int index)
    unsigned int l_device;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadInteger(l_device);
    if(!l_argStream.HasErrors())
    {
        unsigned char l_role;
        if(Core::GetInstance()->GetEngineICore()->GetIVRManager()->GetDeviceHandRole(l_device, l_role))
        {
            l_argStream.PushText(g_deviceHandRoles[l_role]);
        }
        else l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaVRDef::VRGetDeviceByHandRole(lua_State *f_vm)
{
    // int vrGetDeviceByHandRole(str role)
    std::string l_role;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadText(l_role);
    if(!l_argStream.HasErrors())
    {
        size_t l_handIndex = EnumUtils::ReadEnumVector(l_role, g_deviceHandRoles);
        if(l_handIndex != std::numeric_limits<size_t>::max())
        {
            unsigned int l_device;
            if(Core::GetInstance()->GetEngineICore()->GetIVRManager()->GetDeviceByHandRole(static_cast<unsigned char>(l_handIndex), l_device))
            {
                l_argStream.PushInteger(l_device);
            }
            else l_argStream.PushBoolean(false);
        }
        else l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaVRDef::VRGetRenderSize(lua_State *f_vm)
{
    // int int vrGetRenderSize()
    LuaArgReader l_argStream(f_vm);
    glm::uvec2 l_size = Core::GetInstance()->GetEngineICore()->GetIVRManager()->GetTargetsSize();
    for(int i = 0; i < 2; i++) l_argStream.PushInteger(l_size[i]);
    return l_argStream.GetReturnValue();
}

int LuaVRDef::VRDrawEyeImage(lua_State *f_vm)
{
    // vrDrawEyeImage(str side, float x, float y, float width, float height [, float rot = 0, float r = 1, float g = 1, float b = 1, float a = 1])
    std::string l_side;
    glm::vec2 l_pos, l_size;
    float l_rot = 0.f;
    glm::vec4 l_color(1.f);
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadText(l_side);
    for(int i = 0; i < 2; i++) l_argStream.ReadNumber(l_pos[i]);
    for(int i = 0; i < 2; i++) l_argStream.ReadNumber(l_size[i]);
    l_argStream.ReadNextNumber(l_rot);
    for(int i = 0; i < 4; i++) l_argStream.ReadNextNumber(l_color[i]);
    if(!l_argStream.HasErrors() && !l_side.empty())
    {
        size_t l_sideIndex = EnumUtils::ReadEnumVector(l_side, g_vrSides);
        if(l_sideIndex != std::numeric_limits<size_t>::max())
        {
            bool l_result = Core::GetInstance()->GetEngineICore()->GetIVRManager()->DrawEyeImage(static_cast<unsigned char>(l_sideIndex), l_pos, l_size, l_rot, l_color);
            l_argStream.PushBoolean(l_result);
        }
        else l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaVRDef::VRDrawEyeImage3D(lua_State *f_vm)
{
    // bool vrDrawEyeImage3D(str side, float x, float y, float z, float rx, float ry, float rz, float width, float height
    // [, bool shaded = true, bool depth = true, bool transparent = false, bool doublesided = false])
    std::string l_side;
    glm::vec3 l_pos;
    glm::vec3 l_rot;
    glm::vec2 l_size;
    glm::bvec4 l_params(true, true, false, false);
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadText(l_side);
    for(int i = 0; i < 3; i++) l_argStream.ReadNumber(l_pos[i]);
    for(int i = 0; i < 3; i++) l_argStream.ReadNumber(l_rot[i]);
    for(int i = 0; i < 2; i++) l_argStream.ReadNumber(l_size[i]);
    for(int i = 0; i < 4; i++) l_argStream.ReadNextBoolean(l_params[i]);
    if(!l_argStream.HasErrors())
    {
        size_t l_sideIndex = EnumUtils::ReadEnumVector(l_side, g_vrSides);
        if(l_sideIndex != std::numeric_limits<size_t>::max())
        {
            glm::quat l_rotQuat(l_rot);
            bool l_result = Core::GetInstance()->GetEngineICore()->GetIVRManager()->DrawEyeImage(static_cast<unsigned char>(l_sideIndex), l_pos, l_rot, l_size, l_params);
            l_argStream.PushBoolean(l_result);
        }
        else l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}
