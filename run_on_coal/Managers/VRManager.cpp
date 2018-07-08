#include "stdafx.h"

#include "Managers/VRManager.h"

#include "Core/Core.h"
#include "Elements/RenderTarget.h"

#include "Elements/Camera.h"
#include "Utils/MathUtils.h"

namespace ROC
{

extern const glm::mat4 g_IdentityMatrix;
extern const glm::vec3 g_EmptyVec3;
extern const glm::quat g_DefaultRotation;
const vr::VRTextureBounds_t g_VRTextureBounds = { 0.f, 1.f, 1.f, 0.f };

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

    m_vrTexture[0] = { reinterpret_cast<void*>(m_leftEyeRT->GetTextureID()), vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
    m_vrTexture[1] = { reinterpret_cast<void*>(m_rightEyeRT->GetTextureID()), vr::TextureType_OpenGL, vr::ColorSpace_Gamma };

    auto help = glm::quat(1, 0, 0, 0)*glm::vec3(0, 0, -1);
    m_vrStage = VRS_None;
}
ROC::VRManager::~VRManager()
{
    vr::VR_Shutdown();
    delete m_leftEyeRT;
    delete m_rightEyeRT;
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

void ROC::VRManager::DoPulse()
{
    // Update HMD data
    m_vrCompositor->WaitGetPoses(m_trackedPoses, vr::k_unMaxTrackedDeviceCount, nullptr, 0);
    const vr::TrackedDevicePose_t &l_hmdPose = m_trackedPoses[vr::k_unTrackedDeviceIndex_Hmd];
    if(l_hmdPose.bPoseIsValid)
    {
        MathUtils::ExtractMatrix(l_hmdPose.mDeviceToAbsoluteTracking, m_transform);
        btTransform l_transform;
        l_transform.setFromOpenGLMatrix(glm::value_ptr(m_transform));

        std::memcpy(&m_headPosition, l_transform.getOrigin().m_floats, sizeof(glm::vec3));
        btQuaternion l_rotation = l_transform.getRotation();
        for(int i = 0; i < 4; i++) m_headRotation[i] = l_rotation[i];

        vr::HmdMatrix34_t l_eyeTransform = m_vrSystem->GetEyeToHeadTransform(vr::Eye_Left);
        MathUtils::ExtractMatrix(l_eyeTransform, m_transform);
        l_transform.setFromOpenGLMatrix(glm::value_ptr(m_transform));
        std::memcpy(&m_leftEyePosition, l_transform.getOrigin().m_floats, sizeof(glm::vec3));

        l_eyeTransform = m_vrSystem->GetEyeToHeadTransform(vr::Eye_Right);
        MathUtils::ExtractMatrix(l_eyeTransform, m_transform);
        l_transform.setFromOpenGLMatrix(glm::value_ptr(m_transform));
        std::memcpy(&m_rightEyePosition, l_transform.getOrigin().m_floats, sizeof(glm::vec3));
    }
}

void ROC::VRManager::SubmitRender()
{
    m_vrCompositor->Submit(vr::Eye_Left, &m_vrTexture[0]);
    m_vrCompositor->Submit(vr::Eye_Right, &m_vrTexture[1]);
}
