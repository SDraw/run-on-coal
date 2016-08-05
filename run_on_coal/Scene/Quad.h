#pragma once

namespace ROC
{

class Quad
{
    glm::vec3 m_point[6];
    glm::vec2 m_size;

    GLuint m_vertexVBO;
    GLuint m_uvVBO;
protected:
    GLuint m_VAO;
    Quad();
    ~Quad();
    void SetProportions(glm::vec2 &f_size, bool f_bind);
    void Draw(bool f_bind);
    friend class RenderManager;
};

}
