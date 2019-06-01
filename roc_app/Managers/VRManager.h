#pragma once
#include "Interfaces/IVRManager.h"
#include "Utils/CustomArguments.h"

namespace ROC
{

class Core;
class RenderTarget;

class VRManager final : public IVRManager
{
    Core *m_core;

    vr::IVRSystem *m_vrSystem;
    vr::IVRCompositor *m_vrCompositor;
    RenderTarget *m_leftEyeRT;
    RenderTarget *m_rightEyeRT;
    glm::uvec2 m_targetSize;
    vr::Texture_t m_vrTexture[2];
    vr::VREvent_t m_event;
    bool m_state;

    enum RMVRStage : unsigned char
    {
        VRS_None = 0U,
        VRS_Left,
        VRS_Right
    };
    RMVRStage m_vrStage;

    vr::TrackedDevicePose_t m_trackedPoses[vr::k_unMaxTrackedDeviceCount];
    struct VRController
    {
        glm::vec3 m_position;
        glm::quat m_rotation;
        glm::vec3 m_velocity;
        glm::vec3 m_angularVelocity;
        vr::VRControllerState_t m_oldState;
        vr::VRControllerState_t m_newState;
        bool m_updated;
    };
    VRController m_leftController;
    VRController m_rightController;

    glm::mat4 m_transform;
    glm::vec3 m_headPosition;
    glm::quat m_headRotation;
    glm::vec3 m_leftEyePosition;
    glm::vec3 m_rightEyePosition;

    CustomArguments m_arguments;

    VRManager(const VRManager &that) = delete;
    VRManager& operator=(const VRManager &that) = delete;

    void UpdateEyesPosition();
    void UpdateControllerPose(VRController &f_controller, const vr::TrackedDevicePose_t &f_pose);
    void UpdateControllerInput(VRController &f_controller, const std::string &f_hand);
public:
    bool IsVREnabled() const;

    const glm::uvec2& GetTargetsSize() const;

    const glm::vec3& GetHeadPosition() const;
    const glm::quat& GetHeadRotation() const;

    const glm::vec3& GetLeftEyePosition() const;
    const glm::vec3& GetRightEyePosition() const;

    const bool IsLeftControllerActive() const;
    const glm::vec3& GetLeftControllerPosition() const;
    const glm::quat& GetLeftControllerRotation() const;
    const glm::vec3& GetLeftControllerVelocity() const;
    const glm::vec3& GetLeftControllerAngularVelocity() const;

    const bool IsRightControllerActive() const;
    const glm::vec3& GetRightControllerPosition() const;
    const glm::quat& GetRightControllerRotation() const;
    const glm::vec3& GetRightControllerVelocity() const;
    const glm::vec3& GetRightControllerAngularVelocity() const;
protected:
    explicit VRManager(Core *f_core);
    ~VRManager();

    inline RMVRStage GetVRStage() const { return m_vrStage; }
    inline void SetVRStage(RMVRStage f_stage) { m_vrStage = f_stage; }

    void EnableRenderTarget();

    bool DoPulse();
    void SubmitRender();
    
    friend class Core;
    friend class RenderManager;
};

}
