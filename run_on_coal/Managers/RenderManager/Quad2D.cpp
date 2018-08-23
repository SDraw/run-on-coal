#include "stdafx.h"

#include "Managers/RenderManager/Quad2D.h"

#include "Utils/GLBinder.h"

namespace ROC
{

extern const float g_QuadVertexUV[];

}

#define ROC_QUAD2D_VERTEX_COUNT 6U

ROC::Quad2D::Quad2D()
{
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(static_cast<int>(QBI_BufferCount), m_VBO);

    glEnableVertexAttribArray(QBA_Vertex);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[QBI_Vertex]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * ROC_QUAD2D_VERTEX_COUNT, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(QBA_Vertex, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(QBA_UV);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[QBI_UV]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2U * ROC_QUAD2D_VERTEX_COUNT, g_QuadVertexUV, GL_STATIC_DRAW);
    glVertexAttribPointer(QBA_UV, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    for(auto &iter : m_vertex) iter = glm::vec3(0.f, 0.f, 1.0f);
}
ROC::Quad2D::~Quad2D()
{
    glDeleteBuffers(static_cast<int>(QBI_BufferCount), m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void ROC::Quad2D::SetTransformation(const glm::vec2 &f_size)
{
    if(m_size != f_size)
    {
        std::memcpy(&m_size, &f_size, sizeof(glm::vec2));
        m_vertex[0].y = m_vertex[3].y = m_vertex[5].y = m_size.y;
        m_vertex[2].x = m_vertex[4].x = m_vertex[5].x = m_size.x;

        GLBinder::BindArrayBuffer(m_VBO[QBI_Vertex]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * ROC_QUAD2D_VERTEX_COUNT, m_vertex);
    }
}
void ROC::Quad2D::Draw()
{
    GLBinder::BindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, ROC_QUAD2D_VERTEX_COUNT);
}
