#include "stdafx.h"

#include "Lua/LuaDefs/LuaVRDef.h"

#include "Module/LuaModule.h"
#include "Lua/ArgReader.h"
#include "Utils/EnumUtils.h"
#include "Utils/LuaUtils.h"

const std::vector<std::string> g_VRControllersTable
{
    "left", "right"
};

#define VR_CONTROLLER_LEFT 0
#define VR_CONTROLLER_RIGHT 1

void LuaVRDef::Init(lua_State *f_vm)
{
    lua_register(f_vm, "isVREnabled", IsVREnabled);
    lua_register(f_vm, "vrGetHeadPosition", VRGetHeadPosition);
    lua_register(f_vm, "vrGetHeadRotation", VRGetHeadRotation);
    lua_register(f_vm, "vrGetEyesPosition", VRGetEyesPosition);
    lua_register(f_vm, "vrIsControllerActive", VRIsControllerActive);
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

int LuaVRDef::VRIsControllerActive(lua_State *f_vm)
{
    // bool vrIsControllerActive( string controller )
    ArgReader argStream(f_vm);
    std::string l_controller;
    argStream.ReadText(l_controller);
    if(!argStream.HasErrors() && !l_controller.empty())
    {
        ROC::IVRManager *l_vrManager = LuaModule::GetModule()->GetEngineCore()->GetVRManager();
        switch(EnumUtils::ReadEnumVector(l_controller, g_VRControllersTable))
        {
            case VR_CONTROLLER_LEFT:
                argStream.PushBoolean(l_vrManager->IsLeftControllerActive());
                break;
            case VR_CONTROLLER_RIGHT:
                argStream.PushBoolean(l_vrManager->IsRightControllerActive());
                break;
            default:
                argStream.PushBoolean(false);
                break;
        }
    }
    return argStream.GetReturnValue();
}

int LuaVRDef::VRGetControllerPosition(lua_State *f_vm)
{
    // float float float vrGetControllerPosition( string controller )
    ArgReader argStream(f_vm);
    std::string l_controller;
    argStream.ReadText(l_controller);
    if(!argStream.HasErrors() && !l_controller.empty())
    {
        ROC::IVRManager *l_vrManager = LuaModule::GetModule()->GetEngineCore()->GetVRManager();
        switch(EnumUtils::ReadEnumVector(l_controller, g_VRControllersTable))
        {
            case VR_CONTROLLER_LEFT:
            {
                const glm::vec3 &l_pos = l_vrManager->GetLeftControllerPosition();
                for(int i = 0; i < 3; i++) argStream.PushNumber(l_pos[i]);
            } break;
            case VR_CONTROLLER_RIGHT:
            {
                const glm::vec3 &l_pos = l_vrManager->GetRightControllerPosition();
                for(int i = 0; i < 3; i++) argStream.PushNumber(l_pos[i]);
            } break;
            default:
                argStream.PushBoolean(false);
                break;
        }
    }
    return argStream.GetReturnValue();
}

int LuaVRDef::VRGetControllerRotation(lua_State *f_vm)
{
    // float float float float vrGetControllerRotation( string controller )
    ArgReader argStream(f_vm);
    std::string l_controller;
    argStream.ReadText(l_controller);
    if(!argStream.HasErrors() && !l_controller.empty())
    {
        ROC::IVRManager *l_vrManager = LuaModule::GetModule()->GetEngineCore()->GetVRManager();
        switch(EnumUtils::ReadEnumVector(l_controller, g_VRControllersTable))
        {
            case VR_CONTROLLER_LEFT:
            {
                const glm::quat &l_rot = l_vrManager->GetLeftControllerRotation();
                for(int i = 0; i < 4; i++) argStream.PushNumber(l_rot[i]);
            } break;
            case VR_CONTROLLER_RIGHT:
            {
                const glm::quat &l_rot = l_vrManager->GetRightControllerRotation();
                for(int i = 0; i < 4; i++) argStream.PushNumber(l_rot[i]);
            } break;
            default:
                argStream.PushBoolean(false);
                break;
        }
    }
    return argStream.GetReturnValue();
}

int LuaVRDef::VRGetControllerVelocity(lua_State *f_vm)
{
    // float float float vrGetControllerVelocity( string controller )
    ArgReader argStream(f_vm);
    std::string l_controller;
    argStream.ReadText(l_controller);
    if(!argStream.HasErrors() && !l_controller.empty())
    {
        ROC::IVRManager *l_vrManager = LuaModule::GetModule()->GetEngineCore()->GetVRManager();
        switch(EnumUtils::ReadEnumVector(l_controller, g_VRControllersTable))
        {
            case VR_CONTROLLER_LEFT:
            {
                const glm::vec3 &l_velocity = l_vrManager->GetLeftControllerVelocity();
                for(int i = 0; i < 3; i++) argStream.PushNumber(l_velocity[i]);
            } break;
            case VR_CONTROLLER_RIGHT:
            {
                const glm::vec3 &l_velocity = l_vrManager->GetRightControllerVelocity();
                for(int i = 0; i < 3; i++) argStream.PushNumber(l_velocity[i]);
            } break;
            default:
                argStream.PushBoolean(false);
                break;
        }
    }
    return argStream.GetReturnValue();
}

int LuaVRDef::VRGetControllerAngularVelocity(lua_State *f_vm)
{
    // float float float vrGetControllerAngularVelocity( string controller )
    ArgReader argStream(f_vm);
    std::string l_controller;
    argStream.ReadText(l_controller);
    if(!argStream.HasErrors() && !l_controller.empty())
    {
        ROC::IVRManager *l_vrManager = LuaModule::GetModule()->GetEngineCore()->GetVRManager();
        switch(EnumUtils::ReadEnumVector(l_controller, g_VRControllersTable))
        {
            case VR_CONTROLLER_LEFT:
            {
                const glm::vec3 &l_angVelocity = l_vrManager->GetLeftControllerAngularVelocity();
                for(int i = 0; i < 3; i++) argStream.PushNumber(l_angVelocity[i]);
            } break;
            case VR_CONTROLLER_RIGHT:
            {
                const glm::vec3 &l_angVelocity = l_vrManager->GetRightControllerAngularVelocity();
                for(int i = 0; i < 3; i++) argStream.PushNumber(l_angVelocity[i]);
            } break;
            default:
                argStream.PushBoolean(false);
                break;
        }
    }
    return argStream.GetReturnValue();
}
