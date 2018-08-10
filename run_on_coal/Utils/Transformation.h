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
    inline const glm::vec3& GetPosition() const { return m_position; }

    void SetRotation(const glm::quat &f_rot);
    inline const glm::quat& GetRotation() const { return m_rotation; }

    void SetScale(const glm::vec3 &f_scl);
    inline const glm::vec3& GetScale() const { return m_scale; }
    inline bool IsScaled() const { return m_useScale; }

    inline bool IsUpdated() const { return m_updated; }
    void UpdateMatrix();
    void GetMatrix(glm::mat4 &f_mat) const;
    inline const glm::mat4& GetMatrix() const { return m_matrix; }
};

}
