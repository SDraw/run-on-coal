#include "stdafx.h"

#include "Lua/LuaDefs/LuaVRDef.h"

#include "Core/Core.h"
#include "Managers/LuaManager.h"
#include "Managers/VRManager.h"
#include "Lua/ArgReader.h"
#include "Utils/EnumUtils.h"

namespace ROC
{

const std::vector<std::string> g_VRControllersTable
{
    "left", "right"
};

}

#define ROC_VR_CONTROLLER_LEFT 0U
#define ROC_VR_CONTROLLER_RIGHT 1U

void ROC::LuaVRDef::Init(lua_State *f_vm)
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

int ROC::LuaVRDef::IsVREnabled(lua_State *f_vm)
{
    // bool isVREnabled()
    ArgReader argStream(f_vm);
    argStream.PushBoolean(Core::GetCore()->GetVRManager()->IsVREnabled());
    return argStream.GetReturnValue();
}

int ROC::LuaVRDef::VRGetHeadPosition(lua_State *f_vm)
{
    // float float float vrGetHeadPosition()
    ArgReader argStream(f_vm);
    const glm::vec3 &l_pos = Core::GetCore()->GetVRManager()->GetHeadPosition();
    for(int i = 0; i < 3; i++) argStream.PushNumber(l_pos[i]);
    return argStream.GetReturnValue();
}

int ROC::LuaVRDef::VRGetHeadRotation(lua_State *f_vm)
{
    // float float float float vrGetHeadRotation()
    ArgReader argStream(f_vm);
    const glm::quat &l_rot = Core::GetCore()->GetVRManager()->GetHeadRotation();
    for(int i = 0; i < 4; i++) argStream.PushNumber(l_rot[i]);
    return argStream.GetReturnValue();
}

int ROC::LuaVRDef::VRGetEyesPosition(lua_State *f_vm)
{
    // float float float float float float vrGetEyesPosition()
    ArgReader argStream(f_vm);
    VRManager *l_vrManager = Core::GetCore()->GetVRManager();
    const glm::vec3& l_leftEye = l_vrManager->GetLeftEyePosition();
    const glm::vec3& l_rightEye = l_vrManager->GetRightEyePosition();
    for(int i = 0; i < 3; i++) argStream.PushNumber(l_leftEye[i]);
    for(int i = 0; i < 3; i++) argStream.PushNumber(l_rightEye[i]);
    return argStream.GetReturnValue();
}

int ROC::LuaVRDef::VRIsControllerActive(lua_State *f_vm)
{
    // bool vrIsControllerActive( string controller )
    ArgReader argStream(f_vm);
    std::string l_controller;
    argStream.ReadText(l_controller);
    if(!argStream.HasErrors() && !l_controller.empty())
    {
        VRManager *l_vrManager = Core::GetCore()->GetVRManager();
        switch(EnumUtils::ReadEnumVector(l_controller, g_VRControllersTable))
        {
            case ROC_VR_CONTROLLER_LEFT:
                argStream.PushBoolean(l_vrManager->IsLeftControllerActive());
                break;
            case ROC_VR_CONTROLLER_RIGHT:
                argStream.PushBoolean(l_vrManager->IsRightControllerActive());
                break;
            default:
                argStream.PushBoolean(false);
                break;
        }
    }
    return argStream.GetReturnValue();
}

int ROC::LuaVRDef::VRGetControllerPosition(lua_State *f_vm)
{
    // float float float vrGetControllerPosition( string controller )
    ArgReader argStream(f_vm);
    std::string l_controller;
    argStream.ReadText(l_controller);
    if(!argStream.HasErrors() && !l_controller.empty())
    {
        VRManager *l_vrManager = Core::GetCore()->GetVRManager();
        switch(EnumUtils::ReadEnumVector(l_controller, g_VRControllersTable))
        {
            case ROC_VR_CONTROLLER_LEFT:
            {
                const glm::vec3 &l_pos = l_vrManager->GetLeftControllerPosition();
                for(int i = 0; i < 3; i++) argStream.PushNumber(l_pos[i]);
            } break;
            case ROC_VR_CONTROLLER_RIGHT:
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

int ROC::LuaVRDef::VRGetControllerRotation(lua_State *f_vm)
{
    // float float float float vrGetControllerRotation( string controller )
    ArgReader argStream(f_vm);
    std::string l_controller;
    argStream.ReadText(l_controller);
    if(!argStream.HasErrors() && !l_controller.empty())
    {
        VRManager *l_vrManager = Core::GetCore()->GetVRManager();
        switch(EnumUtils::ReadEnumVector(l_controller, g_VRControllersTable))
        {
            case ROC_VR_CONTROLLER_LEFT:
            {
                const glm::quat &l_rot = l_vrManager->GetLeftControllerRotation();
                for(int i = 0; i < 4; i++) argStream.PushNumber(l_rot[i]);
            } break;
            case ROC_VR_CONTROLLER_RIGHT:
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

int ROC::LuaVRDef::VRGetControllerVelocity(lua_State *f_vm)
{
    // float float float vrGetControllerVelocity( string controller )
    ArgReader argStream(f_vm);
    std::string l_controller;
    argStream.ReadText(l_controller);
    if(!argStream.HasErrors() && !l_controller.empty())
    {
        VRManager *l_vrManager = Core::GetCore()->GetVRManager();
        switch(EnumUtils::ReadEnumVector(l_controller, g_VRControllersTable))
        {
            case ROC_VR_CONTROLLER_LEFT:
            {
                const glm::vec3 &l_velocity = l_vrManager->GetLeftControllerVelocity();
                for(int i = 0; i < 3; i++) argStream.PushNumber(l_velocity[i]);
            } break;
            case ROC_VR_CONTROLLER_RIGHT:
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

int ROC::LuaVRDef::VRGetControllerAngularVelocity(lua_State *f_vm)
{
    // float float float vrGetControllerAngularVelocity( string controller )
    ArgReader argStream(f_vm);
    std::string l_controller;
    argStream.ReadText(l_controller);
    if(!argStream.HasErrors() && !l_controller.empty())
    {
        VRManager *l_vrManager = Core::GetCore()->GetVRManager();
        switch(EnumUtils::ReadEnumVector(l_controller, g_VRControllersTable))
        {
            case ROC_VR_CONTROLLER_LEFT:
            {
                const glm::vec3 &l_angVelocity = l_vrManager->GetLeftControllerAngularVelocity();
                for(int i = 0; i < 3; i++) argStream.PushNumber(l_angVelocity[i]);
            } break;
            case ROC_VR_CONTROLLER_RIGHT:
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
