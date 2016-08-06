#pragma once
#define CAMERA_PROJECTION_PERSPECTIVE 0U
#define CAMERA_PROJECTION_ORTHOGONAL 1U

namespace ROC
{

class Camera
{
    float m_fov;
    glm::vec2 m_perspectiveSize;
    glm::vec4 m_orthogonalSize;
    glm::vec2 m_depth;
    unsigned char m_type;

    glm::vec4 m_planes[6];

    bool m_rebuildView;
    bool m_rebuildProjection;
public:
    inline void SetType(unsigned char f_type) { m_type = f_type; }
    inline unsigned char GetType() { return m_type; }

    void SetFOV(float f_fov);
    inline float GetFOV() { return m_fov; }

    void SetPerspectiveSize(glm::vec2 &f_size);
    inline void GetPerspectiveSize(glm::vec2 &f_size) { std::memcpy(&f_size,&m_perspectiveSize,sizeof(glm::vec2)); }

    void SetOrthoSize(glm::vec4 &f_size);
    inline void GetOrthoSize(glm::vec4 &f_size) { std::memcpy(&f_size,&m_orthogonalSize,sizeof(glm::vec4)); }

    void SetDepth(glm::vec2 &f_depth);
    inline void GetDepth(glm::vec2 &f_depth) { std::memcpy(&f_depth,&m_depth,sizeof(glm::vec2)); }

    void SetPosition(glm::vec3 &f_pos);
    inline void GetPosition(glm::vec3 &f_pos) { std::memcpy(&f_pos,&m_viewPosition,sizeof(glm::vec3)); }

    void SetDirection(glm::vec3 &f_dir);
    inline void GetDirection(glm::vec3 &f_dir) { std::memcpy(&f_dir,&m_viewDirection,sizeof(glm::vec3)); }

    bool IsInFrustum(glm::vec3 &f_pos, float f_radius);

    void GetViewMatrix(glm::mat4 &f_mat);
    void GetProjectionMatrix(glm::mat4 &f_mat);
protected:
    glm::vec3 m_viewPosition;
    glm::vec3 m_viewDirection;

    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;

    Camera(unsigned char f_type);
    ~Camera();
    void UpdateMatrices();
    friend class ElementManager;
    friend class RenderManager;
};

}
