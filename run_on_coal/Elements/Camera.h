#pragma once
#include "Elements/Element.h"
#define ROC_CAMERA_PROJECTION_PERSPECTIVE 0
#define ROC_CAMERA_PROJECTION_ORTHOGONAL 1

namespace ROC
{

class Camera final : public Element
{
    int m_type;

    glm::vec3 m_viewPosition;
    glm::vec3 m_viewDirection;
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;
    bool m_rebuildView;
    bool m_rebuildProjection;

    float m_fov;
    float m_aspectRatio;
    glm::vec4 m_orthoParams;
    glm::vec2 m_depth;

    glm::vec4 m_planes[6];
public:
    void SetProjectionType(int f_type);
    inline int GetProjectionType() const { return m_type; }

    void SetFOV(float f_fov);
    inline float GetFOV() const { return m_fov; }

    void SetAspectRatio(float f_ratio);
    inline float GetAspectRatio() const { return m_aspectRatio; }

    void SetOrthoParams(const glm::vec4 &f_size);
    inline void GetOrthoParams(glm::vec4 &f_size) { std::memcpy(&f_size, &m_orthoParams, sizeof(glm::vec4)); }

    void SetDepth(const glm::vec2 &f_depth);
    inline void GetDepth(glm::vec2 &f_depth) { std::memcpy(&f_depth, &m_depth, sizeof(glm::vec2)); }

    void SetPosition(const glm::vec3 &f_pos);
    inline void GetPosition(glm::vec3 &f_pos) { std::memcpy(&f_pos, &m_viewPosition, sizeof(glm::vec3)); }

    void SetDirection(const glm::vec3 &f_dir);
    inline void GetDirection(glm::vec3 &f_dir) { std::memcpy(&f_dir, &m_viewDirection, sizeof(glm::vec3)); }

    void GetViewMatrix(glm::mat4 &f_mat);
    void GetProjectionMatrix(glm::mat4 &f_mat);

    bool IsInFrustum(const glm::vec3 &f_pos, float f_radius);
protected:
    explicit Camera(int f_type);
    ~Camera();

    inline glm::vec3& GetPositionRef() { return m_viewPosition; }
    inline glm::vec3& GetDirectionRef() { return m_viewDirection; }

    void UpdateMatrices();
    inline glm::mat4& GetProjectionMatrixRef() { return m_projectionMatrix; }
    inline glm::mat4& GetViewMatrixRef() { return m_viewMatrix; }

    friend class ElementManager;
    friend class RenderManager;
};

}
