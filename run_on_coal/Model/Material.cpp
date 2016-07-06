#include "stdafx.h"
#include "Model/Material.h"
#include "Scene/Texture.h"

ROC::Material::Material()
{
    m_verticesCount = 0;

    m_bVertexVBO = false;
    m_bUvVBO = false;
    m_bNormalVBO = false;
    m_bWeightVBO = false;
    m_bIndexVBO = false;
    m_bVAO = false;

    m_texture = NULL;

    m_params = glm::vec4(1.f);
    m_type = 0;
}

ROC::Material::~Material()
{
    if(m_bVertexVBO) glDeleteBuffers(1,&m_vertexVBO);
    if(m_bUvVBO) glDeleteBuffers(1,&m_uvVBO);
    if(m_bNormalVBO) glDeleteBuffers(1,&m_normalVBO);
    if(m_bWeightVBO) glDeleteBuffers(1,&m_weightVBO);
    if(m_bIndexVBO) glDeleteBuffers(1,&m_indexVBO);
    if(m_bVAO) glDeleteVertexArrays(1,&m_VAO);
    if(m_texture) delete m_texture;
}

void ROC::Material::SetType(unsigned char f_type)
{
    m_type = f_type;
}
unsigned char ROC::Material::GetType()
{
    return m_type;
}
bool ROC::Material::IsDoubleSided()
{
    return ((m_type&8U) == 8U);
}
bool ROC::Material::IsTransparent()
{
    return ((m_type&4U) == 4U);
}
bool ROC::Material::IsShady()
{
    return ((m_type&1U) == 1U);
}
bool ROC::Material::IsDepthable()
{
    return ((m_type&2U) == 2U);
}
void ROC::Material::GetParams(glm::vec4 &f_params)
{
    std::memcpy(&f_params,&m_params,sizeof(glm::vec4));
}
void ROC::Material::SetParams(glm::vec4 &f_params)
{
    std::memcpy(&m_params,&f_params,sizeof(glm::vec4));
}

void ROC::Material::LoadTexture(std::string &f_path, bool f_compressed)
{
    m_texture = new Texture();
    if(!m_texture->Load(f_path, IsTransparent() ? Texture::TextureType::RGBA:Texture::TextureType::RGB,f_compressed))
    {
        delete m_texture;
        m_texture = NULL;
    }
}

void ROC::Material::LoadVertices(std::vector<glm::vec3> &f_vector)
{
    if(m_bVertexVBO) return;
    m_verticesCount = f_vector.size();
    glGenBuffers (1, &m_vertexVBO);
    glBindBuffer (GL_ARRAY_BUFFER, m_vertexVBO);
    glBufferData (GL_ARRAY_BUFFER, m_verticesCount*sizeof(glm::vec3),f_vector.data(),GL_STATIC_DRAW);
    m_bVertexVBO = true;
}
void ROC::Material::LoadUVs(std::vector<glm::vec2> &f_vector)
{
    if(m_bUvVBO) return;
    glGenBuffers (1, &m_uvVBO);
    glBindBuffer (GL_ARRAY_BUFFER, m_uvVBO);
    glBufferData (GL_ARRAY_BUFFER, f_vector.size()*sizeof(glm::vec2),f_vector.data(),GL_STATIC_DRAW);
    m_bUvVBO = true;
}
void ROC::Material::LoadNormals(std::vector<glm::vec3> &f_vector)
{
    if(m_bNormalVBO) return;
    glGenBuffers (1, &m_normalVBO);
    glBindBuffer (GL_ARRAY_BUFFER, m_normalVBO);
    glBufferData (GL_ARRAY_BUFFER, f_vector.size()*sizeof(glm::vec3),f_vector.data(),GL_STATIC_DRAW);
    m_bNormalVBO = true;
}
void ROC::Material::LoadWeights(std::vector<glm::vec4> &f_vector)
{
    if(m_bWeightVBO) return;
    glGenBuffers (1, &m_weightVBO);
    glBindBuffer (GL_ARRAY_BUFFER, m_weightVBO);
    glBufferData (GL_ARRAY_BUFFER, f_vector.size()*sizeof(glm::vec4),f_vector.data(),GL_STATIC_DRAW);
    m_bWeightVBO = true;
}
void ROC::Material::LoadIndices(std::vector<glm::ivec4> &f_vector)
{
    if(m_bIndexVBO) return;
    glGenBuffers (1, &m_indexVBO);
    glBindBuffer (GL_ARRAY_BUFFER, m_indexVBO);
    glBufferData (GL_ARRAY_BUFFER, f_vector.size()*sizeof(glm::ivec4),f_vector.data(), GL_STATIC_DRAW);
    m_bIndexVBO = true;
}

void ROC::Material::GenerateVAO()
{
    glGenVertexArrays (1, &m_VAO);
    glBindVertexArray (m_VAO);

    if(m_bVertexVBO)
    {
        glEnableVertexAttribArray (0);
        glBindBuffer (GL_ARRAY_BUFFER, m_vertexVBO);
        glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    }
    if(m_bUvVBO)
    {
        glEnableVertexAttribArray (1);
        glBindBuffer (GL_ARRAY_BUFFER, m_uvVBO);
        glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    }
    if(m_bNormalVBO)
    {
        glEnableVertexAttribArray (2);
        glBindBuffer (GL_ARRAY_BUFFER, m_normalVBO);
        glVertexAttribPointer (2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    }
    if(m_bWeightVBO)
    {
        glEnableVertexAttribArray (3);
        glBindBuffer (GL_ARRAY_BUFFER, m_weightVBO);
        glVertexAttribPointer (3, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    }
    if(m_bIndexVBO)
    {
        glEnableVertexAttribArray (4);
        glBindBuffer (GL_ARRAY_BUFFER, m_indexVBO);
        glVertexAttribIPointer (4, 4, GL_UNSIGNED_INT, 0, NULL);
    }
    glBindVertexArray (NULL);

    m_bVAO = true;
}

void ROC::Material::Draw(bool f_texturize, bool f_binding)
{
    if(!m_bVAO) return;
    if(f_texturize)
    {
        if(m_texture) m_texture->Bind(0);
    }
    if(f_binding) glBindVertexArray (m_VAO);
    glDrawArrays(GL_TRIANGLES,0,m_verticesCount);
}

GLuint ROC::Material::GetVAO()
{
    return m_VAO;
}
GLuint ROC::Material::GetTexture()
{
    if(!m_texture) return 0U;
    return m_texture->GetTexture();
}
