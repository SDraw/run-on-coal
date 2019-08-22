#pragma once
#include "Interfaces/ICamera.h"
#include "Elements/Element.h"

namespace ROC
{

class Camera final : public Element, public ICamera
{
    unsigned char m_type;

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

    Camera(const Camera &that) = delete;
    Camera& operator=(const Camera &that) = delete;
public:
    void SetProjectionType(unsigned char f_type);
    unsigned char GetProjectionType() const;

    void SetFOV(float f_fov);
    float GetFOV() const;

    void SetAspectRatio(float f_ratio);
    float GetAspectRatio() const;

    void SetOrthoParams(const glm::vec4 &f_size);
    const glm::vec4& GetOrthoParams() const;

    void SetDepth(const glm::vec2 &f_depth);
    const glm::vec2& GetDepth() const;

    void SetPosition(const glm::vec3 &f_pos);
    const glm::vec3& GetPosition() const;

    void SetDirection(const glm::vec3 &f_dir);
    void SetDirection(const glm::quat &f_dir);
    const glm::vec3& GetDirection() const;

    void SetUpDirection(const glm::vec3 &f_dir);
    const glm::vec3& GetUpDirection() const;

    const glm::mat4& GetViewMatrix() const;
    const glm::mat4& GetProjectionMatrix() const;
    const glm::mat4& GetViewProjectionMatrix() const;

    bool IsInFrustum(const glm::vec3 &f_pos, float f_radius);
    bool IsInFrustum(const glm::mat4 &f_mat, float f_radius);
protected:

    explicit Camera(unsigned char f_type);
    ~Camera();

    static void SetVRSystem(vr::IVRSystem *f_system);

    void Update();

    friend class ElementManager;
    friend class VRManager;
    friend class Scene;
};

}
