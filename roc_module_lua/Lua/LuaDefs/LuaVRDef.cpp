#include "stdafx.h"

#include "Lua/LuaDefs/LuaVRDef.h"

#include "Module/LuaModule.h"
#include "Lua/ArgReader.h"
#include "Utils/EnumUtils.h"
#include "Utils/LuaUtils.h"

const std::vector<std::string> g_VRControllerHands
{
    "none", "left", "right"
};
const std::vector<std::string> g_VREyes
{
    "left", "right"
};

void LuaVRDef::Init(lua_State *f_vm)
{
    lua_register(f_vm, "isVREnabled", IsVREnabled);
    lua_register(f_vm, "vrGetHeadPosition", VRGetHeadPosition);
    lua_register(f_vm, "vrGetHeadRotation", VRGetHeadRotation);
    lua_register(f_vm, "vrGetEyesPosition", VRGetEyesPosition);
    lua_register(f_vm, "vrGetEyesImageSize", VRGetEyesImageSize);
    lua_register(f_vm, "vrIsControllerConnected", VRIsControllerConnected);
    lua_register(f_vm, "vrIsControllerActive", VRIsControllerActive);
    lua_register(f_vm, "vrGetControllerHand", VRGetControllerHand);
    lua_register(f_vm, "vrGetControllerPosition", VRGetControllerPosition);
    lua_register(f_vm, "vrGetControllerRotation", VRGetControllerRotation);
    lua_register(f_vm, "vrGetControllerVelocity", VRGetControllerVelocity);
    lua_register(f_vm, "vrGetControllerAngularVelocity", VRGetControllerAngularVelocity);
    lua_register(f_vm, "vrShowNotification", VRShowNotification);
    lua_register(f_vm, "vrDrawEyeImage", VRDrawEyeImage);
    lua_register(f_vm, "vrDrawEyeImage3D", VRDrawEyeImage3D);
}

int LuaVRDef::IsVREnabled(lua_State *f_vm)
{
    // bool isVREnabled()
    ArgReader argStream(f_vm);
    argStream.PushBoolean(LuaModule::GetModule()->GetEngineCore()->GetVRManager()->IsVREnabled());
    return argStream.GetReturnValue();
}

int LuaVRDef::VRGetHeadPosition(lua_State *f_vm)
{
    // float float float vrGetHeadPosition()
    ArgReader argStream(f_vm);
    const glm::vec3 &l_pos = LuaModule::GetModule()->GetEngineCore()->GetVRManager()->GetHeadPosition();
    for(int i = 0; i < 3; i++) argStream.PushNumber(l_pos[i]);
    return argStream.GetReturnValue();
}

int LuaVRDef::VRGetHeadRotation(lua_State *f_vm)
{
    // float float float float vrGetHeadRotation()
    ArgReader argStream(f_vm);
    const glm::quat &l_rot = LuaModule::GetModule()->GetEngineCore()->GetVRManager()->GetHeadRotation();
    for(int i = 0; i < 4; i++) argStream.PushNumber(l_rot[i]);
    return argStream.GetReturnValue();
}

int LuaVRDef::VRGetEyesPosition(lua_State *f_vm)
{
    // float float float float float float vrGetEyesPosition()
    ArgReader argStream(f_vm);
    ROC::IVRManager *l_vrManager = LuaModule::GetModule()->GetEngineCore()->GetVRManager();
    const glm::vec3& l_leftEye = l_vrManager->GetLeftEyePosition();
    const glm::vec3& l_rightEye = l_vrManager->GetRightEyePosition();
    for(int i = 0; i < 3; i++) argStream.PushNumber(l_leftEye[i]);
    for(int i = 0; i < 3; i++) argStream.PushNumber(l_rightEye[i]);
    return argStream.GetReturnValue();
}

int LuaVRDef::VRGetEyesImageSize(lua_State *f_vm)
{
    // float float vrGetEyesPosition()
    ArgReader argStream(f_vm);
    glm::uvec2 l_size = LuaModule::GetModule()->GetEngineCore()->GetVRManager()->GetTargetsSize();
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
        bool l_result = LuaModule::GetModule()->GetEngineCore()->GetVRManager()->IsControllerConnected(l_id);
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
        bool l_result = LuaModule::GetModule()->GetEngineCore()->GetVRManager()->IsControllerActive(l_id);
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
        unsigned char l_hand = LuaModule::GetModule()->GetEngineCore()->GetVRManager()->GetControllerHandAssignment(l_id);
        argStream.PushText(g_VRControllerHands[static_cast<size_t>(l_hand)]);
    }
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
        if(LuaModule::GetModule()->GetEngineCore()->GetVRManager()->GetControllerPosition(l_id, l_pos))
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
        if(LuaModule::GetModule()->GetEngineCore()->GetVRManager()->GetControllerRotation(l_id, l_rot))
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
        if(LuaModule::GetModule()->GetEngineCore()->GetVRManager()->GetControllerVelocity(l_id, l_val))
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
        if(LuaModule::GetModule()->GetEngineCore()->GetVRManager()->GetControllerAngularVelocity(l_id, l_val))
        {
            for(int i = 0; i < 3; i++) argStream.PushNumber(l_val[i]);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaVRDef::VRShowNotification(lua_State *f_vm)
{
    std::string l_title;
    std::string l_text;
    unsigned int l_time;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_title);
    argStream.ReadText(l_text);
    argStream.ReadInteger(l_time);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaModule::GetModule()->GetEngineCore()->GetVRManager()->ShowNotification(l_title, l_text, l_time);
        argStream.PushBoolean(l_result);
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
    ArgReader argStream(f_vm);
    argStream.ReadText(l_side);
    for(int i = 0; i < 2; i++) argStream.ReadNumber(l_pos[i]);
    for(int i = 0; i < 2; i++) argStream.ReadNumber(l_size[i]);
    argStream.ReadNextNumber(l_rot);
    for(int i = 0; i < 4; i++) argStream.ReadNextNumber(l_color[i]);
    if(!argStream.HasErrors() && !l_side.empty())
    {
        int l_sideIndex = EnumUtils::ReadEnumVector(l_side, g_VREyes);
        if(l_sideIndex != -1)
        {
            bool l_result = LuaModule::GetModule()->GetEngineCore()->GetVRManager()->DrawEyeImage(static_cast<unsigned char>(l_sideIndex), l_pos, l_size, l_rot, l_color);
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
    glm::bvec4 l_params(true, true, false, false);
    ArgReader argStream(f_vm);
    argStream.ReadText(l_side);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_pos[i]);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_rot[i]);
    for(int i = 0; i < 2; i++) argStream.ReadNumber(l_size[i]);
    for(int i = 0; i < 4; i++) argStream.ReadNextBoolean(l_params[i]);
    if(!argStream.HasErrors())
    {
        int l_sideIndex = EnumUtils::ReadEnumVector(l_side, g_VREyes);
        if(l_sideIndex != -1)
        {
            glm::quat l_rotQuat(l_rot);
            bool l_result = LuaModule::GetModule()->GetEngineCore()->GetVRManager()->DrawEyeImage(static_cast<unsigned char>(l_sideIndex), l_pos, l_rot, l_size, l_params);
            argStream.PushBoolean(l_result);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
