#include "stdafx.h"
#include "Managers/RenderManager/Quad.h"

ROC::Quad::Quad()
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_vertexVBO);
    glGenBuffers(1, &m_uvVBO);

    glBindVertexArray(m_VAO);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_uvVBO);
    GLfloat l_uvs[6][2] = {
        { 0.0, 0.0 }, { 0.0, 1.0 }, { 1.0, 1.0 },
        { 0.0, 0.0 }, { 1.0, 1.0 }, { 1.0, 0.0 }
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(l_uvs), l_uvs, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    for(auto &iter : m_point) iter.z = 0.5f;
}
ROC::Quad::~Quad()
{
    glDeleteBuffers(1, &m_vertexVBO);
    glDeleteBuffers(1, &m_uvVBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void ROC::Quad::SetProportions(glm::vec2 &f_size, bool f_bind)
{
    if(f_bind)
    {
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
    }
    if(!std::memcmp(&m_size, &f_size, sizeof(glm::vec2))) return;
    std::memcpy(&m_size, &f_size, sizeof(glm::vec2));
    m_point[0].x = m_point[1].x = m_point[3].x = -m_size.x / 2.f;
    m_point[0].y = m_point[3].y = m_point[5].y = m_size.y / 2.f;
    m_point[2].x = m_point[4].x = m_point[5].x = m_size.x / 2.f;
    m_point[1].y = m_point[2].y = m_point[4].y = -m_size.y / 2.f;
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_point), m_point);
}

void ROC::Quad::Draw(bool f_bind)
{
    if(f_bind) glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
