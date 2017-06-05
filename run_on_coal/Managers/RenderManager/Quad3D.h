#pragma once
#define ROC_QUAD3D_VERTEX_COUNT 6U

namespace ROC
{

class Quad3D
{
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec2 m_size;
    glm::mat4 m_matrix;

    glm::vec3 m_vertex[ROC_QUAD3D_VERTEX_COUNT];
    GLuint m_vertexVBO;
    GLuint m_uvVBO;
    GLuint m_normalVBO;
    GLuint m_VAO;
protected:
    Quad3D();
    ~Quad3D();

    inline glm::mat4& GetMatrixRef() { return m_matrix; }
    inline GLuint GetVAO() const { return m_VAO; }

    void Bind();
    void SetTransformation(const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec2 &f_size);

    static inline void Draw() { glDrawArrays(GL_TRIANGLES, 0, ROC_QUAD3D_VERTEX_COUNT); }

    friend class RenderManager;
};

}
