#pragma once

namespace ROC
{

class Quad3D final
{
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec2 m_size;
    glm::mat4 m_matrix;

    glm::vec3 m_vertex[6];
    GLuint m_VBO[3];
    GLuint m_VAO;
protected:
    Quad3D();
    ~Quad3D();

    inline glm::mat4& GetMatrixRef() { return m_matrix; }

    void SetTransformation(const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec2 &f_size);
    void Draw();

    friend class RenderManager;
};

}
