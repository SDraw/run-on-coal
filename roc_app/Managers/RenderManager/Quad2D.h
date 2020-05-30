#pragma once

class GLArrayBuffer;
class GLVertexArray;

namespace ROC
{

class Quad2D final
{
    glm::vec3 m_vertex[6];
    glm::vec2 m_size;

    enum QuadBufferIndex : size_t
    {
        QBI_Vertex = 0U,
        QBI_UV,

        QBI_BufferCount
    };
    enum QuadBufferAttribute : size_t
    {
        QBA_Vertex = 0U,
        QBA_UV = 2U
    };
    GLArrayBuffer *m_arrayBuffers[QBI_BufferCount];
    GLVertexArray *m_vertexArray;

    Quad2D(const Quad2D &that) = delete;
    Quad2D& operator=(const Quad2D &that) = delete;
protected:
    Quad2D();
    ~Quad2D();

    void SetTransformation(const glm::vec2 &f_size);
    void Draw();

    friend class RenderManager;
};

}
