#include "stdafx.h"

#include "Managers/RenderManager/Quad2D.h"
#include "GL/GLArrayBuffer.h"
#include "GL/GLVertexArray.h"
#include "GL/GLSetting.h"

namespace ROC
{

extern const float g_quadVertexUV[];
const size_t g_quad2DVerticesCount = 6U;

}

ROC::Quad2D::Quad2D()
{
    m_vertexArray = new GLVertexArray();
    m_vertexArray->Create();
    m_vertexArray->Bind();

    for(size_t i = 0U; i < QBI_BufferCount; i++) m_arrayBuffers[i] = new GLArrayBuffer();

    m_arrayBuffers[QBI_Vertex]->Create(sizeof(glm::vec3) * g_quad2DVerticesCount, nullptr, GL_DYNAMIC_DRAW);
    m_arrayBuffers[QBI_Vertex]->Bind();
    m_vertexArray->EnableAttribute(QBA_Vertex, 3, GL_FLOAT);

    m_arrayBuffers[QBI_UV]->Create(sizeof(glm::vec2) * g_quad2DVerticesCount, g_quadVertexUV, GL_STATIC_DRAW);
    m_arrayBuffers[QBI_UV]->Bind();
    m_vertexArray->EnableAttribute(QBA_UV, 2, GL_FLOAT);

    for(auto &l_vertex : m_vertex) l_vertex = glm::vec3(0.f, 0.f, 1.0f);
}

ROC::Quad2D::~Quad2D()
{
    for(size_t i = 0U; i < QBI_BufferCount; i++)
    {
        m_arrayBuffers[i]->Destroy();
        delete m_arrayBuffers[i];
    }

    m_vertexArray->Destroy();
    delete m_vertexArray;
}

void ROC::Quad2D::SetTransformation(const glm::vec2 &f_size)
{
    if(m_size != f_size)
    {
        std::memcpy(&m_size, &f_size, sizeof(glm::vec2));
        m_vertex[0].y = m_vertex[3].y = m_vertex[5].y = m_size.y;
        m_vertex[2].x = m_vertex[4].x = m_vertex[5].x = m_size.x;

        m_arrayBuffers[QBI_Vertex]->Bind();
        m_arrayBuffers[QBI_Vertex]->Update(0, sizeof(glm::vec3) * g_quad2DVerticesCount, m_vertex);
    }
}

void ROC::Quad2D::Draw()
{
    GLSetting::Set(GL_CULL_FACE, false);
    GLSetting::SetDepthMask(false);

    m_vertexArray->Bind();
    m_vertexArray->Draw(GL_TRIANGLES, g_quad2DVerticesCount);
}
