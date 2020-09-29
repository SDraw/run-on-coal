#include "stdafx.h"

#include "Managers/VRManager.h"

#include "Core/Core.h"
#include "Elements/RenderTarget.h"
#include "Utils/CustomArguments.h"

#include "Managers/ConfigManager.h"
#include "Managers/ModuleManager.h"
#include "Managers/RenderManager/RenderManager.h"
#include "Elements/Camera.h"
#include "Interfaces/IModule.h"
#include "GL/GLState.h"
#include "GL/GLTexture2D.h"
#include "Utils/MathUtils.h"

namespace ROC
{

extern const glm::mat4 g_identityMatrix;
extern const glm::vec3 g_emptyVec3;
extern const glm::quat g_defaultRotation;

const std::vector<std::string> g_vrRenderSide
{
    "left", "right"
};
enum VRRenderSide : size_t
{
    VRRS_Left = 0U,
    VRRS_Right
};

}

ROC::VRManager::VRManager(Core *f_core)
{
    m_core = f_core;

    if(m_core->GetConfigManager()->IsVRModeEnabled())
    {
        vr::HmdError l_hmdError = vr::EVRInitError::VRInitError_None;
        m_vrSystem = vr::VR_Init(&l_hmdError, vr::EVRApplicationType::VRApplication_Scene);
        if(l_hmdError != vr::EVRInitError::VRInitError_None)
        {
            std::string l_errorString("Unable to start application in VR mode\nOpenVR: ");
            l_errorString.append(vr::VR_GetVRInitErrorAsEnglishDescription(l_hmdError));
            MessageBoxA(NULL, l_errorString.c_str(), NULL, MB_OK | MB_ICONEXCLAMATION);
            std::terminate();
        }
        Camera::SetVRSystem(m_vrSystem);

        m_vrSystem->GetRecommendedRenderTargetSize(&m_targetSize.x, &m_targetSize.y);
        m_renderTargets[VRE_Left] = new RenderTarget();
        if(!m_renderTargets[VRE_Left]->Create(RenderTarget::RTT_RGBA, m_targetSize, Drawable::DFT_Linear))
        {
            MessageBoxA(NULL, "OpenVR: Unable to create render target for left eye", NULL, MB_OK | MB_ICONEXCLAMATION);
            std::terminate();
        }
        m_renderTargets[VRE_Right] = new RenderTarget();
        if(!m_renderTargets[VRE_Right]->Create(RenderTarget::RTT_RGBA, m_targetSize, Drawable::DFT_Linear))
        {
            MessageBoxA(NULL, "OpenVR: Unable to create render target for right eye", NULL, MB_OK | MB_ICONEXCLAMATION);
            std::terminate();
        }

        m_vrTextures[0U] = { reinterpret_cast<void*>(static_cast<uintptr_t>(m_renderTargets[VRE_Left]->GetGLTexture()->GetName())), vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
        m_vrTextures[1U] = { reinterpret_cast<void*>(static_cast<uintptr_t>(m_renderTargets[VRE_Right]->GetGLTexture()->GetName())), vr::TextureType_OpenGL, vr::ColorSpace_Gamma };

        // Add controllers
        vr::TrackedDeviceIndex_t l_controllers[vr::k_unMaxTrackedDeviceCount];
        uint32_t l_controllersCount = m_vrSystem->GetSortedTrackedDeviceIndicesOfClass(vr::TrackedDeviceClass_Controller, l_controllers, vr::k_unMaxTrackedDeviceCount);
        for(vr::TrackedDeviceIndex_t i = 0U; i < l_controllersCount; i++) AddController(l_controllers[i]);

        const float l_frameTime = 1.f / m_vrSystem->GetFloatTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_DisplayFrequency_Float);
        const float l_vsyncToPhotons = m_vrSystem->GetFloatTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SecondsFromVsyncToPhotons_Float);
        m_hmdFramePrediction = l_frameTime + l_vsyncToPhotons;
    }
    else
    {
        m_vrSystem = nullptr;
        m_overlayHandle = vr::k_ulOverlayHandleInvalid;
        for(auto &l_rt : m_renderTargets) l_rt = nullptr;
        m_hmdFramePrediction = 0.f;
    }

    m_notificationID = 0;
    m_vrStage = VRS_None;
    m_event = { 0 };
    m_state = true;

    m_transform = g_identityMatrix;
    m_hmdPosition = g_emptyVec3;
    m_hmdRotation = g_defaultRotation;

    m_arguments = new CustomArguments();
}

ROC::VRManager::~VRManager()
{
    if(m_vrSystem) vr::VR_Shutdown();
    for(auto l_controller : m_controllers) delete l_controller.second;
    for(auto l_rt : m_renderTargets) delete l_rt;
    delete m_arguments;
}

bool ROC::VRManager::IsVREnabled() const
{
    return (m_vrSystem != nullptr);
}

const glm::uvec2& ROC::VRManager::GetTargetsSize()  const
{
    return m_targetSize;
}

const glm::vec3& ROC::VRManager::GetHmdPosition() const
{
    return m_hmdPosition;
}

const glm::quat& ROC::VRManager::GetHmdRotation() const
{
    return m_hmdRotation;
}

bool ROC::VRManager::IsControllerConnected(unsigned int f_id) const
{
    bool l_result = (m_controllers.find(f_id) != m_controllers.end());
    return l_result;
}

bool ROC::VRManager::IsControllerActive(unsigned int f_id) const
{
    bool l_result = false;
    auto l_searchIter = m_controllers.find(f_id);
    if(l_searchIter != m_controllers.end()) l_result = l_searchIter->second->m_active;
    return l_result;
}

unsigned char ROC::VRManager::GetControllerHand(unsigned int f_id) const
{
    ControllerHandAssigment l_hand = CHA_Any;
    auto l_searchIter = m_controllers.find(f_id);
    if(l_searchIter != m_controllers.end()) l_hand = l_searchIter->second->m_hand;
    return l_hand;
}

unsigned int ROC::VRManager::GetControllerFromHand(unsigned char f_hand)
{
    unsigned int l_result = 0xFFU;

    // Search through stored
    for(auto l_iter : m_controllers)
    {
        if(l_iter.second->m_hand == f_hand)
        {
            l_result = l_iter.second->m_id;
            break;
        }
    }

    // No stored controller, try to retrieve it
    if((l_result == 0xFFU) && m_vrSystem)
    {
        // Try left hand
        vr::TrackedDeviceIndex_t l_controllerIndex = m_vrSystem->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand);
        if(l_controllerIndex != vr::k_unTrackedDeviceIndexInvalid)
        {
            auto l_searchIter = m_controllers.find(l_controllerIndex);
            if(l_searchIter != m_controllers.end())
            {
                l_searchIter->second->m_hand = CHA_Left; // Reassign hand
            }
            else AddController(l_controllerIndex);
            l_result = l_controllerIndex;
        }

        // Try right hand
        if(l_controllerIndex == vr::k_unTrackedDeviceIndexInvalid)
        {
            l_controllerIndex = m_vrSystem->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_RightHand);
            if(l_controllerIndex != vr::k_unTrackedDeviceIndexInvalid)
            {
                auto l_searchIter = m_controllers.find(l_controllerIndex);
                if(l_searchIter != m_controllers.end())
                {
                    l_searchIter->second->m_hand = CHA_Right; // Reassign hand
                    l_result = l_controllerIndex;
                }
            }
            else AddController(l_controllerIndex);
            l_result = l_controllerIndex;
        }
    }

    return l_result;
}

bool ROC::VRManager::GetControllerPosition(unsigned int f_id, glm::vec3 &f_pos) const
{
    bool l_result = false;
    auto l_searchIter = m_controllers.find(f_id);
    if(l_searchIter != m_controllers.end())
    {
        std::memcpy(&f_pos, &l_searchIter->second->m_position, sizeof(glm::vec3));
        l_result = true;
    }
    return l_result;
}

bool ROC::VRManager::GetControllerRotation(unsigned int f_id, glm::quat &f_rot) const
{
    bool l_result = false;
    auto l_searchIter = m_controllers.find(f_id);
    if(l_searchIter != m_controllers.end())
    {
        std::memcpy(&f_rot, &l_searchIter->second->m_rotation, sizeof(glm::quat));
        l_result = true;
    }
    return l_result;
}

bool ROC::VRManager::GetControllerVelocity(unsigned int f_id, glm::vec3 &f_val) const
{
    bool l_result = false;
    auto l_searchIter = m_controllers.find(f_id);
    if(l_searchIter != m_controllers.end())
    {
        std::memcpy(&f_val, &l_searchIter->second->m_velocity, sizeof(glm::vec3));
        l_result = true;
    }
    return l_result;
}

bool ROC::VRManager::GetControllerAngularVelocity(unsigned int f_id, glm::vec3 &f_val) const
{
    bool l_result = false;
    auto l_searchIter = m_controllers.find(f_id);
    if(l_searchIter != m_controllers.end())
    {
        std::memcpy(&f_val, &l_searchIter->second->m_angularVelocity, sizeof(glm::vec3));
        l_result = true;
    }
    return l_result;
}

bool ROC::VRManager::DrawEyeImage(unsigned char f_side, const glm::vec2 &f_pos, const glm::vec2 &f_size, float f_rot, const glm::vec4 &f_color, const std::string &f_layer)
{
    bool l_result = false;
    if(m_vrStage == VRS_None)
    {
        btClamp<unsigned char>(f_side, VRE_Left, VRE_Right);
        l_result = m_core->GetRenderManager()->Render(m_renderTargets[f_side], f_pos, f_size, f_rot, f_color, f_layer);
    }
    return l_result;
}

bool ROC::VRManager::DrawEyeImage(unsigned char f_side, const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec2 &f_size, const std::string &f_layer, const glm::bvec4 &f_params)
{
    bool l_result = false;
    if(m_vrStage == VRS_None)
    {
        btClamp<unsigned char>(f_side, VRE_Left, VRE_Right);
        l_result = m_core->GetRenderManager()->Render(m_renderTargets[f_side], f_pos, f_rot, f_size, f_layer, f_params);
    }
    return l_result;
}

void ROC::VRManager::Render()
{
    if(m_vrStage == VRS_None)
    {
        m_vrStage = VRS_Left;
        m_renderTargets[VRE_Left]->Enable();
        RenderTarget::SetFallbackRenderTarget(m_renderTargets[VRE_Left]);

        m_arguments->Push(g_vrRenderSide[VRRenderSide::VRRS_Left]);
        m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnVRRender, m_arguments);
        m_arguments->Clear();

        m_vrStage = VRS_Right;
        m_renderTargets[VRE_Right]->Enable();
        RenderTarget::SetFallbackRenderTarget(m_renderTargets[VRE_Right]);

        m_arguments->Push(g_vrRenderSide[VRRenderSide::VRRS_Right]);
        m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnVRRender, m_arguments);
        m_arguments->Clear();

        m_vrStage = VRS_None;
        m_renderTargets[VRE_Right]->Disable();
        RenderTarget::SetFallbackRenderTarget(nullptr);

        vr::VRCompositor()->WaitGetPoses(m_trackedPoses, vr::k_unMaxTrackedDeviceCount, nullptr, 0);
        vr::VRCompositor()->Submit(vr::Eye_Left, &m_vrTextures[0U]);
        vr::VRCompositor()->Submit(vr::Eye_Right, &m_vrTextures[1U]);
        GLState::Flush();
    }
}

void ROC::VRManager::RestoreRenderTarget()
{
    switch(m_vrStage)
    {
        case VRS_Left:
            m_renderTargets[VRE_Left]->Enable(false);
            break;
        case VRS_Right:
            m_renderTargets[VRE_Right]->Enable(false);
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
                case vr::VREvent_TrackedDeviceActivated:
                {
                    if(m_vrSystem->GetTrackedDeviceClass(m_event.trackedDeviceIndex) == vr::TrackedDeviceClass_Controller) AddController(m_event.trackedDeviceIndex);
                } break;
                case vr::VREvent_TrackedDeviceDeactivated:
                {
                    // Search through stored controllers
                    auto l_searchIter = m_controllers.find(m_event.trackedDeviceIndex);
                    if(l_searchIter != m_controllers.end())
                    {
                        delete l_searchIter->second;
                        m_controllers.erase(l_searchIter);
                    }
                } break;
            }
        }

        if(m_state)
        {
            // Update poses
            float l_lastVSync;
            m_vrSystem->GetTimeSinceLastVsync(&l_lastVSync, NULL);
            m_vrSystem->GetDeviceToAbsoluteTrackingPose(vr::ETrackingUniverseOrigin::TrackingUniverseStanding, m_hmdFramePrediction - l_lastVSync, m_trackedPoses, vr::k_unMaxTrackedDeviceCount);

            // Update HMD data
            const vr::TrackedDevicePose_t &l_hmdPose = m_trackedPoses[vr::k_unTrackedDeviceIndex_Hmd];
            if(l_hmdPose.bPoseIsValid)
            {
                MathUtils::ConvertMatrix(l_hmdPose.mDeviceToAbsoluteTracking, m_transform);
                btTransform l_transform;
                l_transform.setFromOpenGLMatrix(glm::value_ptr(m_transform));

                btVector3 &l_origin = l_transform.getOrigin();
                std::memcpy(&m_hmdPosition, l_origin.m_floats, sizeof(glm::vec3));
                btQuaternion l_rotation = l_transform.getRotation();
                for(int i = 0; i < 4; i++) m_hmdRotation[i] = l_rotation[i];
            }

            // Update controllers
            for(auto l_iter : m_controllers)
            {
                VRController *l_controller = l_iter.second;
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

void ROC::VRManager::AddController(vr::TrackedDeviceIndex_t f_id)
{
    if(m_controllers.find(f_id) == m_controllers.end())
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
                l_controller->m_hand = CHA_Any;
                break;
        }
        l_controller->m_position = g_emptyVec3;
        l_controller->m_rotation = g_defaultRotation;
        l_controller->m_velocity = g_emptyVec3;
        l_controller->m_angularVelocity = g_emptyVec3;
        l_controller->m_oldState = { 0 };
        l_controller->m_newState = { 0 };
        l_controller->m_active = true;

        m_controllers.emplace(f_id, l_controller);
    }
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
            m_arguments->Push(f_controller->m_id);
            m_arguments->Push(i);
            m_arguments->Push(((l_buttonBit & l_newState.ulButtonPressed) != 0U) ? 1 : 0);
            m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnVRControllerKeyPress, m_arguments);
            m_arguments->Clear();
        }

        // Update buttons touch
        if((l_buttonBit & l_newState.ulButtonTouched) != (l_buttonBit & l_oldState.ulButtonTouched))
        {
            m_arguments->Push(f_controller->m_id);
            m_arguments->Push(i);
            m_arguments->Push(((l_buttonBit & l_newState.ulButtonTouched) != 0U) ? 1 : 0);
            m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnVRControllerKeyTouch, m_arguments);
            m_arguments->Clear();
        }
    }

    // Update axes
    for(uint32_t i = 0; i < vr::k_unControllerStateAxisCount; i++)
    {
        const vr::VRControllerAxis_t &l_newAxis = l_newState.rAxis[i];
        const vr::VRControllerAxis_t &l_oldAxis = l_oldState.rAxis[i];
        if((l_newAxis.x != l_oldAxis.x) || (l_newAxis.y != l_oldAxis.y))
        {
            m_arguments->Push(f_controller->m_id);
            m_arguments->Push(i);
            m_arguments->Push(l_newAxis.x);
            m_arguments->Push(l_newAxis.y);
            m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnVRControllerAxis, m_arguments);
            m_arguments->Clear();
        }
    }

    std::memcpy(&l_oldState, &l_newState, sizeof(vr::VRControllerState_t));
}
