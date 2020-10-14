#include "stdafx.h"

#include "Elements/Geometry/Material.h"
#include "Elements/Texture.h"
#include "GL/GLArrayBuffer.h"
#include "GL/GLVertexArray.h"

#include "GL/GLSetting.h"

ROC::Texture *ROC::Material::ms_dummyTexture = nullptr;

ROC::Material::Material()
{
    m_verticesCount = 0;

    for(size_t i = 0U; i < MBI_BufferCount; i++) m_arrayBuffers[i] = nullptr;
    m_vertexArray = nullptr;

    m_params = glm::vec4(1.f);
    m_type = 0;

    m_texture = nullptr;
}

ROC::Material::~Material()
{
    for(size_t i = 0U; i < MBI_BufferCount; i++)
    {
        if(m_arrayBuffers[i])
        {
            m_arrayBuffers[i]->Destroy();
            delete m_arrayBuffers[i];
        }
    }
    if(m_vertexArray)
    {
        m_vertexArray->Destroy();
        delete m_vertexArray;
    }
    delete m_texture;
}

void ROC::Material::LoadVertices(const std::vector<glm::vec3> &f_vector)
{
    if(!m_arrayBuffers[MBI_Vertex])
    {
        m_verticesCount = static_cast<int>(f_vector.size());

        m_arrayBuffers[MBI_Vertex] = new GLArrayBuffer();
        m_arrayBuffers[MBI_Vertex]->Create(f_vector.size()*sizeof(glm::vec3), f_vector.data(), GL_STATIC_DRAW);
    }
}

void ROC::Material::LoadNormals(const std::vector<glm::vec3> &f_vector)
{
    if(!m_arrayBuffers[MBI_Normal])
    {
        m_arrayBuffers[MBI_Normal] = new GLArrayBuffer();
        m_arrayBuffers[MBI_Normal]->Create(f_vector.size()*sizeof(glm::vec3), f_vector.data(), GL_STATIC_DRAW);
    }
}

void ROC::Material::LoadUVs(const std::vector<glm::vec2> &f_vector)
{
    if(!m_arrayBuffers[MBI_UV])
    {
        m_arrayBuffers[MBI_UV] = new GLArrayBuffer();
        m_arrayBuffers[MBI_UV]->Create(f_vector.size()*sizeof(glm::vec2), f_vector.data(), GL_STATIC_DRAW);
    }
}

void ROC::Material::LoadWeights(const std::vector<glm::vec4> &f_vector)
{
    if(!m_arrayBuffers[MBI_Weight])
    {
        m_arrayBuffers[MBI_Weight] = new GLArrayBuffer();
        m_arrayBuffers[MBI_Weight]->Create(f_vector.size()*sizeof(glm::vec4), f_vector.data(), GL_STATIC_DRAW);
    }
}

void ROC::Material::LoadIndices(const std::vector<glm::ivec4> &f_vector)
{
    if(!m_arrayBuffers[MBI_WeightIndex])
    {
        m_arrayBuffers[MBI_WeightIndex] = new GLArrayBuffer();
        m_arrayBuffers[MBI_WeightIndex]->Create(f_vector.size()*sizeof(glm::ivec4), f_vector.data(), GL_STATIC_DRAW);
    }
}

void ROC::Material::Generate()
{
    if(!m_vertexArray)
    {
        m_vertexArray = new GLVertexArray();
        m_vertexArray->Create();
        m_vertexArray->Bind();

        if(m_arrayBuffers[MBI_Vertex])
        {
            m_arrayBuffers[MBI_Vertex]->Bind();
            m_vertexArray->EnableAttribute(MBI_Vertex, 3, GL_FLOAT);
        }
        if(m_arrayBuffers[MBI_Normal])
        {
            m_arrayBuffers[MBI_Normal]->Bind();
            m_vertexArray->EnableAttribute(MBI_Normal, 3, GL_FLOAT);
        }
        if(m_arrayBuffers[MBI_UV])
        {
            m_arrayBuffers[MBI_UV]->Bind();
            m_vertexArray->EnableAttribute(MBI_UV, 2, GL_FLOAT);
        }
        if(m_arrayBuffers[MBI_Weight])
        {
            m_arrayBuffers[MBI_Weight]->Bind();
            m_vertexArray->EnableAttribute(MBI_Weight, 4, GL_FLOAT);
        }
        if(m_arrayBuffers[MBI_WeightIndex])
        {
            m_arrayBuffers[MBI_WeightIndex]->Bind();
            m_vertexArray->EnableAttribute(MBI_WeightIndex, 4, GL_INT);
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

void ROC::Material::SetType(unsigned char f_type)
{
    m_type = f_type;
}

unsigned char ROC::Material::GetType() const
{
    return m_type;
}

void ROC::Material::SetParams(const glm::vec4 &f_params)
{
    std::memcpy(&m_params, &f_params, sizeof(glm::vec4));
}

const glm::vec4& ROC::Material::GetParams() const
{
    return m_params;
}

bool ROC::Material::IsDoubleSided() const
{
    return ((m_type&MPB_Doubleside) != 0U);
}

bool ROC::Material::IsTransparent() const
{
    return ((m_type&MPB_Transparency) != 0U);
}

bool ROC::Material::IsShady() const
{
    return ((m_type&MPB_Shading) != 0U);
}

bool ROC::Material::HasDepth() const
{
    return ((m_type&MPB_Depth) != 0U);
}

bool ROC::Material::IsCompressed() const
{
    return ((m_type&MPB_Compression) != 0U);
}

unsigned char ROC::Material::GetFilteringType() const
{
    return ((m_type&MPB_Filtering) >> 4);
}

ROC::Texture* ROC::Material::GetTexture() const
{
    return m_texture;
}

void ROC::Material::Draw(bool f_useTexture)
{
    if(m_vertexArray)
    {
        GLSetting::Set(GL_BLEND, IsTransparent());
        GLSetting::Set(GL_CULL_FACE, !IsDoubleSided());
        GLSetting::SetDepthMask(HasDepth());

        if(f_useTexture && m_texture) m_texture->Bind();
        else ms_dummyTexture->Bind();

        m_vertexArray->Bind();
        m_vertexArray->Draw(GL_TRIANGLES, m_verticesCount);
    }
}

// Static
void ROC::Material::InitStaticResources()
{
    if(!ms_dummyTexture)
    {
        ms_dummyTexture = new Texture();
        ms_dummyTexture->LoadDummy();
    }
}

void ROC::Material::ReleaseStaticResources()
{
    if(ms_dummyTexture)
    {
        delete ms_dummyTexture;
        ms_dummyTexture = nullptr;
    }
}
