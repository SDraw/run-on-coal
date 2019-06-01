#include "stdafx.h"

#include "Managers/VRManager.h"

#include "Core/Core.h"
#include "Elements/RenderTarget.h"

#include "Managers/ConfigManager.h"
#include "Managers/ModuleManager.h"
#include "Interfaces/IModule.h"
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

    if(m_core->GetConfigManager()->IsVRModeEnabled())
    {
        vr::HmdError l_hmdError = vr::EVRInitError::VRInitError_None;
        m_vrSystem = vr::VR_Init(&l_hmdError, vr::EVRApplicationType::VRApplication_Scene);
        if(l_hmdError != vr::EVRInitError::VRInitError_None)
        {
            MessageBoxA(NULL, "OpenVR: Unable to start application in VR mode", NULL, MB_OK | MB_ICONEXCLAMATION);
            exit(EXIT_FAILURE);
        }
        Camera::SetVRSystem(m_vrSystem);

        m_vrCompositor = vr::VRCompositor();
        if(!m_vrCompositor)
        {
            MessageBoxA(NULL, "OpenVR: Unable to initialize SteamVR compositor", NULL, MB_OK | MB_ICONEXCLAMATION);
            exit(EXIT_FAILURE);
        }

        m_vrSystem->GetRecommendedRenderTargetSize(&m_targetSize.x, &m_targetSize.y);
        m_leftEyeRT = new RenderTarget();
        if(!m_leftEyeRT->Create(RenderTarget::RTT_RGB, m_targetSize, Drawable::DFT_Linear))
        {
            MessageBoxA(NULL, "OpenVR: Unable to create render target for left eye", NULL, MB_OK | MB_ICONEXCLAMATION);
            exit(EXIT_FAILURE);
        }
        m_rightEyeRT = new RenderTarget();
        if(!m_rightEyeRT->Create(RenderTarget::RTT_RGB, m_targetSize, Drawable::DFT_Linear))
        {
            MessageBoxA(NULL, "OpenVR: Unable to create render target for right eye", NULL, MB_OK | MB_ICONEXCLAMATION);
            exit(EXIT_FAILURE);
        }

        m_vrTexture[0] = { UIntToPtr(m_leftEyeRT->GetTextureID()), vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
        m_vrTexture[1] = { UIntToPtr(m_rightEyeRT->GetTextureID()), vr::TextureType_OpenGL, vr::ColorSpace_Gamma };

        UpdateEyesPosition();
    }
    else
    {
        m_vrSystem = nullptr;
        m_vrCompositor = nullptr;
        m_leftEyeRT = nullptr;
        m_rightEyeRT = nullptr;
    }

    m_vrStage = VRS_None;
    m_leftController = { g_EmptyVec3, g_DefaultRotation, g_EmptyVec3, g_EmptyVec3, { 0U }, { 0U }, false };
    m_rightController = { g_EmptyVec3, g_DefaultRotation, g_EmptyVec3, g_EmptyVec3, { 0U }, { 0U }, false };
    m_event = { 0 };
    m_state = true;
}
ROC::VRManager::~VRManager()
{
    if(m_vrSystem) vr::VR_Shutdown();
    delete m_leftEyeRT;
    delete m_rightEyeRT;
}

bool ROC::VRManager::IsVREnabled() const
{
    return (m_vrSystem != nullptr);
}

const glm::uvec2& ROC::VRManager::GetTargetsSize()  const
{
    return m_targetSize;
}

const glm::vec3& ROC::VRManager::GetHeadPosition() const
{
    return m_headPosition;
}
const glm::quat& ROC::VRManager::GetHeadRotation() const
{
    return m_headRotation;
}

const glm::vec3& ROC::VRManager::GetLeftEyePosition() const
{
    return m_leftEyePosition;
}
const glm::vec3& ROC::VRManager::GetRightEyePosition() const
{
    return m_rightEyePosition;
}

const bool ROC::VRManager::IsLeftControllerActive() const
{
    return m_leftController.m_updated;
}
const glm::vec3& ROC::VRManager::GetLeftControllerPosition() const
{
    return m_leftController.m_position;
}
const glm::quat& ROC::VRManager::GetLeftControllerRotation() const
{
    return m_leftController.m_rotation;
}
const glm::vec3& ROC::VRManager::GetLeftControllerVelocity() const
{
    return m_leftController.m_velocity;
}
const glm::vec3& ROC::VRManager::GetLeftControllerAngularVelocity() const
{
    return m_leftController.m_angularVelocity;
}

const bool ROC::VRManager::IsRightControllerActive() const
{
    return m_rightController.m_updated;
}
const glm::vec3& ROC::VRManager::GetRightControllerPosition() const
{
    return m_rightController.m_position;
}
const glm::quat& ROC::VRManager::GetRightControllerRotation() const
{
    return m_rightController.m_rotation;
}
const glm::vec3& ROC::VRManager::GetRightControllerVelocity() const
{
    return m_rightController.m_velocity;
}
const glm::vec3& ROC::VRManager::GetRightControllerAngularVelocity() const
{
    return m_rightController.m_angularVelocity;
}

void ROC::VRManager::EnableRenderTarget()
{
    if(m_vrSystem)
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
}

bool ROC::VRManager::DoPulse()
{
    if(m_vrSystem)
    {
        // Poll events
        while(m_vrSystem->PollNextEvent(&m_event, sizeof(vr::VREvent_t)))
        {
            switch(m_event.eventType)
            {
                case vr::VREvent_DriverRequestedQuit: case vr::VREvent_Quit:
                    m_state = false;
                    break;
                case vr::VREvent_IpdChanged:
                    UpdateEyesPosition();
                    break;
            }
        }

        if(m_state)
        {
            // Update HMD data
            m_vrCompositor->WaitGetPoses(m_trackedPoses, vr::k_unMaxTrackedDeviceCount, nullptr, 0);
            m_vrSystem->GetDeviceToAbsoluteTrackingPose(vr::ETrackingUniverseOrigin::TrackingUniverseStanding, 0.f, m_trackedPoses, vr::k_unMaxTrackedDeviceCount);

            const vr::TrackedDevicePose_t &l_hmdPose = m_trackedPoses[vr::k_unTrackedDeviceIndex_Hmd];
            if(l_hmdPose.bPoseIsValid)
            {
                MathUtils::ConvertMatrix(l_hmdPose.mDeviceToAbsoluteTracking, m_transform);
                btTransform l_transform;
                l_transform.setFromOpenGLMatrix(glm::value_ptr(m_transform));

                btVector3 &l_origin = l_transform.getOrigin();
                std::memcpy(&m_headPosition, l_origin.m_floats, sizeof(glm::vec3));
                btQuaternion l_rotation = l_transform.getRotation();
                for(int i = 0; i < 4; i++) m_headRotation[i] = l_rotation[i];
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
                                m_vrSystem->GetControllerStateWithPose(vr::ETrackingUniverseOrigin::TrackingUniverseStanding, i, &m_leftController.m_newState, sizeof(vr::VRControllerState_t), &m_trackedPoses[i]);
                                UpdateControllerPose(m_leftController, m_trackedPoses[i]);
                                UpdateControllerInput(m_leftController, "left");
                            } break;
                            case vr::TrackedControllerRole_RightHand:
                            {
                                m_vrSystem->GetControllerStateWithPose(vr::ETrackingUniverseOrigin::TrackingUniverseStanding, i, &m_rightController.m_newState, sizeof(vr::VRControllerState_t), &m_trackedPoses[i]);
                                UpdateControllerPose(m_rightController, m_trackedPoses[i]);
                                UpdateControllerInput(m_rightController, "right");
                            } break;
                        }
                    }
                }
            }
        }
    }

    return m_state;
}

void ROC::VRManager::UpdateEyesPosition()
{
    btTransform l_transform;
    btVector3 &l_origin = l_transform.getOrigin();

    vr::HmdMatrix34_t l_eyeTransform = m_vrSystem->GetEyeToHeadTransform(vr::Eye_Left);
    MathUtils::ConvertMatrix(l_eyeTransform, m_transform);
    m_transform = glm::inverse(m_transform);
    l_transform.setFromOpenGLMatrix(glm::value_ptr(m_transform));
    std::memcpy(&m_leftEyePosition, l_origin.m_floats, sizeof(glm::vec3));

    l_eyeTransform = m_vrSystem->GetEyeToHeadTransform(vr::Eye_Right);
    MathUtils::ConvertMatrix(l_eyeTransform, m_transform);
    m_transform = glm::inverse(m_transform);
    l_transform.setFromOpenGLMatrix(glm::value_ptr(m_transform));
    std::memcpy(&m_rightEyePosition, l_origin.m_floats, sizeof(glm::vec3));
}

void ROC::VRManager::UpdateControllerPose(VRController &f_controller, const vr::TrackedDevicePose_t &f_pose)
{
    if(!f_controller.m_updated)
    {
        btTransform l_transform;
        MathUtils::ConvertMatrix(f_pose.mDeviceToAbsoluteTracking, m_transform);
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
                m_arguments.Push(f_hand);
                m_arguments.Push(iter.second);
                m_arguments.Push(l_pressState ? 1 : 0);
                m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnVRControllerKeyPress, m_arguments);
                m_arguments.Clear();
            }
        }

        // Update buttons touch
        for(const auto &iter : g_VRControllerButtons)
        {
            if((iter.first & l_newState.ulButtonTouched) != (iter.first & l_oldState.ulButtonTouched))
            {
                bool l_touchState = ((iter.first & l_newState.ulButtonTouched) != 0U);
                m_arguments.Push(f_hand);
                m_arguments.Push(iter.second);
                m_arguments.Push(l_touchState ? 1 : 0);
                m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnVRControllerKeyTouch, m_arguments);
                m_arguments.Clear();
            }
        }

        // Update axes
        for(uint32_t i = 0; i < vr::k_unControllerStateAxisCount; i++)
        {
            const vr::VRControllerAxis_t &l_newAxis = l_newState.rAxis[i];
            const vr::VRControllerAxis_t &l_oldAxis = l_oldState.rAxis[i];
            if((l_newAxis.x != l_oldAxis.x) || (l_newAxis.y != l_oldAxis.y))
            {
                m_arguments.Push(f_hand);
                m_arguments.Push(static_cast<int>(i));
                m_arguments.Push(l_newAxis.x);
                m_arguments.Push(l_newAxis.y);
                m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnVRControllerAxis, m_arguments);
                m_arguments.Clear();
            }
        }

        std::memcpy(&l_oldState, &l_newState, sizeof(vr::VRControllerState_t));
    }
}

void ROC::VRManager::SubmitRender()
{
    if(m_vrCompositor)
    {
        m_vrCompositor->Submit(vr::Eye_Left, &m_vrTexture[0U]);
        m_vrCompositor->Submit(vr::Eye_Right, &m_vrTexture[1U]);
    }
}
