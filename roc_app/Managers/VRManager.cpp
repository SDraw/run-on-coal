#include "stdafx.h"

#include "Managers/VRManager.h"

#include "Core/Core.h"
#include "Elements/RenderTarget.h"

#include "Managers/ConfigManager.h"
#include "Managers/ModuleManager.h"
#include "Managers/RenderManager/RenderManager.h"
#include "Interfaces/IModule.h"
#include "Elements/Camera.h"
#include "Utils/MathUtils.h"

namespace ROC
{

extern const glm::mat4 g_IdentityMatrix;
extern const glm::vec3 g_EmptyVec3;
extern const glm::quat g_DefaultRotation;
const std::vector<std::string> g_VRRenderSide
{
    "left", "right"
};

}

#define ROC_VRRENDER_SIDE_LEFT 0U
#define ROC_VRRENDER_SIDE_RIGHT 1U

ROC::VRManager::VRManager(Core *f_core)
{
    m_core = f_core;

    if(m_core->GetConfigManager()->IsVRModeEnabled())
    {
        vr::HmdError l_hmdError = vr::EVRInitError::VRInitError_None;
        m_vrSystem = vr::VR_Init(&l_hmdError, vr::EVRApplicationType::VRApplication_Scene);
        if(l_hmdError != vr::EVRInitError::VRInitError_None)
        {
            std::string l_errorString("OpenVR: Unable to start application in VR mode\n");
            l_errorString.append(vr::VR_GetVRInitErrorAsEnglishDescription(l_hmdError));
            MessageBoxA(NULL, l_errorString.c_str(), NULL, MB_OK | MB_ICONEXCLAMATION);
            exit(EXIT_FAILURE);
        }
        Camera::SetVRSystem(m_vrSystem);

        m_vrCompositor = vr::VRCompositor();
        if(!m_vrCompositor)
        {
            MessageBoxA(NULL, "OpenVR: Unable to initialize SteamVR compositor", NULL, MB_OK | MB_ICONEXCLAMATION);
            exit(EXIT_FAILURE);
        }

        m_vrOverlay = vr::VROverlay();
        if(!m_vrOverlay)
        {
            MessageBoxA(NULL, "OpenVR: Unable to initialize SteamVR overlays", NULL, MB_OK | MB_ICONEXCLAMATION);
            exit(EXIT_FAILURE);
        }

        m_vrNotifications = vr::VRNotifications();
        if(!m_vrNotifications)
        {
            MessageBoxA(NULL, "OpenVR: Unable to initialize SteamVR notifications", NULL, MB_OK | MB_ICONEXCLAMATION);
            exit(EXIT_FAILURE);
        }

        m_vrSystem->GetRecommendedRenderTargetSize(&m_targetSize.x, &m_targetSize.y);
        m_eyeRT[VRE_Left] = new RenderTarget();
        if(!m_eyeRT[VRE_Left]->Create(RenderTarget::RTT_RGB, m_targetSize, Drawable::DFT_Linear))
        {
            MessageBoxA(NULL, "OpenVR: Unable to create render target for left eye", NULL, MB_OK | MB_ICONEXCLAMATION);
            exit(EXIT_FAILURE);
        }
        m_eyeRT[VRE_Right] = new RenderTarget();
        if(!m_eyeRT[VRE_Right]->Create(RenderTarget::RTT_RGB, m_targetSize, Drawable::DFT_Linear))
        {
            MessageBoxA(NULL, "OpenVR: Unable to create render target for right eye", NULL, MB_OK | MB_ICONEXCLAMATION);
            exit(EXIT_FAILURE);
        }

        m_vrTexture[0] = { reinterpret_cast<void*>(static_cast<uintptr_t>(m_eyeRT[VRE_Left]->GetTextureID())), vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
        m_vrTexture[1] = { reinterpret_cast<void*>(static_cast<uintptr_t>(m_eyeRT[VRE_Right]->GetTextureID())), vr::TextureType_OpenGL, vr::ColorSpace_Gamma };

        UpdateEyesPosition();

        // Overlay
        m_vrOverlay->CreateOverlay("roc_overlay", "ROC overlay", &m_overlayHandle);

        // Add controllers
        vr::TrackedDeviceIndex_t l_controllers[vr::k_unMaxTrackedDeviceCount];
        uint32_t l_controllersCount = m_vrSystem->GetSortedTrackedDeviceIndicesOfClass(vr::TrackedDeviceClass_Controller, l_controllers, vr::k_unMaxTrackedDeviceCount);
        for(vr::TrackedDeviceIndex_t i = 0U; i < l_controllersCount; i++) AddController(l_controllers[i]);
    }
    else
    {
        m_vrSystem = nullptr;
        m_vrCompositor = nullptr;
        m_vrNotifications = nullptr;
        m_vrOverlay = nullptr;
        m_overlayHandle = vr::k_ulOverlayHandleInvalid;
        for(auto &l_rt : m_eyeRT) l_rt = nullptr;
    }

    m_notificationID = 0;
    m_vrStage = VRS_None;
    m_event = { 0 };
    m_state = true;

    m_transform = g_IdentityMatrix;
    m_headPosition = g_EmptyVec3;
    m_headRotation = g_DefaultRotation;
    m_leftEyePosition = g_EmptyVec3;
    m_rightEyePosition = g_EmptyVec3;
}
ROC::VRManager::~VRManager()
{
    if(m_vrOverlay) m_vrOverlay->DestroyOverlay(m_overlayHandle);
    //if(m_vrSystem) vr::VR_Shutdown(); // Somehow it starts home app. If home app is disabled, it brings to void space
    for(auto l_controller : m_vrControllers) delete l_controller;
    for(auto l_rt : m_eyeRT) delete l_rt;
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


bool ROC::VRManager::IsControllerConnected(unsigned int f_id) const
{
    bool l_result = false;
    for(auto l_controller : m_vrControllers)
    {
        if(l_controller->m_id == f_id)
        {
            l_result = true;
            break;
        }
    }
    return l_result;
}
bool ROC::VRManager::IsControllerActive(unsigned int f_id) const
{
    bool l_result = false;
    for(auto l_controller : m_vrControllers)
    {
        if(l_controller->m_id == f_id)
        {
            l_result = l_controller->m_active;
            break;
        }
    }
    return l_result;
}
unsigned char ROC::VRManager::GetControllerHandAssignment(unsigned int f_id) const
{
    unsigned char l_result = CHA_None;
    for(auto l_controller : m_vrControllers)
    {
        if(l_controller->m_id == f_id)
        {
            l_result = l_controller->m_hand;
            break;
        }
    }
    return l_result;
}
bool ROC::VRManager::GetControllerPosition(unsigned int f_id, glm::vec3 &f_pos) const
{
    bool l_result = false;
    for(auto l_controller : m_vrControllers)
    {
        if(l_controller->m_id == f_id)
        {
            std::memcpy(&f_pos, &l_controller->m_position, sizeof(glm::vec3));
            l_result = true;
            break;
        }
    }
    return l_result;
}
bool ROC::VRManager::GetControllerRotation(unsigned int f_id, glm::quat &f_rot) const
{
    bool l_result = false;
    for(auto l_controller : m_vrControllers)
    {
        if(l_controller->m_id == f_id)
        {
            std::memcpy(&f_rot, &l_controller->m_rotation, sizeof(glm::quat));
            l_result = true;
            break;
        }
    }
    return l_result;
}
bool ROC::VRManager::GetControllerVelocity(unsigned int f_id, glm::vec3 &f_val) const
{
    bool l_result = false;
    for(auto l_controller : m_vrControllers)
    {
        if(l_controller->m_id == f_id)
        {
            std::memcpy(&f_val, &l_controller->m_velocity, sizeof(glm::vec3));
            l_result = true;
            break;
        }
    }
    return l_result;
}
bool ROC::VRManager::GetControllerAngularVelocity(unsigned int f_id, glm::vec3 &f_val) const
{
    bool l_result = false;
    for(auto l_controller : m_vrControllers)
    {
        if(l_controller->m_id == f_id)
        {
            std::memcpy(&f_val, &l_controller->m_angularVelocity, sizeof(glm::vec3));
            l_result = true;
            break;
        }
    }
    return l_result;
}

bool ROC::VRManager::ShowNotification(const std::string &f_title, const std::string &f_text, unsigned int f_time)
{
    bool l_result = false;
    if(m_vrOverlay)
    {
        vr::EVROverlayError l_overlayError = m_vrOverlay->SetOverlayName(m_overlayHandle, f_title.c_str());
        l_result = (l_result && (l_overlayError == vr::VROverlayError_None));
    }
    if(m_vrNotifications)
    {
        if(m_notificationID) m_vrNotifications->RemoveNotification(m_notificationID);
        vr::EVRNotificationError l_notifyError = m_vrNotifications->CreateNotification(m_overlayHandle, f_time, vr::EVRNotificationType_Transient, f_text.c_str(), vr::EVRNotificationStyle_None, nullptr, &m_notificationID);
        if(l_notifyError != vr::VRNotificationError_OK) m_notificationID = 0;
        l_result = (l_result && (l_notifyError == vr::VRNotificationError_OK));
    }
    return l_result;
}

bool ROC::VRManager::DrawEyeImage(unsigned char f_side, const glm::vec2 &f_pos, const glm::vec2 &f_size, float f_rot, const glm::vec4 &f_color)
{
    bool l_result = false;
    if(m_vrStage == VRS_None)
    {
        RenderTarget *l_rt = nullptr;
        switch(f_side)
        {
            case VRE_Left: case VRE_Right:
                l_rt = m_eyeRT[f_side];
                break;
        }
        if(l_rt) m_core->GetRenderManager()->Render(l_rt, f_pos, f_size, f_rot, f_color);
    }
    return l_result;
}
bool ROC::VRManager::DrawEyeImage(unsigned char f_side, const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec2 &f_size, const glm::bvec4 &f_params)
{
    bool l_result = false;
    if(m_vrStage == VRS_None)
    {
        RenderTarget *l_rt = nullptr;
        switch(f_side)
        {
            case VRE_Left: case VRE_Right:
                l_rt = m_eyeRT[f_side];
                break;
        }
        if(l_rt) l_result = m_core->GetRenderManager()->Render(l_rt, f_pos, f_rot, f_size, f_params);
    }
    return l_result;
}

void ROC::VRManager::Render()
{
    if(m_vrStage == VRS_None)
    {
        m_vrStage = VRS_Left;
        m_eyeRT[VRE_Left]->Enable();

        m_arguments.Push(g_VRRenderSide[ROC_VRRENDER_SIDE_LEFT]);
        m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnVRRender, m_arguments);
        m_arguments.Clear();

        m_vrStage = VRS_Right;
        m_eyeRT[VRE_Right]->Enable();

        m_arguments.Push(g_VRRenderSide[ROC_VRRENDER_SIDE_RIGHT]);
        m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnVRRender, m_arguments);
        m_arguments.Clear();

        m_vrStage = VRS_None;
        m_eyeRT[VRE_Right]->Disable();
        if(m_vrCompositor)
        {
            m_vrCompositor->Submit(vr::Eye_Left, &m_vrTexture[0U]);
            m_vrCompositor->Submit(vr::Eye_Right, &m_vrTexture[1U]);
        }
    }
}
void ROC::VRManager::RestoreRenderTarget()
{
    switch(m_vrStage)
    {
        case VRS_Left:
            m_eyeRT[VRE_Left]->Enable();
            break;
        case VRS_Right:
            m_eyeRT[VRE_Right]->Enable();
            break;
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
                {
                    m_vrSystem->AcknowledgeQuit_Exiting();
                    m_state = false;
                } break;
                case vr::VREvent_IpdChanged:
                    UpdateEyesPosition();
                    break;
                case vr::VREvent_TrackedDeviceActivated:
                {
                    if(m_vrSystem->GetTrackedDeviceClass(m_event.trackedDeviceIndex) == vr::TrackedDeviceClass_Controller) AddController(m_event.trackedDeviceIndex);
                } break;
                case vr::VREvent_TrackedDeviceDeactivated:
                {
                    // Search through stored controllers
                    for(auto l_controllerIter = m_vrControllers.begin(); l_controllerIter != m_vrControllers.end(); ++l_controllerIter)
                    {
                        if((*l_controllerIter)->m_id == m_event.trackedDeviceIndex)
                        {
                            delete (*l_controllerIter);
                            m_vrControllers.erase(l_controllerIter);
                            break;
                        }
                    }
                } break;
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
            for(auto l_controller : m_vrControllers)
            {
                if(m_trackedPoses[l_controller->m_id].bPoseIsValid)
                {
                    l_controller->m_active = true;

                    m_vrSystem->GetControllerStateWithPose(vr::ETrackingUniverseOrigin::TrackingUniverseStanding, l_controller->m_id, &l_controller->m_newState, sizeof(vr::VRControllerState_t), &m_trackedPoses[l_controller->m_id]);
                    UpdateControllerPose(l_controller);
                    UpdateControllerInput(l_controller);
                }
                else l_controller->m_active = false;
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

void ROC::VRManager::AddController(vr::TrackedDeviceIndex_t f_id)
{
    VRController *l_controller = new VRController();
    l_controller->m_id = f_id;
    switch(m_vrSystem->GetControllerRoleForTrackedDeviceIndex(f_id))
    {
        case vr::TrackedControllerRole_LeftHand:
            l_controller->m_hand = CHA_Left;
            break;
        case vr::TrackedControllerRole_RightHand:
            l_controller->m_hand = CHA_Right;
            break;
        default:
            l_controller->m_hand = CHA_None;
            break;
    }
    l_controller->m_position = g_EmptyVec3;
    l_controller->m_rotation = g_DefaultRotation;
    l_controller->m_velocity = g_EmptyVec3;
    l_controller->m_angularVelocity = g_EmptyVec3;
    l_controller->m_oldState = { 0 };
    l_controller->m_newState = { 0 };
    l_controller->m_active = true;

    m_vrControllers.push_back(l_controller);
}

void ROC::VRManager::UpdateControllerPose(VRController *f_controller)
{
    btTransform l_transform;
    MathUtils::ConvertMatrix(m_trackedPoses[f_controller->m_id].mDeviceToAbsoluteTracking, m_transform);
    l_transform.setFromOpenGLMatrix(glm::value_ptr(m_transform));

    std::memcpy(&f_controller->m_position, l_transform.getOrigin().m_floats, sizeof(glm::vec3));

    btQuaternion l_rotation = l_transform.getRotation();

    for(int i = 0; i < 4; i++) f_controller->m_rotation[i] = l_rotation[i];

    std::memcpy(&f_controller->m_velocity, m_trackedPoses[f_controller->m_id].vVelocity.v, sizeof(glm::vec3));
    std::memcpy(&f_controller->m_angularVelocity, m_trackedPoses[f_controller->m_id].vAngularVelocity.v, sizeof(glm::vec3));
}
void ROC::VRManager::UpdateControllerInput(VRController *f_controller)
{
    vr::VRControllerState_t &l_oldState = f_controller->m_oldState;
    vr::VRControllerState_t &l_newState = f_controller->m_newState;

    for(unsigned int i = 0U; i < vr::k_EButton_Max; i++)
    {
        uint64_t l_buttonBit = (1ull << i);

        // Update buttons press
        if((l_buttonBit & l_newState.ulButtonPressed) != (l_buttonBit & l_oldState.ulButtonPressed))
        {
            m_arguments.Push(f_controller->m_id);
            m_arguments.Push(i);
            m_arguments.Push(((l_buttonBit & l_newState.ulButtonPressed) != 0U) ? 1 : 0);
            m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnVRControllerKeyPress, m_arguments);
            m_arguments.Clear();
        }

        // Update buttons touch
        if((l_buttonBit & l_newState.ulButtonTouched) != (l_buttonBit & l_oldState.ulButtonTouched))
        {
            m_arguments.Push(f_controller->m_id);
            m_arguments.Push(i);
            m_arguments.Push(((l_buttonBit & l_newState.ulButtonTouched) != 0U) ? 1 : 0);
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
            m_arguments.Push(f_controller->m_id);
            m_arguments.Push(i);
            m_arguments.Push(l_newAxis.x);
            m_arguments.Push(l_newAxis.y);
            m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnVRControllerAxis, m_arguments);
            m_arguments.Clear();
        }
    }

    std::memcpy(&l_oldState, &l_newState, sizeof(vr::VRControllerState_t));
}
