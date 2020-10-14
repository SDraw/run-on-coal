#pragma once

class GLArrayBuffer;
class GLVertexArray;

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
        QBI_Normal,
        QBI_UV,

        QBI_BufferCount
    };
    glm::vec3 m_vertex[6];
    GLArrayBuffer *m_arrayBuffers[QBI_BufferCount];
    GLVertexArray *m_vertexArray;

    Quad3D(const Quad3D &that) = delete;
    Quad3D& operator=(const Quad3D &that) = delete;
protected:
    Quad3D();
    ~Quad3D();

    const glm::mat4& GetMatrix() const;

    void SetTransformation(const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec2 &f_size);
    void Draw();

    friend class RenderManager;
};

}
