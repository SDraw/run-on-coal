#include "stdafx.h"

#include "Elements/Geometry/Material.h"
#include "Elements/Texture.h"

ROC::Material::Material()
{
    m_verticesCount = 0;

    m_vertexVBO = 0U;
    m_uvVBO = 0U;
    m_normalVBO = 0U;
    m_weightVBO = 0U;
    m_indexVBO = 0U;
    m_VAO = 0U;

    m_params = glm::vec4(1.f);
    m_type = 0;

    m_texture = nullptr;
}

ROC::Material::~Material()
{
    if(m_vertexVBO != 0U) glDeleteBuffers(1, &m_vertexVBO);
    if(m_uvVBO != 0U) glDeleteBuffers(1, &m_uvVBO);
    if(m_normalVBO != 0U) glDeleteBuffers(1, &m_normalVBO);
    if(m_weightVBO != 0U) glDeleteBuffers(1, &m_weightVBO);
    if(m_indexVBO != 0U) glDeleteBuffers(1, &m_indexVBO);
    if(m_VAO != 0U) glDeleteVertexArrays(1, &m_VAO);
    delete m_texture;
}

void ROC::Material::LoadVertices(const std::vector<glm::vec3> &f_vector)
{
    if(m_vertexVBO == 0U)
    {
        m_verticesCount = f_vector.size();
        glGenBuffers(1, &m_vertexVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
        glBufferData(GL_ARRAY_BUFFER, m_verticesCount*sizeof(glm::vec3), f_vector.data(), GL_STATIC_DRAW);
    }
}
void ROC::Material::LoadUVs(const std::vector<glm::vec2> &f_vector)
{
    if(m_uvVBO == 0U)
    {
        glGenBuffers(1, &m_uvVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_uvVBO);
        glBufferData(GL_ARRAY_BUFFER, f_vector.size()*sizeof(glm::vec2), f_vector.data(), GL_STATIC_DRAW);
    }
}
void ROC::Material::LoadNormals(const std::vector<glm::vec3> &f_vector)
{
    if(m_normalVBO == 0U)
    {
        glGenBuffers(1, &m_normalVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_normalVBO);
        glBufferData(GL_ARRAY_BUFFER, f_vector.size()*sizeof(glm::vec3), f_vector.data(), GL_STATIC_DRAW);
    }
}
void ROC::Material::LoadWeights(const std::vector<glm::vec4> &f_vector)
{
    if(m_weightVBO == 0U)
    {
        glGenBuffers(1, &m_weightVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_weightVBO);
        glBufferData(GL_ARRAY_BUFFER, f_vector.size()*sizeof(glm::vec4), f_vector.data(), GL_STATIC_DRAW);
    }
}
void ROC::Material::LoadIndices(const std::vector<glm::ivec4> &f_vector)
{
    if(m_indexVBO == 0U)
    {
        glGenBuffers(1, &m_indexVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_indexVBO);
        glBufferData(GL_ARRAY_BUFFER, f_vector.size()*sizeof(glm::ivec4), f_vector.data(), GL_STATIC_DRAW);
    }
}

void ROC::Material::GenerateVAO()
{
    if(m_VAO == 0U)
    {
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        if(m_vertexVBO != 0U)
        {
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        }
        if(m_uvVBO != 0U)
        {
            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, m_uvVBO);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        }
        if(m_normalVBO != 0U)
        {
            glEnableVertexAttribArray(2);
            glBindBuffer(GL_ARRAY_BUFFER, m_normalVBO);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        }
        if(m_weightVBO != 0U)
        {
            glEnableVertexAttribArray(3);
            glBindBuffer(GL_ARRAY_BUFFER, m_weightVBO);
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        }
        if(m_indexVBO != 0U)
        {
            glEnableVertexAttribArray(4);
            glBindBuffer(GL_ARRAY_BUFFER, m_indexVBO);
            glVertexAttribIPointer(4, 4, GL_INT, 0, NULL);
        }
        glBindVertexArray(NULL);
    }
}
void ROC::Material::LoadTexture(const std::string &f_path)
{
    if(!m_texture)
    {
        m_texture = new Texture();
        if(!m_texture->Load(f_path, IsTransparent() ? ROC_TEXTURE_TYPE_RGBA : ROC_TEXTURE_TYPE_RGB, GetFilteringType(), IsCompressed()))
        {
            delete m_texture;
            m_texture = nullptr;
        }
    }
}

void ROC::Material::Draw(bool f_bind)
{
    if(m_VAO != 0U)
    {
        if(f_bind) glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, m_verticesCount);
    }
}
