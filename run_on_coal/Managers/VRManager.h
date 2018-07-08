#pragma once

namespace ROC
{

class Core;
class RenderTarget;

class VRManager
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

    glm::mat4 m_transform;
    glm::vec3 m_headPosition;
    glm::quat m_headRotation;
    glm::vec3 m_leftEyePosition;
    glm::vec3 m_rightEyePosition;
public:
    inline const glm::uvec2& GetTargetsSize() const { return m_targetSize; }

    inline const glm::vec3& GetHeadPosition() const { return m_headPosition; }
    inline const glm::quat& GetHeadRotation() const { return m_headRotation; }
    inline const glm::vec3& GetLeftEyePosition() const { return m_leftEyePosition; }
    inline const glm::vec3& GetRightEyePosition() const { return m_rightEyePosition; }
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
