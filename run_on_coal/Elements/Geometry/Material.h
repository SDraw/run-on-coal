#pragma once

#define CHECK_BIT(val,bit) ((val&bit) == bit)

#define ROC_MATERIAL_BIT_SHADING 1U
#define ROC_MATERIAL_BIT_DEPTH 2U
#define ROC_MATERIAL_BIT_TRANSPARENCY 4U
#define ROC_MATERIAL_BIT_DOUBLESIDE 8U
#define ROC_MATERIAL_BIT_FILTER 16U
#define ROC_MATERIAL_BIT_COMPRESSION 32U

namespace ROC
{

class Texture;
class Material final
{
    unsigned int m_verticesCount;
    GLuint m_vertexVBO;
    GLuint m_uvVBO;
    GLuint m_normalVBO;
    GLuint m_weightVBO;
    GLuint m_indexVBO;
    GLuint m_VAO;

    unsigned char m_type;
    glm::vec4 m_params;
    Texture *m_texture;
public:
    inline unsigned char GetType() const { return m_type; }

    inline bool IsDoubleSided() const { return CHECK_BIT(m_type, ROC_MATERIAL_BIT_DOUBLESIDE); }
    inline bool IsTransparent() const { return CHECK_BIT(m_type, ROC_MATERIAL_BIT_TRANSPARENCY); }
    inline bool IsShady() const { return CHECK_BIT(m_type, ROC_MATERIAL_BIT_SHADING); }
    inline bool HasDepth() const { return CHECK_BIT(m_type, ROC_MATERIAL_BIT_DEPTH); }
    inline bool IsCompressed() const { return CHECK_BIT(m_type, ROC_MATERIAL_BIT_COMPRESSION); }
    inline unsigned char GetFilteringType() const { return ((m_type&ROC_MATERIAL_BIT_FILTER) >> 4); }
protected:
    Material();
    ~Material();

    void LoadVertices(const std::vector<glm::vec3> &f_vector);
    void LoadUVs(const std::vector<glm::vec2> &f_vector);
    void LoadNormals(const std::vector<glm::vec3> &f_vector);
    void LoadWeights(const std::vector<glm::vec4> &f_vector);
    void LoadIndices(const std::vector<glm::ivec4> &f_vector);
    void LoadTexture(const std::string &f_path);
    void GenerateVAO();

    inline void SetType(unsigned char f_type) { m_type = f_type; }

    inline void SetParams(const glm::vec4 &f_params) { std::memcpy(&m_params, &f_params, sizeof(glm::vec4)); }
    inline glm::vec4& GetParamsRef() { return m_params; }

    inline GLuint GetVAO() const { return m_VAO; }
    inline Texture* GetTexture() { return m_texture; }

    void Draw(const glm::bvec2 &f_bind);

    friend class RenderManager;
    friend class Geometry;
};

}
