#pragma once

namespace ROC
{

class Transformation
{
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;
    glm::mat4 m_matrix;
    bool m_useScale;
    bool m_update;
    bool m_updated;
public:
    Transformation();
    ~Transformation();

    void SetPosition(const glm::vec3 &f_pos);
    const glm::vec3& GetPosition() const;

    void SetRotation(const glm::quat &f_rot);
    const glm::quat& GetRotation() const;

    void SetScale(const glm::vec3 &f_scl);
    const glm::vec3& GetScale() const;
    bool IsScaled() const;

    bool IsUpdated() const;
    void UpdateMatrix();
    void GetMatrix(glm::mat4 &f_mat) const;
    const glm::mat4& GetMatrix() const;
};

}
