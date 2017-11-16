#pragma once

namespace ROC
{

class Quad2D final
{
    glm::vec3 m_vertex[6];
    glm::vec2 m_size;

    GLuint m_VBO[2];
    GLuint m_VAO;
protected:
    Quad2D();
    ~Quad2D();

    void SetTransformation(const glm::vec2 &f_size);
    void Draw();

    friend class RenderManager;
};

}
