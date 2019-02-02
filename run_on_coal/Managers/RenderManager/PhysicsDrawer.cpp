#include "stdafx.h"

#include "Managers/RenderManager/PhysicsDrawer.h"

#include "Utils/GLBinder.h"

#if defined _WIN64
#define ROC_PHYSICSDRAWER_MAX_LINES 65536ULL
#elif defined _WIN32
#define ROC_PHYSICSDRAWER_MAX_LINES 65536U
#else
#define ROC_PHYSICSDRAWER_MAX_LINES 65536
#endif

ROC::PhysicsDrawer::PhysicsDrawer()
{
    glGenVertexArrays(1, &m_VAO);
    GLBinder::BindVertexArray(m_VAO);

    glGenBuffers(static_cast<int>(PDBI_BufferCount), m_VBO);

    GLBinder::BindArrayBuffer(m_VBO[PDBI_Vertex]);
    glBufferData(GL_ARRAY_BUFFER, ROC_PHYSICSDRAWER_MAX_LINES*sizeof(glm::vec3), NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(PDBI_Vertex);
    glVertexAttribPointer(PDBI_Vertex, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    GLBinder::BindArrayBuffer(m_VBO[PDBI_Color]);
    glBufferData(GL_ARRAY_BUFFER, ROC_PHYSICSDRAWER_MAX_LINES*sizeof(glm::vec3), NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(PDBI_Color);
    glVertexAttribPointer(PDBI_Color, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}
ROC::PhysicsDrawer::~PhysicsDrawer()
{
    for(size_t i = 0U; i < PDBI_BufferCount; i++) GLBinder::ResetArrayBuffer(m_VBO[i]);
    glDeleteBuffers(static_cast<int>(PDBI_BufferCount), m_VBO);

    if(m_VAO != 0U)
    {
        GLBinder::ResetVertexArray(m_VAO);
        glDeleteVertexArrays(1, &m_VAO);
    }
}

void ROC::PhysicsDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
    glm::vec3 l_vec;

    std::memcpy(&l_vec, from.m_floats, sizeof(glm::vec3));
    m_lines.push_back(l_vec);
    std::memcpy(&l_vec, to.m_floats, sizeof(glm::vec3));
    m_lines.push_back(l_vec);

    std::memcpy(&l_vec, color.m_floats, sizeof(glm::vec3));
    m_colors.insert(m_colors.end(), 2U, l_vec);
}

void ROC::PhysicsDrawer::Draw(float f_width)
{
    if(!m_lines.empty())
    {
        if(f_width != 1.f) glLineWidth(f_width);

        GLBinder::BindVertexArray(m_VAO);
        while(!m_lines.empty())
        {
            size_t l_count = std::min(ROC_PHYSICSDRAWER_MAX_LINES, m_lines.size()); // Count is same for m_colors

            GLBinder::BindArrayBuffer(m_VBO[PDBI_Vertex]);
            glBufferSubData(GL_ARRAY_BUFFER, 0, l_count*sizeof(glm::vec3), m_lines.data());

            GLBinder::BindArrayBuffer(m_VBO[PDBI_Color]);
            glBufferSubData(GL_ARRAY_BUFFER, 0, l_count*sizeof(glm::vec3), m_colors.data());

            glDrawArrays(GL_LINES, 0, static_cast<int>(l_count));

            m_lines.erase(m_lines.begin(), m_lines.begin() + l_count);
            m_colors.erase(m_colors.begin(), m_colors.begin() + l_count);
        }

        if(f_width != 1.f) glLineWidth(1.f);
    }
}
