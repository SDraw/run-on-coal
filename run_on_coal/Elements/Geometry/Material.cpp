#include "stdafx.h"

#include "Elements/Geometry/Material.h"
#include "Elements/Texture.h"

#include "Utils/GLBinder.h"

#define ROC_MATERIAL_BUFFER_COUNT 5U
#define ROC_MATERIAL_VERTEX_BUFFER 0U
#define ROC_MATERIAL_UV_BUFFER 1U
#define ROC_MATERIAL_NORMAL_BUFFER 2U
#define ROC_MATERIAL_WEIGHT_BUFFER 3U
#define ROC_MATERIAL_INDEX_BUFFER 4U

ROC::Material::Material()
{
    m_verticesCount = 0;

    for(size_t i = 0U; i < ROC_MATERIAL_BUFFER_COUNT; i++) m_VBO[i] = 0U;
    m_VAO = 0U;

    m_params = glm::vec4(1.f);
    m_type = 0;

    m_texture = nullptr;
}

ROC::Material::~Material()
{
    for(auto iter : m_VBO)
    {
        if(iter != 0U) glDeleteBuffers(1, &iter);
    }
    if(m_VAO != 0U)
    {
        if(GLBinder::IsVertexArrayBinded(m_VAO)) GLBinder::ResetVertexArray();
        glDeleteVertexArrays(1, &m_VAO);
    }
    delete m_texture;
}

void ROC::Material::LoadVertices(const std::vector<glm::vec3> &f_vector)
{
    if(m_VBO[ROC_MATERIAL_VERTEX_BUFFER] == 0U)
    {
        m_verticesCount = static_cast<int>(f_vector.size());
        glGenBuffers(1, &m_VBO[ROC_MATERIAL_VERTEX_BUFFER]);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO[ROC_MATERIAL_VERTEX_BUFFER]);
        glBufferData(GL_ARRAY_BUFFER, f_vector.size()*sizeof(glm::vec3), f_vector.data(), GL_STATIC_DRAW);
    }
}
void ROC::Material::LoadUVs(const std::vector<glm::vec2> &f_vector)
{
    if(m_VBO[ROC_MATERIAL_UV_BUFFER] == 0U)
    {
        glGenBuffers(1, &m_VBO[ROC_MATERIAL_UV_BUFFER]);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO[ROC_MATERIAL_UV_BUFFER]);
        glBufferData(GL_ARRAY_BUFFER, f_vector.size()*sizeof(glm::vec2), f_vector.data(), GL_STATIC_DRAW);
    }
}
void ROC::Material::LoadNormals(const std::vector<glm::vec3> &f_vector)
{
    if(m_VBO[ROC_MATERIAL_NORMAL_BUFFER] == 0U)
    {
        glGenBuffers(1, &m_VBO[ROC_MATERIAL_NORMAL_BUFFER]);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO[ROC_MATERIAL_NORMAL_BUFFER]);
        glBufferData(GL_ARRAY_BUFFER, f_vector.size()*sizeof(glm::vec3), f_vector.data(), GL_STATIC_DRAW);
    }
}
void ROC::Material::LoadWeights(const std::vector<glm::vec4> &f_vector)
{
    if(m_VBO[ROC_MATERIAL_WEIGHT_BUFFER] == 0U)
    {
        glGenBuffers(1, &m_VBO[ROC_MATERIAL_WEIGHT_BUFFER]);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO[ROC_MATERIAL_WEIGHT_BUFFER]);
        glBufferData(GL_ARRAY_BUFFER, f_vector.size()*sizeof(glm::vec4), f_vector.data(), GL_STATIC_DRAW);
    }
}
void ROC::Material::LoadIndices(const std::vector<glm::ivec4> &f_vector)
{
    if(m_VBO[ROC_MATERIAL_INDEX_BUFFER] == 0U)
    {
        glGenBuffers(1, &m_VBO[ROC_MATERIAL_INDEX_BUFFER]);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO[ROC_MATERIAL_INDEX_BUFFER]);
        glBufferData(GL_ARRAY_BUFFER, f_vector.size()*sizeof(glm::ivec4), f_vector.data(), GL_STATIC_DRAW);
    }
}

void ROC::Material::GenerateVAO()
{
    if(m_VAO == 0U)
    {
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        if(m_VBO[ROC_MATERIAL_VERTEX_BUFFER] != 0U)
        {
            glEnableVertexAttribArray(ROC_MATERIAL_VERTEX_BUFFER);
            glBindBuffer(GL_ARRAY_BUFFER, m_VBO[ROC_MATERIAL_VERTEX_BUFFER]);
            glVertexAttribPointer(ROC_MATERIAL_VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        }
        if(m_VBO[ROC_MATERIAL_UV_BUFFER] != 0U)
        {
            glEnableVertexAttribArray(ROC_MATERIAL_UV_BUFFER);
            glBindBuffer(GL_ARRAY_BUFFER, m_VBO[ROC_MATERIAL_UV_BUFFER]);
            glVertexAttribPointer(ROC_MATERIAL_UV_BUFFER, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        }
        if(m_VBO[ROC_MATERIAL_NORMAL_BUFFER] != 0U)
        {
            glEnableVertexAttribArray(ROC_MATERIAL_NORMAL_BUFFER);
            glBindBuffer(GL_ARRAY_BUFFER, m_VBO[ROC_MATERIAL_NORMAL_BUFFER]);
            glVertexAttribPointer(ROC_MATERIAL_NORMAL_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        }
        if(m_VBO[ROC_MATERIAL_WEIGHT_BUFFER] != 0U)
        {
            glEnableVertexAttribArray(ROC_MATERIAL_WEIGHT_BUFFER);
            glBindBuffer(GL_ARRAY_BUFFER, m_VBO[ROC_MATERIAL_WEIGHT_BUFFER]);
            glVertexAttribPointer(ROC_MATERIAL_WEIGHT_BUFFER, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        }
        if(m_VBO[ROC_MATERIAL_INDEX_BUFFER] != 0U)
        {
            glEnableVertexAttribArray(ROC_MATERIAL_INDEX_BUFFER);
            glBindBuffer(GL_ARRAY_BUFFER, m_VBO[ROC_MATERIAL_INDEX_BUFFER]);
            glVertexAttribIPointer(ROC_MATERIAL_INDEX_BUFFER, 4, GL_INT, 0, NULL);
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
