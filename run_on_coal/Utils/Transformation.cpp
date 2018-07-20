#include "stdafx.h"
#include "Utils/Transformation.h"

namespace ROC
{

extern const glm::vec3 g_DefaultPosition;
extern const glm::vec3 g_DefaultScale;
extern const glm::quat g_DefaultRotation;
extern const glm::mat4 g_IdentityMatrix;
extern const float g_Epsilon;

}

ROC::Transformation::Transformation()
{
    m_position = g_DefaultPosition;
    m_rotation = g_DefaultRotation;
    m_scale = g_DefaultScale;
    m_matrix = g_IdentityMatrix;
    m_useScale = false;
    m_update = false;
    m_updated = false;
}
ROC::Transformation::~Transformation()
{
}

void ROC::Transformation::SetPosition(const glm::vec3 &f_pos)
{
    if(m_position != f_pos)
    {
        std::memcpy(&m_position, &f_pos, sizeof(glm::vec3));
        m_update = true;
    }
}

void ROC::Transformation::SetRotation(const glm::quat &f_rot)
{
    if(m_rotation != f_rot)
    {
        std::memcpy(&m_rotation, &f_rot, sizeof(glm::quat));
        m_update = true;
    }
}

void ROC::Transformation::SetScale(const glm::vec3 &f_scl)
{
    if(m_scale != f_scl)
    {
        std::memcpy(&m_scale, &f_scl, sizeof(glm::vec3));
        for(int i = 0; i < 3; i++)
        {
            if(glm::epsilonNotEqual(m_scale[i], g_DefaultScale[i], g_Epsilon))
            {
                m_useScale = true;
                break;
            }
        }
        m_update = true;
    }
}

void ROC::Transformation::Inverse()
{
    UpdateMatrix();
    m_matrix = glm::inverse(m_matrix);

    glm::vec3 l_dummyVec3;
    glm::vec4 l_dummyVec4;
    glm::decompose(m_matrix, m_scale, m_rotation, m_position, l_dummyVec3, l_dummyVec4);
}

void ROC::Transformation::GetMatrix(glm::mat4 &f_mat)
{
    std::memcpy(&f_mat, &m_matrix, sizeof(glm::mat4));
}

void ROC::Transformation::UpdateMatrix()
{
    m_updated = false;
    if(m_update)
    {
        btTransform l_transform = btTransform::getIdentity();
        l_transform.setOrigin(btVector3(m_position.x, m_position.y, m_position.z));
        l_transform.setRotation(btQuaternion(m_rotation.x, m_rotation.y, m_rotation.z, m_rotation.w));
        l_transform.getOpenGLMatrix(glm::value_ptr(m_matrix));
        if(m_useScale) m_matrix *= glm::scale(g_IdentityMatrix, m_scale);
        m_update = false;
        m_updated = true;
    }
}
