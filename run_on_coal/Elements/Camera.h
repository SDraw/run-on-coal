#pragma once
#include "Elements/Element.h"

namespace ROC
{

class Camera final : public Element
{
    int m_type;

    glm::vec3 m_viewPosition;
    glm::vec3 m_viewDirection;
    glm::vec3 m_upDirection;
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;
    glm::mat4 m_viewProjectionMatrix;
    bool m_rebuildView;
    bool m_rebuildProjection;

    float m_fov;
    float m_aspectRatio;
    glm::vec4 m_orthoParams;
    glm::vec2 m_depth;

    glm::vec4 m_planes[6];

    static vr::IVRSystem *ms_vrSystem;

    Camera(const Camera &that);
    Camera& operator=(const Camera &that);
public:
    enum CameraProjectionType
    {
        CPT_Perspective,
        CPT_Orthogonal,
        CPT_Screen,
        CPT_VRLeft,
        CPT_VRRight
    };

    void SetProjectionType(int f_type);
    inline int GetProjectionType() const { return m_type; }

    void SetFOV(float f_fov);
    inline float GetFOV() const { return m_fov; }

    void SetAspectRatio(float f_ratio);
    inline float GetAspectRatio() const { return m_aspectRatio; }

    void SetOrthoParams(const glm::vec4 &f_size);
    inline const glm::vec4& GetOrthoParams() const { return m_orthoParams; }

    void SetDepth(const glm::vec2 &f_depth);
    inline const glm::vec2& GetDepth() const { return m_depth; }

    void SetPosition(const glm::vec3 &f_pos);
    inline const glm::vec3& GetPosition() const { return m_viewPosition; }

    void SetDirection(const glm::vec3 &f_dir);
    void SetDirection(const glm::quat &f_dir);
    inline const glm::vec3& GetDirection() const { return m_viewDirection; }

    void SetUpDirection(const glm::vec3 &f_dir);
    inline const glm::vec3& GetUpDirection() const { return m_upDirection; }

    inline const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
    inline const glm::mat4& GetProjectionMatrix() const { return m_projectionMatrix; }
    inline const glm::mat4& GetViewProjectionMatrix() const { return m_viewProjectionMatrix; }

    bool IsInFrustum(const glm::vec3 &f_pos, float f_radius);
    bool IsInFrustum(const glm::mat4 &f_mat, float f_radius);
protected:

    explicit Camera(int f_type);
    ~Camera();

    static void SetVRSystem(vr::IVRSystem *f_system);

    void Update();

    friend class ElementManager;
    friend class VRManager;
    friend class Scene;
};

}
