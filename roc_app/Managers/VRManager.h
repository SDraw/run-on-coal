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

    vr::TrackedDevicePose_t m_trackedPoses[vr::k_unMaxTrackedDeviceCount];
    struct VRController
    {
        vr::TrackedDeviceIndex_t m_id;
        ControllerHandAssigment m_hand;
        glm::vec3 m_position;
        glm::quat m_rotation;
        glm::vec3 m_velocity;
        glm::vec3 m_angularVelocity;
        vr::VRControllerState_t m_oldState;
        vr::VRControllerState_t m_newState;
        bool m_active;
    };
    std::map<vr::TrackedDeviceIndex_t, VRController*> m_controllers;

    glm::mat4 m_transform;
    glm::vec3 m_hmdPosition;
    glm::quat m_hmdRotation;

    float m_hmdFramePrediction;

    CustomArguments *m_arguments;

    VRManager(const VRManager &that) = delete;
    VRManager& operator=(const VRManager &that) = delete;

    void AddController(vr::TrackedDeviceIndex_t f_id);
    void UpdateControllerPose(VRController *f_controller);
    void UpdateControllerInput(VRController *f_controller);
public:
    bool IsVREnabled() const;

    const glm::uvec2& GetTargetsSize() const;

    const glm::vec3& GetHmdPosition() const;
    const glm::quat& GetHmdRotation() const;

    bool IsControllerConnected(unsigned int f_id) const;
    bool IsControllerActive(unsigned int f_id) const;
    unsigned char GetControllerHand(unsigned int f_id) const;
    unsigned int GetControllerFromHand(unsigned char f_hand);
    bool GetControllerPosition(unsigned int f_id, glm::vec3 &f_pos) const;
    bool GetControllerRotation(unsigned int f_id, glm::quat &f_rot) const;
    bool GetControllerVelocity(unsigned int f_id, glm::vec3 &f_val) const;
    bool GetControllerAngularVelocity(unsigned int f_id, glm::vec3 &f_val) const;

    bool DrawEyeImage(unsigned char f_side, const glm::vec2 &f_pos, const glm::vec2 &f_size, float f_rot, const glm::vec4 &f_color, const std::string &f_layer);
    bool DrawEyeImage(unsigned char f_side, const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec2 &f_size, const std::string &f_layer, const glm::bvec4 &f_params);
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
