#include "stdafx.h"

#include "Managers/RenderManager/Quad.h"

namespace ROC
{

const float g_QuadVertexUV[] = {
    0.f, 0.f, 0.f, 1.f, 1.f, 1.f,
    0.f, 0.f, 1.f, 1.f, 1.f, 0.f
};

}

ROC::Quad::Quad()
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_vertexVBO);
    glGenBuffers(1, &m_uvVBO);

    glBindVertexArray(m_VAO);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * ROC_QUAD_VERTEX_COUNT, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_uvVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_QuadVertexUV), g_QuadVertexUV, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    for(auto &iter : m_vertex) iter = glm::vec3(0.f, 0.f, 1.0f);
}
ROC::Quad::~Quad()
{
    glDeleteBuffers(1, &m_vertexVBO);
    glDeleteBuffers(1, &m_uvVBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void ROC::Quad::Bind()
{
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
}
void ROC::Quad::SetTransformation(const glm::vec2 &f_size)
{
    if(m_size != f_size)
    {
        std::memcpy(&m_size, &f_size, sizeof(glm::vec2));
        m_vertex[0].y = m_vertex[3].y = m_vertex[5].y = m_size.y;
        m_vertex[2].x = m_vertex[4].x = m_vertex[5].x = m_size.x;
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * ROC_QUAD_VERTEX_COUNT, m_vertex);
    }
}
