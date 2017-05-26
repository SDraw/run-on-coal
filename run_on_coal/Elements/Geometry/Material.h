#pragma once
#define MATERIAL_BIT_SHADING 1U
#define MATERIAL_BIT_DEPTH 2U
#define MATERIAL_BIT_TRANSPARENT 4U
#define MATERIAL_BIT_DOUBLESIDE 8U
#define MATERIAL_BIT_FILTER 16U
#define MATERIAL_BIT_COMPRESSION 32U

namespace ROC
{

class Texture;
class Material
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

    inline bool IsDoubleSided() const { return ((m_type&MATERIAL_BIT_DOUBLESIDE) == MATERIAL_BIT_DOUBLESIDE); }
    inline bool IsTransparent() const { return ((m_type&MATERIAL_BIT_TRANSPARENT) == MATERIAL_BIT_TRANSPARENT); }
    inline bool IsShady() const { return ((m_type&MATERIAL_BIT_SHADING) == MATERIAL_BIT_SHADING); }
    inline bool IsDepthable() const { return ((m_type&MATERIAL_BIT_DEPTH) == MATERIAL_BIT_DEPTH); }
    inline bool IsCompressed() const { return ((m_type&MATERIAL_BIT_COMPRESSION) == MATERIAL_BIT_COMPRESSION); }
    inline unsigned char GetFilteringType() const { return ((m_type&MATERIAL_BIT_FILTER) >> 4); }
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

    inline void SetType(unsigned int f_type) { m_type = f_type; }

    inline void SetParams(const glm::vec4 &f_params) { std::memcpy(&m_params, &f_params, sizeof(glm::vec4)); }
    inline glm::vec4& GetParamsRef() { return m_params; }

    inline GLuint GetVAO() const { return m_VAO; }
    inline Texture* GetTexture() { return m_texture; }

    void Draw(bool f_texturize, bool f_binding);

    friend class RenderManager;
    friend class Geometry;
};

}
