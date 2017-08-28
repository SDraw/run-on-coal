#pragma once

#define ROC_QUAD_VERTEX_COUNT 6U

namespace ROC
{

class Quad final
{
    glm::vec3 m_vertex[ROC_QUAD_VERTEX_COUNT];
    glm::vec2 m_size;

    GLuint m_vertexVBO;
    GLuint m_uvVBO;
    GLuint m_VAO;
protected:
    Quad();
    ~Quad();

    inline GLuint GetVAO() const { return m_VAO; }

    void Bind();
    void SetTransformation(const glm::vec2 &f_size);

    static inline void Draw() { glDrawArrays(GL_TRIANGLES, 0, ROC_QUAD_VERTEX_COUNT); }

    friend class RenderManager;
};

}
