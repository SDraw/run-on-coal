#pragma once
namespace ROC
{

class Camera
{
    glm::vec3 m_viewPosition;
    glm::vec3 m_viewDirection;
    glm::mat4 m_viewMatrix,m_projectionMatrix;

    glm::vec4 m_planes[6];
    
    bool m_rebuildViewMatrix;
    bool m_rebuildFrustumPlanes;
    inline void UpdateViewMatrix() { m_viewMatrix = glm::lookAt(m_viewPosition,m_viewPosition+m_viewDirection,glm::vec3(0.0f,1.0f,0.0f)); }
public:
    void SetPerspective(float f_fov, float f_width, float f_height, float f_near, float f_far);
    void SetOrtho(float l_left,float l_right,float f_bottom,float f_top,float f_near,float f_far);

    void SetPosition(glm::vec3 &f_pos);
    void GetPosition(glm::vec3 &f_pos);
    void SetDirection(glm::vec3 &f_dir);
    void GetDirection(glm::vec3 &f_dir);

    void GetViewMatrix(glm::mat4 &f_mat);
    inline void GetProjectionMatrix(glm::mat4 &f_mat) { std::memcpy(&f_mat,&m_projectionMatrix,sizeof(glm::mat4)); }
protected:
    Camera();
    ~Camera();
    void UpdateFrustumPlanes();
    bool IsInFrustum(glm::vec3 &f_pos, float f_radius);
    friend class ElementManager;
    friend class RenderManager;
};

}
