#pragma once
#define CAMERA_PROJECTION_PERSPECTIVE 0U
#define CAMERA_PROJECTION_ORTHOGONAL 1U

namespace ROC
{
class Camera
{
    unsigned char m_type;

    float m_fov;
    float m_aspectRatio;
    glm::vec4 m_orthoParams;
    glm::vec2 m_depth;

    glm::vec4 m_planes[6];

    bool m_rebuildView;
    bool m_rebuildProjection;
public:
    inline void SetType(unsigned char f_type) { m_type = f_type; }
    inline unsigned char GetType() { return m_type; }

    void SetFOV(float f_fov);
    inline float GetFOV() { return m_fov; }
    void SetAspectRatio(float f_ratio);
    inline float GetAspectRatio() { return m_aspectRatio; }
    void SetOrthoParams(glm::vec4 &f_size);
    inline void GetOrthoParams(glm::vec4 &f_size) { std::memcpy(&f_size, &m_orthoParams, sizeof(glm::vec4)); }
    void SetDepth(glm::vec2 &f_depth);
    inline void GetDepth(glm::vec2 &f_depth) { std::memcpy(&f_depth, &m_depth, sizeof(glm::vec2)); }
    void SetPosition(glm::vec3 &f_pos);
    inline void GetPosition(glm::vec3 &f_pos) { std::memcpy(&f_pos, &m_viewPosition, sizeof(glm::vec3)); }
    void SetDirection(glm::vec3 &f_dir);
    inline void GetDirection(glm::vec3 &f_dir) { std::memcpy(&f_dir, &m_viewDirection, sizeof(glm::vec3)); }

    void GetViewMatrix(glm::mat4 &f_mat);
    void GetProjectionMatrix(glm::mat4 &f_mat);

    bool IsInFrustum(glm::vec3 &f_pos, float f_radius);
protected:
    glm::vec3 m_viewPosition;
    glm::vec3 m_viewDirection;
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;

    explicit Camera(unsigned char f_type);
    ~Camera();

    void UpdateMatrices();
    friend class ElementManager;
    friend class RenderManager;
};
}
