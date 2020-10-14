#pragma once
#include "Interfaces/IVRManager.h"

namespace ROC
{

class Core;
class RenderTarget;
class CustomArguments;

class VRManager final : public IVRManager
{
    Core *m_core;

    vr::IVRSystem *m_vrSystem;

    RenderTarget *m_renderTargets[2U];
    glm::uvec2 m_targetSize;
    vr::Texture_t m_vrTextures[2U];
    vr::VREvent_t m_event;
    vr::VROverlayHandle_t m_overlayHandle;
    vr::VRNotificationId m_notificationID;

    enum VRStage : unsigned char
    {
        VRS_None = 0U,
        VRS_Left,
        VRS_Right
    };
    VRStage m_vrStage;
    bool m_state;

    float m_hmdFramePrediction;
    vr::TrackedDevicePose_t m_trackedPoses[vr::k_unMaxTrackedDeviceCount];

    CustomArguments *m_arguments;

    VRManager(const VRManager &that) = delete;
    VRManager& operator=(const VRManager &that) = delete;
public:
    bool IsVREnabled() const;

    bool IsDeviceActive(unsigned int f_id) const;
    bool IsDeviceConnected(unsigned int f_id) const;
    bool GetDeviceType(unsigned int f_id, unsigned char &f_type) const;
    bool GetDevicePosition(unsigned int f_id, glm::vec3 &f_pos) const;
    bool GetDeviceRotation(unsigned int f_id, glm::quat &f_rot) const;
    bool GetDeviceVelocity(unsigned int f_id, glm::vec3 &f_val) const;
    bool GetDeviceAngularVelocity(unsigned int f_id, glm::vec3 &f_val) const;
    bool GetDeviceHandRole(unsigned int f_id, unsigned char &f_role) const;
    bool GetDeviceByHandRole(unsigned char f_role, unsigned int &f_id) const;
    bool GetDeviceAxisValues(unsigned int f_id, unsigned int f_axis, glm::vec2 &f_val) const;

    const glm::uvec2& GetTargetsSize() const;
    bool DrawEyeImage(unsigned char f_side, const glm::vec2 &f_pos, const glm::vec2 &f_size, float f_rot, const glm::vec4 &f_color);
    bool DrawEyeImage(unsigned char f_side, const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec2 &f_size, const glm::bvec4 &f_params);
protected:
    explicit VRManager(Core *f_core);
    ~VRManager();

    void Render();
    void RestoreRenderTarget();

    bool DoPulse();

    friend class Core;
    friend class RenderManager;
};

}
