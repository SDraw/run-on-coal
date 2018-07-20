#include "stdafx.h"

#include "Managers/VRManager.h"

#include "Core/Core.h"
#include "Elements/RenderTarget.h"
#include "Lua/LuaArguments.h"

#include "Managers/LuaManager.h"
#include "Managers/EventManager.h"
#include "Elements/Camera.h"
#include "Utils/MathUtils.h"

namespace ROC
{

extern const glm::mat4 g_IdentityMatrix;
extern const glm::vec3 g_EmptyVec3;
extern const glm::quat g_DefaultRotation;

const std::vector<std::pair<uint64_t, std::string>> g_VRControllerButtons
{
    { vr::ButtonMaskFromId(vr::k_EButton_System), "system" },
    { vr::ButtonMaskFromId(vr::k_EButton_ApplicationMenu), "appMenu" },
    { vr::ButtonMaskFromId(vr::k_EButton_Grip), "grip" },
    { vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad), "touchpad" },
    { vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Trigger), "trigger" },
    { vr::ButtonMaskFromId(vr::k_EButton_DPad_Left), "dpad_left" },
    { vr::ButtonMaskFromId(vr::k_EButton_DPad_Up), "dpad_up" },
    { vr::ButtonMaskFromId(vr::k_EButton_DPad_Right), "dpad_right" },
    { vr::ButtonMaskFromId(vr::k_EButton_DPad_Down), "dpad_down" },
    { vr::ButtonMaskFromId(vr::k_EButton_A), "a" }
};

}

ROC::VRManager::VRManager(Core *f_core)
{
    m_core = f_core;

    m_transform = g_IdentityMatrix;
    m_headPosition = g_EmptyVec3;
    m_headRotation = g_DefaultRotation;
    m_leftEyePosition = g_EmptyVec3;
    m_rightEyePosition = g_EmptyVec3;

    vr::HmdError l_hmdError = vr::EVRInitError::VRInitError_None;
    m_vrSystem = vr::VR_Init(&l_hmdError, vr::EVRApplicationType::VRApplication_Scene);
    if(l_hmdError != vr::EVRInitError::VRInitError_None)
    {
        MessageBoxA(NULL, "Unable to start application in VR mode", NULL, MB_OK | MB_ICONEXCLAMATION);
        exit(EXIT_FAILURE);
    }
    Camera::SetVRSystem(m_vrSystem);

    m_vrCompositor = vr::VRCompositor();
    if(!m_vrCompositor)
    {
        MessageBoxA(NULL, "Unable to initialize SteamVR compositor", NULL, MB_OK | MB_ICONEXCLAMATION);
        exit(EXIT_FAILURE);
    }

    m_vrSystem->GetRecommendedRenderTargetSize(&m_targetSize.x, &m_targetSize.y);
    m_leftEyeRT = new RenderTarget();
    m_leftEyeRT->Create(RenderTarget::RTT_RGB, m_targetSize, Drawable::DFT_Linear);
    m_rightEyeRT = new RenderTarget();
    m_rightEyeRT->Create(RenderTarget::RTT_RGB, m_targetSize, Drawable::DFT_Linear);

    m_vrTexture[0] = { UIntToPtr(m_leftEyeRT->GetTextureID()), vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
    m_vrTexture[1] = { UIntToPtr(m_rightEyeRT->GetTextureID()), vr::TextureType_OpenGL, vr::ColorSpace_Gamma };

    m_vrStage = VRS_None;
    m_leftController = { g_EmptyVec3, g_DefaultRotation, g_EmptyVec3, g_EmptyVec3, { 0U }, { 0U }, false };
    m_rightController = { g_EmptyVec3, g_DefaultRotation, g_EmptyVec3, g_EmptyVec3, { 0U }, { 0U }, false };
    m_event = { 0 };
    m_quitState = false;

    m_luaArguments = new LuaArguments();
}
ROC::VRManager::~VRManager()
{
    vr::VR_Shutdown();
    delete m_leftEyeRT;
    delete m_rightEyeRT;
    delete m_luaArguments;
}

void ROC::VRManager::EnableRenderTarget()
{
    switch(m_vrStage)
    {
        case VRS_Left:
            m_leftEyeRT->Enable();
            break;
        case VRS_Right:
            m_rightEyeRT->Enable();
            break;
    }
}
void ROC::VRManager::DisableRenderTarget()
{
    switch(m_vrStage)
    {
        case VRS_Left:
            m_leftEyeRT->Disable();
            break;
        case VRS_Right:
            m_rightEyeRT->Disable();
            break;
    }
}

bool ROC::VRManager::DoPulse()
{
    // Poll events
    while(m_vrSystem->PollNextEvent(&m_event, sizeof(vr::VREvent_t)))
    {
        switch(m_event.eventType)
        {
            case vr::VREvent_DriverRequestedQuit: case vr::VREvent_Quit:
                m_quitState = true;
                break;
        }
    }

    if(!m_quitState)
    {
        // Update HMD data
        m_vrCompositor->WaitGetPoses(m_trackedPoses, vr::k_unMaxTrackedDeviceCount, nullptr, 0);
        m_vrSystem->GetDeviceToAbsoluteTrackingPose(vr::ETrackingUniverseOrigin::TrackingUniverseStanding, 0.f, m_trackedPoses, vr::k_unMaxTrackedDeviceCount);

        const vr::TrackedDevicePose_t &l_hmdPose = m_trackedPoses[vr::k_unTrackedDeviceIndex_Hmd];
        if(l_hmdPose.bPoseIsValid)
        {
            MathUtils::ExtractMatrix(l_hmdPose.mDeviceToAbsoluteTracking, m_transform);
            btTransform l_transform;
            l_transform.setFromOpenGLMatrix(glm::value_ptr(m_transform));

            btVector3 &l_origin = l_transform.getOrigin();
            std::memcpy(&m_headPosition, l_origin.m_floats, sizeof(glm::vec3));
            btQuaternion l_rotation = l_transform.getRotation();
            for(int i = 0; i < 4; i++) m_headRotation[i] = l_rotation[i];

            vr::HmdMatrix34_t l_eyeTransform = m_vrSystem->GetEyeToHeadTransform(vr::Eye_Left);
            MathUtils::ExtractMatrix(l_eyeTransform, m_transform);
            l_transform.setFromOpenGLMatrix(glm::value_ptr(m_transform));
            std::memcpy(&m_leftEyePosition, l_origin.m_floats, sizeof(glm::vec3));

            l_eyeTransform = m_vrSystem->GetEyeToHeadTransform(vr::Eye_Right);
            MathUtils::ExtractMatrix(l_eyeTransform, m_transform);
            l_transform.setFromOpenGLMatrix(glm::value_ptr(m_transform));
            std::memcpy(&m_rightEyePosition, l_origin.m_floats, sizeof(glm::vec3));
        }

        // Update controllers
        m_leftController.m_updated = false;
        m_rightController.m_updated = false;
        for(vr::TrackedDeviceIndex_t i = vr::k_unTrackedDeviceIndex_Hmd + 1U; i < vr::k_unMaxTrackedDeviceCount; i++)
        {
            if(m_vrSystem->IsTrackedDeviceConnected(i))
            {
                if((m_vrSystem->GetTrackedDeviceClass(i) == vr::TrackedDeviceClass_Controller) && m_trackedPoses[i].bPoseIsValid)
                {
                    switch(m_vrSystem->GetControllerRoleForTrackedDeviceIndex(i))
                    {
                        case vr::TrackedControllerRole_LeftHand:
                        {
                            UpdateControllerPose(m_leftController, m_trackedPoses[i]);
                            m_vrSystem->GetControllerState(i, &m_leftController.m_newState, sizeof(vr::VRControllerState_t));
                            UpdateControllerInput(m_leftController, "left");
                        } break;
                        case vr::TrackedControllerRole_RightHand:
                        {
                            UpdateControllerPose(m_rightController, m_trackedPoses[i]);
                            m_vrSystem->GetControllerState(i, &m_rightController.m_newState, sizeof(vr::VRControllerState_t));
                            UpdateControllerInput(m_rightController, "right");
                        } break;
                    }
                }
            }
        }
    }

    return m_quitState;
}

void ROC::VRManager::UpdateControllerPose(VRController &f_controller, const vr::TrackedDevicePose_t &f_pose)
{
    if(!f_controller.m_updated)
    {
        btTransform l_transform;
        MathUtils::ExtractMatrix(f_pose.mDeviceToAbsoluteTracking, m_transform);
        l_transform.setFromOpenGLMatrix(glm::value_ptr(m_transform));

        std::memcpy(&f_controller.m_position, l_transform.getOrigin().m_floats, sizeof(glm::vec3));

        btQuaternion l_rotation = l_transform.getRotation();
        for(int i = 0; i < 4; i++) f_controller.m_rotation[i] = l_rotation[i];

        std::memcpy(&f_controller.m_velocity, f_pose.vVelocity.v, sizeof(glm::vec3));
        std::memcpy(&f_controller.m_angularVelocity, f_pose.vAngularVelocity.v, sizeof(glm::vec3));

        f_controller.m_updated = true;
    }
}
void ROC::VRManager::UpdateControllerInput(VRController &f_controller, const std::string &f_hand)
{
    if(f_controller.m_updated)
    {
        vr::VRControllerState_t &l_oldState = f_controller.m_oldState;
        vr::VRControllerState_t &l_newState = f_controller.m_newState;

        // Update buttons press
        for(const auto &iter : g_VRControllerButtons)
        {
            if((iter.first & l_newState.ulButtonPressed) != (iter.first & l_oldState.ulButtonPressed))
            {
                bool l_pressState = ((iter.first & l_newState.ulButtonPressed) != 0U);
                m_luaArguments->PushArgument(f_hand);
                m_luaArguments->PushArgument(iter.second);
                m_luaArguments->PushArgument(l_pressState ? 1 : 0);
                m_core->GetLuaManager()->GetEventManager()->CallEvent("onVRControllerKeyPress", m_luaArguments);
                m_luaArguments->Clear();
            }
        }

        // Update buttons touch
        for(const auto &iter : g_VRControllerButtons)
        {
            if((iter.first & l_newState.ulButtonTouched) != (iter.first & l_oldState.ulButtonTouched))
            {
                bool l_touchState = ((iter.first & l_newState.ulButtonTouched) != 0U);
                m_luaArguments->PushArgument(f_hand);
                m_luaArguments->PushArgument(iter.second);
                m_luaArguments->PushArgument(l_touchState ? 1 : 0);
                m_core->GetLuaManager()->GetEventManager()->CallEvent("onVRControllerKeyTouch", m_luaArguments);
                m_luaArguments->Clear();
            }
        }

        // Update axes
        for(uint32_t i = 0; i < vr::k_unControllerStateAxisCount; i++)
        {
            const vr::VRControllerAxis_t &l_newAxis = l_newState.rAxis[i];
            const vr::VRControllerAxis_t &l_oldAxis = l_oldState.rAxis[i];
            if((l_newAxis.x != l_oldAxis.x) || (l_newAxis.y != l_oldAxis.y))
            {
                m_luaArguments->PushArgument(f_hand);
                m_luaArguments->PushArgument(static_cast<int>(i));
                m_luaArguments->PushArgument(l_newAxis.x);
                m_luaArguments->PushArgument(l_newAxis.y);
                m_core->GetLuaManager()->GetEventManager()->CallEvent("onVRControllerAxis", m_luaArguments);
                m_luaArguments->Clear();
            }
        }

        std::memcpy(&l_oldState, &l_newState, sizeof(vr::VRControllerState_t));
    }
}

void ROC::VRManager::SubmitRender()
{
    m_vrCompositor->Submit(vr::Eye_Left, &m_vrTexture[0]);
    m_vrCompositor->Submit(vr::Eye_Right, &m_vrTexture[1]);
}
