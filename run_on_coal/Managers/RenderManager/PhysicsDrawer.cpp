#include "stdafx.h"

#include "Managers/RenderManager/PhysicsDrawer.h"

#include "Utils/GLBinder.h"

#define ROC_PHYSICSDRAWER_MAX_LINES 65536U
#define ROC_PHYSICSDRAWER_VERTEX_BUFFER 0U

ROC::PhysicsDrawer::PhysicsDrawer()
{
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, ROC_PHYSICSDRAWER_MAX_LINES*sizeof(glm::vec3), NULL, GL_DYNAMIC_DRAW);

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glEnableVertexAttribArray(ROC_PHYSICSDRAWER_VERTEX_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glVertexAttribPointer(ROC_PHYSICSDRAWER_VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}
ROC::PhysicsDrawer::~PhysicsDrawer()
{
    if(m_VBO != 0U) glDeleteBuffers(1, &m_VBO);
    if(m_VAO != 0U) glDeleteVertexArrays(1, &m_VAO);
}

void ROC::PhysicsDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
    if(m_lines.size() < ROC_PHYSICSDRAWER_MAX_LINES)
    {
        glm::vec3 l_start(from.x(), from.y(), from.z());
        glm::vec3 l_stop(to.x(), to.y(), to.z());
        m_lines.push_back(l_start);
        m_lines.push_back(l_stop);
    }
}

void ROC::PhysicsDrawer::Draw()
{
    size_t l_count = m_lines.size();
    if(l_count > 0U)
    {
        GLBinder::BindArrayBuffer(m_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, l_count*sizeof(glm::vec3), m_lines.data());

        GLBinder::BindVertexArray(m_VAO);
        glDrawArrays(GL_LINES, 0, static_cast<int>(l_count));
    }
}

void ROC::PhysicsDrawer::ClearStoredLines()
{
    m_lines.clear();
}
