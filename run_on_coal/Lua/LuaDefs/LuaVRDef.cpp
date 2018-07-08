#include "stdafx.h"

#include "Lua/LuaDefs/LuaVRDef.h"

#include "Core/Core.h"
#include "Managers/LuaManager.h"
#include "Managers/VRManager.h"
#include "Lua/ArgReader.h"

void ROC::LuaVRDef::Init(lua_State *f_vm)
{
    lua_register(f_vm, "vrGetHeadPosition", VRGetHeadPosition);
    lua_register(f_vm, "vrGetHeadRotation", VRGetHeadRotation);
    lua_register(f_vm, "vrGetEyesPosition", VRGetEyesPosition);
}

int ROC::LuaVRDef::VRGetHeadPosition(lua_State *f_vm)
{
    // float float float vrGetHeadPosition()
    ArgReader argStream(f_vm);
    VRManager *l_vrManager = LuaManager::GetCore()->GetVRManager();
    if(l_vrManager)
    {
        const glm::vec3 &l_pos = l_vrManager->GetHeadPosition();
        for(int i = 0; i < 3; i++) argStream.PushNumber(l_pos[i]);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaVRDef::VRGetHeadRotation(lua_State *f_vm)
{
    // float float float float vrGetHeadRotation()
    ArgReader argStream(f_vm);
    VRManager *l_vrManager = LuaManager::GetCore()->GetVRManager();
    if(l_vrManager)
    {
        const glm::quat &l_rot = l_vrManager->GetHeadRotation();
        argStream.PushQuat(l_rot);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaVRDef::VRGetEyesPosition(lua_State *f_vm)
{
    // float float float float float float vrGetEyesPosition()
    ArgReader argStream(f_vm);
    VRManager *l_vrManager = LuaManager::GetCore()->GetVRManager();
    if(l_vrManager)
    {
        const glm::vec3& l_leftEye = l_vrManager->GetLeftEyePosition();
        const glm::vec3& l_rightEye = l_vrManager->GetRightEyePosition();
        for(int i = 0; i < 3; i++) argStream.PushNumber(l_leftEye[i]);
        for(int i = 0; i < 3; i++) argStream.PushNumber(l_rightEye[i]);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
