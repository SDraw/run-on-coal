#pragma once

namespace ROC
{

class Quad
{
    glm::vec3 m_point[6];
    glm::vec2 m_size;

    GLuint m_vertexVBO;
    GLuint m_uvVBO;
    GLuint m_VAO;
protected:
    Quad();
    ~Quad();

    void SetProportions(glm::vec2 &f_size, bool f_bind);
    inline GLuint GetVAO() const { return m_VAO; }
    static inline void Draw() { glDrawArrays(GL_TRIANGLES, 0, 6); }

    friend class RenderManager;
};

}
