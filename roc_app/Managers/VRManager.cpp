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
extern const glm::vec4 g_zeroPoint;

const std::vector<std::string> g_vrRenderSide
{
    "left", "right"
};
enum VRRenderSide : size_t
{
    VRRS_Left = 0U,
    VRRS_Right
};

const unsigned int g_deviceAxesCount = vr::k_EButton_Axis4 - vr::k_EButton_Axis0;

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

    m_arguments = new CustomArguments();
}

ROC::VRManager::~VRManager()
{
    if(m_vrSystem) vr::VR_Shutdown();
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

bool ROC::VRManager::IsDeviceActive(unsigned int f_id) const
{
    unsigned int l_id = glm::clamp(f_id, 0U, vr::k_unMaxTrackedDeviceCount);
    return m_trackedPoses[l_id].bPoseIsValid;
}

bool ROC::VRManager::IsDeviceConnected(unsigned int f_id) const
{
    unsigned int l_id = glm::clamp(f_id, 0U, vr::k_unMaxTrackedDeviceCount);
    return m_trackedPoses[l_id].bDeviceIsConnected; 
}

bool ROC::VRManager::GetDeviceType(unsigned int f_id, unsigned char &f_type) const
{
    bool l_result = false;
    if(m_vrSystem)
    {
        vr::ETrackedDeviceClass l_class = m_vrSystem->GetTrackedDeviceClass(f_id);
        if(l_class != vr::TrackedDeviceClass_Invalid)
        {
            f_type = l_class - 1U;
            l_result = true;
        }
    }
    return l_result;
}

bool ROC::VRManager::GetDevicePosition(unsigned int f_id, glm::vec3 &f_pos) const
{
    unsigned int l_id = glm::clamp(f_id, 0U, vr::k_unMaxTrackedDeviceCount);
    glm::mat4 l_matrix(1.f);
    MathUtils::ConvertMatrix(m_trackedPoses[l_id].mDeviceToAbsoluteTracking,l_matrix);
    f_pos = l_matrix*g_zeroPoint;
    return m_trackedPoses[l_id].bDeviceIsConnected;
}

bool ROC::VRManager::GetDeviceRotation(unsigned int f_id, glm::quat &f_rot) const
{
    unsigned int l_id = glm::clamp(f_id, 0U, vr::k_unMaxTrackedDeviceCount);
    glm::mat4 l_matrix(1.f);
    MathUtils::ConvertMatrix(m_trackedPoses[l_id].mDeviceToAbsoluteTracking,l_matrix);
    f_rot = glm::quat_cast(l_matrix);
    return m_trackedPoses[l_id].bDeviceIsConnected;
}

bool ROC::VRManager::GetDeviceVelocity(unsigned int f_id, glm::vec3 &f_val) const
{
    unsigned int l_id = glm::clamp(f_id, 0U, vr::k_unMaxTrackedDeviceCount);
    for(int i = 0; i < 3; i++) f_val[i] = m_trackedPoses[l_id].vVelocity.v[i];
    return m_trackedPoses[l_id].bDeviceIsConnected;
}

bool ROC::VRManager::GetDeviceAngularVelocity(unsigned int f_id, glm::vec3 &f_val) const
{
    unsigned int l_id = glm::clamp(f_id, 0U, vr::k_unMaxTrackedDeviceCount);
    for(int i = 0; i < 3; i++) f_val[i] = m_trackedPoses[l_id].vAngularVelocity.v[i];
    return m_trackedPoses[l_id].bDeviceIsConnected;
}

bool ROC::VRManager::GetDeviceHandRole(unsigned int f_id, unsigned char &f_role) const
{
    bool l_result = false;
    if(m_vrSystem)
    {
        vr::ETrackedControllerRole l_role = m_vrSystem->GetControllerRoleForTrackedDeviceIndex(f_id);
        if(l_role != vr::TrackedControllerRole_Invalid)
        {
            f_role = l_role - 1U;
            l_result = true;
        }
    }
    return l_result;
}

bool ROC::VRManager::GetDeviceByHandRole(unsigned char f_role, unsigned int &f_id) const
{
    bool l_result = false;
    if(m_vrSystem)
    {
        vr::TrackedDeviceIndex_t l_device = m_vrSystem->GetTrackedDeviceIndexForControllerRole(static_cast<vr::ETrackedControllerRole>(f_role+1U));
        if(l_device != vr::k_unTrackedDeviceIndexInvalid)
        {
            f_id = l_device;
            l_result = true;
        }
    }
    return l_result;
}

bool ROC::VRManager::GetDeviceAxisValues(unsigned int f_id, unsigned int f_axis, glm::vec2 &f_val) const
{
    bool l_result = false;
    if(m_vrSystem)
    {
        unsigned int l_id = glm::clamp(f_id, 0U, vr::k_unMaxTrackedDeviceCount);
        vr::VRControllerState_t l_state;
        if(m_vrSystem->GetControllerState(l_id, &l_state, sizeof(vr::VRControllerState_t)))
        {
            unsigned int l_axis = glm::clamp(f_axis, 0U, g_deviceAxesCount);
            f_val.x = l_state.rAxis[l_axis].x;
            f_val.y = l_state.rAxis[l_axis].y;
            l_result = true;
        }
    }
    return l_result;
}

bool ROC::VRManager::DrawEyeImage(unsigned char f_side, const glm::vec2 &f_pos, const glm::vec2 &f_size, float f_rot, const glm::vec4 &f_color)
{
    bool l_result = false;
    if(m_vrStage == VRS_None)
    {
        btClamp<unsigned char>(f_side, VRE_Left, VRE_Right);
        l_result = m_core->GetRenderManager()->Draw(m_renderTargets[f_side], f_pos, f_size, f_rot, f_color);
    }
    return l_result;
}

bool ROC::VRManager::DrawEyeImage(unsigned char f_side, const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec2 &f_size, const glm::bvec4 &f_params)
{
    bool l_result = false;
    if(m_vrStage == VRS_None)
    {
        btClamp<unsigned char>(f_side, VRE_Left, VRE_Right);
        l_result = m_core->GetRenderManager()->Draw(m_renderTargets[f_side], f_pos, f_rot, f_size, f_params);
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
                    m_arguments->Push(m_event.trackedDeviceIndex);
                    m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnVRDeviceActivated, m_arguments);
                    m_arguments->Clear();
                } break;
                case vr::VREvent_TrackedDeviceDeactivated:
                {
                    m_arguments->Push(m_event.trackedDeviceIndex);
                    m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnVRDeviceDeactivated, m_arguments);
                    m_arguments->Clear();
                } break;
                case vr::VREvent_ButtonPress:
                {
                    m_arguments->Push(m_event.trackedDeviceIndex);
                    m_arguments->Push(m_event.data.controller.button);
                    m_arguments->Push(1);
                    m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnVRDeviceKeyPress, m_arguments);
                    m_arguments->Clear();
                } break;
                case vr::VREvent_ButtonUnpress:
                {
                    m_arguments->Push(m_event.trackedDeviceIndex);
                    m_arguments->Push(m_event.data.controller.button);
                    m_arguments->Push(0);
                    m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnVRDeviceKeyPress, m_arguments);
                    m_arguments->Clear();
                } break;
                case vr::VREvent_ButtonTouch:
                {
                    m_arguments->Push(m_event.trackedDeviceIndex);
                    m_arguments->Push(m_event.data.controller.button);
                    m_arguments->Push(1);
                    m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnVRDeviceKeyTouch, m_arguments);
                    m_arguments->Clear();
                } break;
                case vr::VREvent_ButtonUntouch:
                {
                    m_arguments->Push(m_event.trackedDeviceIndex);
                    m_arguments->Push(m_event.data.controller.button);
                    m_arguments->Push(0);
                    m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnVRDeviceKeyTouch, m_arguments);
                    m_arguments->Clear();
                } break;
            }
        }

        if(m_state)
        {
            // Update poses
            float l_lastVSync;
            m_vrSystem->GetTimeSinceLastVsync(&l_lastVSync, NULL);
            m_vrSystem->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseRawAndUncalibrated, m_hmdFramePrediction - l_lastVSync, m_trackedPoses, vr::k_unMaxTrackedDeviceCount);
        }
    }

    return m_state;
}
