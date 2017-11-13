#include "stdafx.h"

#include "Managers/RenderManager/Quad2D.h"

#include "Utils/GLBinder.h"

namespace ROC
{

extern const float g_QuadVertexUV[];

}

ROC::Quad2D::Quad2D()
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_vertexVBO);
    glGenBuffers(1, &m_uvVBO);

    glBindVertexArray(m_VAO);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * ROC_QUAD2D_VERTEX_COUNT, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_uvVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2U * ROC_QUAD2D_VERTEX_COUNT, g_QuadVertexUV, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    for(auto &iter : m_vertex) iter = glm::vec3(0.f, 0.f, 1.0f);
}
ROC::Quad2D::~Quad2D()
{
    glDeleteBuffers(1, &m_vertexVBO);
    glDeleteBuffers(1, &m_uvVBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void ROC::Quad2D::Bind()
{
    GLBinder::BindVertexArray(m_VAO);
    GLBinder::BindArrayBuffer(m_vertexVBO);
}
void ROC::Quad2D::SetTransformation(const glm::vec2 &f_size)
{
    if(m_size != f_size)
    {
        std::memcpy(&m_size, &f_size, sizeof(glm::vec2));
        m_vertex[0].y = m_vertex[3].y = m_vertex[5].y = m_size.y;
        m_vertex[2].x = m_vertex[4].x = m_vertex[5].x = m_size.x;
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * ROC_QUAD2D_VERTEX_COUNT, m_vertex);
    }
}
