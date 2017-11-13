#include "stdafx.h"

#include "Managers/RenderManager/Quad3D.h"

#include "Utils/GLBinder.h"

namespace ROC
{

extern const glm::mat4 g_IdentityMatrix;
extern const glm::vec3 g_DefaultPosition;
extern const glm::quat g_DefaultRotation;

extern const float g_QuadVertexUV[];

}

ROC::Quad3D::Quad3D()
{
    m_position = g_DefaultPosition;
    m_rotation = g_DefaultRotation;
    m_size = glm::vec2(0.f);
    m_matrix = g_IdentityMatrix;

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_vertexVBO);
    glGenBuffers(1, &m_uvVBO);
    glGenBuffers(1, &m_normalVBO);

    glBindVertexArray(m_VAO);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * ROC_QUAD3D_VERTEX_COUNT, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_uvVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2U * ROC_QUAD3D_VERTEX_COUNT, g_QuadVertexUV, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    std::vector<glm::vec3> l_normals(ROC_QUAD3D_VERTEX_COUNT, glm::vec3(0.f, 0.f, 1.f));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, m_normalVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*ROC_QUAD3D_VERTEX_COUNT, l_normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    for(auto &iter : m_vertex) iter.z = 0.f;
}
ROC::Quad3D::~Quad3D()
{
    glDeleteBuffers(1, &m_vertexVBO);
    glDeleteBuffers(1, &m_uvVBO);
    glDeleteBuffers(1, &m_normalVBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void ROC::Quad3D::Bind()
{
    GLBinder::BindVertexArray(m_VAO);
    GLBinder::BindArrayBuffer(m_vertexVBO);
}
void ROC::Quad3D::SetTransformation(const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec2 &f_size)
{
    if(f_pos != m_position || f_rot != m_rotation)
    {
        std::memcpy(&m_position, &f_pos, sizeof(glm::vec3));
        std::memcpy(&m_rotation, &f_rot, sizeof(glm::quat));

        btTransform l_transform = btTransform::getIdentity();
        btVector3 l_position(m_position.x, m_position.y, m_position.z);
        btQuaternion l_rotation(m_rotation.x, m_rotation.y, m_rotation.z, m_rotation.w);
        l_transform.setOrigin(l_position);
        l_transform.setRotation(l_rotation);
        l_transform.getOpenGLMatrix(glm::value_ptr(m_matrix));
    }
    if(f_size != m_size)
    {
        std::memcpy(&m_size, &f_size, sizeof(glm::vec2));
        m_vertex[0].x = m_vertex[1].x = m_vertex[3].x = -m_size.x / 2.f;
        m_vertex[0].y = m_vertex[3].y = m_vertex[5].y = m_size.y / 2.f;
        m_vertex[2].x = m_vertex[4].x = m_vertex[5].x = m_size.x / 2.f;
        m_vertex[1].y = m_vertex[2].y = m_vertex[4].y = -m_size.y / 2.f;
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3)*ROC_QUAD3D_VERTEX_COUNT, m_vertex);
    }
}
