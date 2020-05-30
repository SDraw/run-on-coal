#include "stdafx.h"

#include "Lua/LuaDefs/LuaVRDef.h"

#include "Module/LuaModule.h"
#include "Lua/ArgReader.h"
#include "Utils/EnumUtils.h"
#include "Utils/LuaUtils.h"

const std::vector<std::string> g_VRControllerHands
{
    "left", "right", "any"
};
const std::vector<std::string> g_VRSides
{
    "left", "right"
};

void LuaVRDef::Init(lua_State *f_vm)
{
    lua_register(f_vm, "isVREnabled", IsVREnabled);
    lua_register(f_vm, "vrGetHmdPosition", VRGetHmdPosition);
    lua_register(f_vm, "vrGetHmdRotation", VRGetHmdRotation);
    lua_register(f_vm, "vrGetRenderTargetSize", VRGetRenderTargetSize);
    lua_register(f_vm, "vrIsControllerConnected", VRIsControllerConnected);
    lua_register(f_vm, "vrIsControllerActive", VRIsControllerActive);
    lua_register(f_vm, "vrGetControllerHand", VRGetControllerHand);
    lua_register(f_vm, "vrGetControllerFromHand", VRGetControllerFromHand);
    lua_register(f_vm, "vrGetControllerPosition", VRGetControllerPosition);
    lua_register(f_vm, "vrGetControllerRotation", VRGetControllerRotation);
    lua_register(f_vm, "vrGetControllerVelocity", VRGetControllerVelocity);
    lua_register(f_vm, "vrGetControllerAngularVelocity", VRGetControllerAngularVelocity);
    lua_register(f_vm, "vrDrawEyeImage", VRDrawEyeImage);
    lua_register(f_vm, "vrDrawEyeImage3D", VRDrawEyeImage3D);
}

int LuaVRDef::IsVREnabled(lua_State *f_vm)
{
    // bool isVREnabled()
    ArgReader argStream(f_vm);
    argStream.PushBoolean(LuaModule::GetModule()->GetEngineCore()->GetIVRManager()->IsVREnabled());
    return argStream.GetReturnValue();
}

int LuaVRDef::VRGetHmdPosition(lua_State *f_vm)
{
    // float float float VRGetHmdPosition()
    ArgReader argStream(f_vm);
    const glm::vec3 &l_pos = LuaModule::GetModule()->GetEngineCore()->GetIVRManager()->GetHmdPosition();
    for(int i = 0; i < 3; i++) argStream.PushNumber(l_pos[i]);
    return argStream.GetReturnValue();
}

int LuaVRDef::VRGetHmdRotation(lua_State *f_vm)
{
    // float float float float VRGetHmdRotation()
    ArgReader argStream(f_vm);
    const glm::quat &l_rot = LuaModule::GetModule()->GetEngineCore()->GetIVRManager()->GetHmdRotation();
    for(int i = 0; i < 4; i++) argStream.PushNumber(l_rot[i]);
    return argStream.GetReturnValue();
}

int LuaVRDef::VRGetRenderTargetSize(lua_State *f_vm)
{
    // float float vrGetRenderTargetSize()
    ArgReader argStream(f_vm);
    glm::uvec2 l_size = LuaModule::GetModule()->GetEngineCore()->GetIVRManager()->GetTargetsSize();
    for(int i = 0; i < 2; i++) argStream.PushNumber(l_size[i]);
    return argStream.GetReturnValue();
}

int LuaVRDef::VRIsControllerConnected(lua_State *f_vm)
{
    // bool vrIsControllerConnected( int id )
    ArgReader argStream(f_vm);
    unsigned int l_id;
    argStream.ReadInteger(l_id);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaModule::GetModule()->GetEngineCore()->GetIVRManager()->IsControllerConnected(l_id);
        argStream.PushBoolean(l_result);
    }
    return argStream.GetReturnValue();
}

int LuaVRDef::VRIsControllerActive(lua_State *f_vm)
{
    // bool vrIsControllerActive( int id )
    ArgReader argStream(f_vm);
    unsigned int l_id;
    argStream.ReadInteger(l_id);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaModule::GetModule()->GetEngineCore()->GetIVRManager()->IsControllerActive(l_id);
        argStream.PushBoolean(l_result);
    }
    return argStream.GetReturnValue();
}

int LuaVRDef::VRGetControllerHand(lua_State *f_vm)
{
    // string vrGetControllerHand( int id )
    ArgReader argStream(f_vm);
    unsigned int l_id;
    argStream.ReadInteger(l_id);
    if(!argStream.HasErrors())
    {
        unsigned char l_hand = LuaModule::GetModule()->GetEngineCore()->GetIVRManager()->GetControllerHand(l_id);
        argStream.PushText(g_VRControllerHands[l_hand]);
    }
    return argStream.GetReturnValue();
}

int LuaVRDef::VRGetControllerFromHand(lua_State *f_vm)
{
    // int vrGetControllerFromHand(str hand)
    std::string l_hand;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_hand);
    if(!argStream.HasErrors())
    {
        size_t l_handIndex = EnumUtils::ReadEnumVector(l_hand, g_VRControllerHands);
        if(l_handIndex != std::numeric_limits<size_t>::max())
        {
            unsigned int l_controllerIndex = LuaModule::GetModule()->GetEngineCore()->GetIVRManager()->GetControllerFromHand(static_cast<unsigned char>(l_handIndex));
            if(l_controllerIndex != 0xFFU) argStream.PushInteger(l_controllerIndex);
            else argStream.PushBoolean(false);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaVRDef::VRGetControllerPosition(lua_State *f_vm)
{
    // float float float vrGetControllerPosition( int id )
    ArgReader argStream(f_vm);
    unsigned int l_id;
    argStream.ReadInteger(l_id);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_pos;
        if(LuaModule::GetModule()->GetEngineCore()->GetIVRManager()->GetControllerPosition(l_id, l_pos))
        {
            for(int i = 0; i < 3; i++) argStream.PushNumber(l_pos[i]);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaVRDef::VRGetControllerRotation(lua_State *f_vm)
{
    // float float float float vrGetControllerRotation( int id )
    ArgReader argStream(f_vm);
    unsigned int l_id;
    argStream.ReadInteger(l_id);
    if(!argStream.HasErrors())
    {
        glm::quat l_rot;
        if(LuaModule::GetModule()->GetEngineCore()->GetIVRManager()->GetControllerRotation(l_id, l_rot))
        {
            for(int i = 0; i < 4; i++) argStream.PushNumber(l_rot[i]);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaVRDef::VRGetControllerVelocity(lua_State *f_vm)
{
    // float float float vrGetControllerVelocity( int id )
    ArgReader argStream(f_vm);
    unsigned int l_id;
    argStream.ReadInteger(l_id);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_val;
        if(LuaModule::GetModule()->GetEngineCore()->GetIVRManager()->GetControllerVelocity(l_id, l_val))
        {
            for(int i = 0; i < 3; i++) argStream.PushNumber(l_val[i]);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaVRDef::VRGetControllerAngularVelocity(lua_State *f_vm)
{
    // float float float vrGetControllerAngularVelocity( int id )
    ArgReader argStream(f_vm);
    unsigned int l_id;
    argStream.ReadInteger(l_id);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_val;
        if(LuaModule::GetModule()->GetEngineCore()->GetIVRManager()->GetControllerAngularVelocity(l_id, l_val))
        {
            for(int i = 0; i < 3; i++) argStream.PushNumber(l_val[i]);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaVRDef::VRDrawEyeImage(lua_State *f_vm)
{
    std::string l_side;
    glm::vec2 l_pos, l_size;
    float l_rot = 0.f;
    glm::vec4 l_color(1.f);
    std::string l_layer("screen");
    ArgReader argStream(f_vm);
    argStream.ReadText(l_side);
    for(int i = 0; i < 2; i++) argStream.ReadNumber(l_pos[i]);
    for(int i = 0; i < 2; i++) argStream.ReadNumber(l_size[i]);
    argStream.ReadNextNumber(l_rot);
    for(int i = 0; i < 4; i++) argStream.ReadNextNumber(l_color[i]);
    argStream.ReadNextText(l_layer);
    if(!argStream.HasErrors() && !l_side.empty())
    {
        size_t l_sideIndex = EnumUtils::ReadEnumVector(l_side, g_VRSides);
        if(l_sideIndex != std::numeric_limits<size_t>::max())
        {
            bool l_result = LuaModule::GetModule()->GetEngineCore()->GetIVRManager()->DrawEyeImage(static_cast<unsigned char>(l_sideIndex), l_pos, l_size, l_rot, l_color, l_layer);
            argStream.PushBoolean(l_result);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaVRDef::VRDrawEyeImage3D(lua_State *f_vm)
{
    std::string l_side;
    glm::vec3 l_pos;
    glm::vec3 l_rot;
    glm::vec2 l_size;
    std::string l_layer("default");
    glm::bvec4 l_params(true, true, false, false);
    ArgReader argStream(f_vm);
    argStream.ReadText(l_side);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_pos[i]);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_rot[i]);
    for(int i = 0; i < 2; i++) argStream.ReadNumber(l_size[i]);
    argStream.ReadNextText(l_layer);
    for(int i = 0; i < 4; i++) argStream.ReadNextBoolean(l_params[i]);
    if(!argStream.HasErrors())
    {
        size_t l_sideIndex = EnumUtils::ReadEnumVector(l_side, g_VRSides);
        if(l_sideIndex != std::numeric_limits<size_t>::max())
        {
            glm::quat l_rotQuat(l_rot);
            bool l_result = LuaModule::GetModule()->GetEngineCore()->GetIVRManager()->DrawEyeImage(static_cast<unsigned char>(l_sideIndex), l_pos, l_rot, l_size, l_layer, l_params);
            argStream.PushBoolean(l_result);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
