#include "stdafx.h"

#include "Elements/Geometry/Material.h"
#include "Elements/Texture.h"

#include "Utils/GLBinder.h"

ROC::Material::Material()
{
    m_verticesCount = 0;

    for(size_t i = 0U; i < MBI_BufferCount; i++) m_VBO[i] = 0U;
    m_VAO = 0U;

    m_params = glm::vec4(1.f);
    m_type = 0;

    m_texture = nullptr;
}

ROC::Material::~Material()
{
    for(size_t i = 0U; i < MBI_BufferCount; i++)
    {
        if(m_VBO[i] != 0U)
        {
            GLBinder::ResetArrayBuffer(m_VBO[i]);
            glDeleteBuffers(1, &m_VBO[i]);
        }
    }
    if(m_VAO != 0U)
    {
        GLBinder::ResetVertexArray(m_VAO);
        glDeleteVertexArrays(1, &m_VAO);
    }
    delete m_texture;
}

void ROC::Material::LoadVertices(const std::vector<glm::vec3> &f_vector)
{
    if(m_VBO[MBI_Vertex] == 0U)
    {
        m_verticesCount = static_cast<int>(f_vector.size());
        glGenBuffers(1, &m_VBO[MBI_Vertex]);
        GLBinder::BindArrayBuffer(m_VBO[MBI_Vertex]);
        glBufferData(GL_ARRAY_BUFFER, f_vector.size()*sizeof(glm::vec3), f_vector.data(), GL_STATIC_DRAW);
    }
}
void ROC::Material::LoadUVs(const std::vector<glm::vec2> &f_vector)
{
    if(m_VBO[MBI_UV] == 0U)
    {
        glGenBuffers(1, &m_VBO[MBI_UV]);
        GLBinder::BindArrayBuffer(m_VBO[MBI_UV]);
        glBufferData(GL_ARRAY_BUFFER, f_vector.size()*sizeof(glm::vec2), f_vector.data(), GL_STATIC_DRAW);
    }
}
void ROC::Material::LoadNormals(const std::vector<glm::vec3> &f_vector)
{
    if(m_VBO[MBI_Normal] == 0U)
    {
        glGenBuffers(1, &m_VBO[MBI_Normal]);
        GLBinder::BindArrayBuffer(m_VBO[MBI_Normal]);
        glBufferData(GL_ARRAY_BUFFER, f_vector.size()*sizeof(glm::vec3), f_vector.data(), GL_STATIC_DRAW);
    }
}
void ROC::Material::LoadWeights(const std::vector<glm::vec4> &f_vector)
{
    if(m_VBO[MBI_Weight] == 0U)
    {
        glGenBuffers(1, &m_VBO[MBI_Weight]);
        GLBinder::BindArrayBuffer(m_VBO[MBI_Weight]);
        glBufferData(GL_ARRAY_BUFFER, f_vector.size()*sizeof(glm::vec4), f_vector.data(), GL_STATIC_DRAW);
    }
}
void ROC::Material::LoadIndices(const std::vector<glm::ivec4> &f_vector)
{
    if(m_VBO[MBI_WeightIndex] == 0U)
    {
        glGenBuffers(1, &m_VBO[MBI_WeightIndex]);
        GLBinder::BindArrayBuffer(m_VBO[MBI_WeightIndex]);
        glBufferData(GL_ARRAY_BUFFER, f_vector.size()*sizeof(glm::ivec4), f_vector.data(), GL_STATIC_DRAW);
    }
}

void ROC::Material::GenerateVAO()
{
    if(m_VAO == 0U)
    {
        glGenVertexArrays(1, &m_VAO);
        GLBinder::BindVertexArray(m_VAO);

        if(m_VBO[MBI_Vertex] != 0U)
        {
            glEnableVertexAttribArray(MBI_Vertex);
            GLBinder::BindArrayBuffer(m_VBO[MBI_Vertex]);
            glVertexAttribPointer(MBI_Vertex, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        }
        if(m_VBO[MBI_UV] != 0U)
        {
            glEnableVertexAttribArray(MBI_UV);
            GLBinder::BindArrayBuffer(m_VBO[MBI_UV]);
            glVertexAttribPointer(MBI_UV, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        }
        if(m_VBO[MBI_Normal] != 0U)
        {
            glEnableVertexAttribArray(MBI_Normal);
            GLBinder::BindArrayBuffer(m_VBO[MBI_Normal]);
            glVertexAttribPointer(MBI_Normal, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        }
        if(m_VBO[MBI_Weight] != 0U)
        {
            glEnableVertexAttribArray(MBI_Weight);
            GLBinder::BindArrayBuffer(m_VBO[MBI_Weight]);
            glVertexAttribPointer(MBI_Weight, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        }
        if(m_VBO[MBI_WeightIndex] != 0U)
        {
            glEnableVertexAttribArray(MBI_WeightIndex);
            GLBinder::BindArrayBuffer(m_VBO[MBI_WeightIndex]);
            glVertexAttribIPointer(MBI_WeightIndex, 4, GL_INT, 0, NULL);
        }
    }
}
void ROC::Material::LoadTexture(const std::string &f_path)
{
    if(!m_texture)
    {
        m_texture = new Texture();
        if(!m_texture->Load(f_path, IsTransparent() ? Texture::TT_RGBA : Texture::TT_RGB, GetFilteringType(), IsCompressed()))
        {
            delete m_texture;
            m_texture = nullptr;
        }
    }
}

void ROC::Material::Draw()
{
    if(m_VAO != 0U)
    {
        GLBinder::BindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, m_verticesCount);
    }
}
