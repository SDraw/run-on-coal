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
    bool m_bVertexVBO;
    GLuint m_uvVBO;
    bool m_bUvVBO;
    GLuint m_normalVBO;
    bool m_bNormalVBO;
    GLuint m_weightVBO;
    bool m_bWeightVBO;
    GLuint m_indexVBO;
    bool m_bIndexVBO;
    GLuint m_VAO;
    bool m_bVAO;

    unsigned char m_type;
    glm::vec4 m_params;
    Texture *m_texture;
protected:
    Material();
    ~Material();

    void LoadVertices(std::vector<glm::vec3> &f_vector);
    void LoadUVs(std::vector<glm::vec2> &f_vector);
    void LoadNormals(std::vector<glm::vec3> &f_vector);
    void LoadWeights(std::vector<glm::vec4> &f_vector);
    void LoadIndices(std::vector<glm::ivec4> &f_vector);
    void LoadTexture(std::string &f_path);
    void GenerateVAO();
    inline void SetType(unsigned int f_type) { m_type = f_type; }
    inline void SetParams(glm::vec4 &f_params) { std::memcpy(&m_params, &f_params, sizeof(glm::vec4)); }

    inline GLuint GetVAO() { return m_VAO; }
    inline unsigned char GetType() { return m_type; }
    inline glm::vec4& GetParamsRef() { return m_params; }
    inline Texture* GetTexture() { return m_texture; }

    void Draw(bool f_texturize, bool f_binding);
    inline bool IsDoubleSided() { return ((m_type&MATERIAL_BIT_DOUBLESIDE) == MATERIAL_BIT_DOUBLESIDE); }
    inline bool IsTransparent() { return ((m_type&MATERIAL_BIT_TRANSPARENT) == MATERIAL_BIT_TRANSPARENT); }
    inline bool IsShady() { return ((m_type&MATERIAL_BIT_SHADING) == MATERIAL_BIT_SHADING); }
    inline bool IsDepthable() { return ((m_type&MATERIAL_BIT_DEPTH) == MATERIAL_BIT_DEPTH); }
    inline unsigned char GetFilteringType() { return ((m_type&MATERIAL_BIT_FILTER) >> 4); }
    inline bool IsCompressed() { return ((m_type&MATERIAL_BIT_COMPRESSION) == MATERIAL_BIT_COMPRESSION); }
    friend class RenderManager;
    friend class Geometry;
};

}
