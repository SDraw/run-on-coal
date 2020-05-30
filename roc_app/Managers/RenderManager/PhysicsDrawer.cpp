#include "stdafx.h"

#include "Managers/RenderManager/PhysicsDrawer.h"
#include "GL/GLArrayBuffer.h"
#include "GL/GLVertexArray.h"
#include "GL/GLSetting.h"
#include "GL/GLViewport.h"

namespace ROC
{

const size_t g_PhysicsDrawerMaxLinesCount = 65536U;

}

ROC::PhysicsDrawer::PhysicsDrawer()
{
    m_vertexArray = new GLVertexArray();
    m_vertexArray->Create();
    m_vertexArray->Bind();

    for(size_t i = 0U; i < PDBI_BufferCount; i++) m_arrayBuffers[i] = new GLArrayBuffer();

    m_arrayBuffers[PDBI_Vertex]->Create(g_PhysicsDrawerMaxLinesCount*sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);
    m_arrayBuffers[PDBI_Vertex]->Bind();
    m_vertexArray->EnableAttribute(PDBI_Vertex, 3, GL_FLOAT);

    m_arrayBuffers[PDBI_Color]->Create(g_PhysicsDrawerMaxLinesCount*sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);
    m_arrayBuffers[PDBI_Color]->Bind();
    m_vertexArray->EnableAttribute(PDBI_Color, 3, GL_FLOAT);
}

ROC::PhysicsDrawer::~PhysicsDrawer()
{
    for(size_t i = 0U; i < PDBI_BufferCount; i++)
    {
        m_arrayBuffers[i]->Destroy();
        delete m_arrayBuffers[i];
    }

    if(m_vertexArray)
    {
        m_vertexArray->Destroy();
        delete m_vertexArray;
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

void ROC::PhysicsDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
    glm::vec3 l_vec;
    std::memcpy(&l_vec, PointOnB.m_floats, sizeof(glm::vec3));
    m_lines.push_back(l_vec);

    glm::vec3 l_normal;
    std::memcpy(&l_normal, normalOnB.m_floats, sizeof(glm::vec3));
    l_vec += l_normal;
    m_lines.push_back(l_vec);

    std::memcpy(&l_vec, color.m_floats, sizeof(glm::vec3));
    m_colors.insert(m_colors.end(), 2U, l_vec);
}

void ROC::PhysicsDrawer::Draw(float f_width)
{
    if(!m_lines.empty())
    {
        GLSetting::Set(GL_CULL_FACE, false);
        GLSetting::SetDepthMask(true);
        GLSetting::Set(GL_BLEND, false);
        GLViewport::SetLineWidth(f_width);

        m_vertexArray->Bind();
        while(!m_lines.empty())
        {
            const size_t l_count = std::min(g_PhysicsDrawerMaxLinesCount, m_lines.size()); // Count is same for m_colors
            const size_t l_dataSize = l_count*sizeof(glm::vec3);

            m_arrayBuffers[PDBI_Vertex]->Bind();
            m_arrayBuffers[PDBI_Vertex]->Update(0, l_dataSize, m_lines.data());

            m_arrayBuffers[PDBI_Color]->Bind();
            m_arrayBuffers[PDBI_Color]->Update(0, l_dataSize, m_colors.data());

            m_vertexArray->Draw(GL_LINES, static_cast<GLsizei>(l_count));

            m_lines.erase(m_lines.begin(), m_lines.begin() + l_count);
            m_colors.erase(m_colors.begin(), m_colors.begin() + l_count);
        }
    }
}
