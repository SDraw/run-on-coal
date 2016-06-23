#pragma once
namespace ROC
{

class Camera
{
    glm::vec3 m_viewPosition;
    glm::vec3 m_viewDirection;
    glm::mat4 m_viewMatrix,m_projectionMatrix;
    
    bool m_rebuildViewMatrix;
    void UpdateViewMatrix();
public:
    void SetPerspective(float f_fov, float f_width, float f_height, float f_near, float f_far);
    void SetOrtho(float l_left,float l_right,float f_bottom,float f_top,float l_near,float l_far);

    void SetPosition(glm::vec3 &f_pos);
    void GetPosition(glm::vec3 &f_pos);
    void SetDirection(glm::vec3 &f_dir);
    void GetDirection(glm::vec3 &f_dir);

    void GetViewMatrix(glm::mat4 &f_mat);
    void GetProjectionMatrix(glm::mat4 &f_mat);
protected:
    Camera();
    ~Camera();
    friend class ElementManager;
};

}
