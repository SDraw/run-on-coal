#pragma once

namespace ROC
{

class Quad3D final
{
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec2 m_size;
    glm::mat4 m_matrix;

    enum QuadBufferIndex : size_t
    {
        QBI_Vertex = 0U,
        QBI_UV,
        QBI_Normal,

        QBI_BufferCount
    };
    glm::vec3 m_vertex[6];
    GLuint m_VBO[QBI_BufferCount];
    GLuint m_VAO;

    Quad3D(const Quad3D &that);
    Quad3D& operator=(const Quad3D &that);
protected:
    Quad3D();
    ~Quad3D();

    inline const glm::mat4& GetMatrix() const { return m_matrix; }

    void SetTransformation(const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec2 &f_size);
    void Draw();

    friend class RenderManager;
};

}
