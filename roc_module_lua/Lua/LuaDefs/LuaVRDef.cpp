#include "stdafx.h"

#include "Lua/LuaDefs/LuaVRDef.h"

#include "Module/LuaModule.h"
#include "Lua/ArgReader.h"
#include "Utils/EnumUtils.h"
#include "Utils/LuaUtils.h"

const std::vector<std::string> g_VRControllerHandsTable
{
    "none", "left", "right"
};

void LuaVRDef::Init(lua_State *f_vm)
{
    lua_register(f_vm, "isVREnabled", IsVREnabled);
    lua_register(f_vm, "vrGetHeadPosition", VRGetHeadPosition);
    lua_register(f_vm, "vrGetHeadRotation", VRGetHeadRotation);
    lua_register(f_vm, "vrGetEyesPosition", VRGetEyesPosition);
    lua_register(f_vm, "vrIsControllerConnected", VRIsControllerConnected);
    lua_register(f_vm, "vrIsControllerActive", VRIsControllerActive);
    lua_register(f_vm, "vrGetControllerHand", VRGetControllerHand);
    lua_register(f_vm, "vrGetControllerPosition", VRGetControllerPosition);
    lua_register(f_vm, "vrGetControllerRotation", VRGetControllerRotation);
    lua_register(f_vm, "vrGetControllerVelocity", VRGetControllerVelocity);
    lua_register(f_vm, "vrGetControllerAngularVelocity", VRGetControllerAngularVelocity);
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
        argStream.PushText(g_VRControllerHandsTable[static_cast<size_t>(l_hand)]);
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
