#pragma once

namespace ROC
{

class Core;
class RenderTarget;

class VRManager final
{
    Core *m_core;

    vr::IVRSystem *m_vrSystem;
    vr::IVRCompositor *m_vrCompositor;
    RenderTarget *m_leftEyeRT;
    RenderTarget *m_rightEyeRT;
    glm::uvec2 m_targetSize;
    vr::Texture_t m_vrTexture[2];

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
        bool m_updated;
    };
    VRController m_leftController;
    VRController m_rightController;

    glm::mat4 m_transform;
    glm::vec3 m_headPosition;
    glm::quat m_headRotation;
    glm::vec3 m_leftEyePosition;
    glm::vec3 m_rightEyePosition;

    VRManager(const VRManager& that);
    VRManager &operator =(const VRManager &that);
public:
    inline const glm::uvec2& GetTargetsSize() const { return m_targetSize; }

    inline const glm::vec3& GetHeadPosition() const { return m_headPosition; }
    inline const glm::quat& GetHeadRotation() const { return m_headRotation; }

    inline const glm::vec3& GetLeftEyePosition() const { return m_leftEyePosition; }
    inline const glm::vec3& GetRightEyePosition() const { return m_rightEyePosition; }

    inline const bool IsLeftControllerActive() const { return m_leftController.m_updated; }
    inline const glm::vec3& GetLeftControllerPosition() const { return m_leftController.m_position; }
    inline const glm::quat& GetLeftControllerRotation() const { return m_leftController.m_rotation; }
    inline const glm::vec3& GetLeftControllerVelocity() const { return m_leftController.m_velocity; }
    inline const glm::vec3& GetLeftControllerAngularVelocity() const { return m_leftController.m_angularVelocity; }

    inline const bool IsRightControllerActive() const { return m_rightController.m_updated; }
    inline const glm::vec3& GetRightControllerPosition() const { return m_rightController.m_position; }
    inline const glm::quat& GetRightControllerRotation() const { return m_rightController.m_rotation; }
    inline const glm::vec3& GetRightControllerVelocity() const { return m_rightController.m_velocity; }
    inline const glm::vec3& GetRightControllerAngularVelocity() const { return m_rightController.m_angularVelocity; }
protected:
    explicit VRManager(Core *f_core);
    ~VRManager();

    inline RMVRStage GetVRStage() const { return m_vrStage; }
    inline void SetVRStage(RMVRStage f_stage) { m_vrStage = f_stage; }

    void EnableRenderTarget();
    void DisableRenderTarget();

    void DoPulse();
    void SubmitRender();
    
    friend class Core;
    friend class RenderManager;
};

}
