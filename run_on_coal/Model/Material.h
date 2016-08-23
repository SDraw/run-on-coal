#pragma once
#define MATERIAL_BIT_SHADING 1U
#define MATERIAL_BIT_DEPTH 2U
#define MATERIAL_BIT_TRANSPARENT 4U
#define MATERIAL_BIT_DOUBLESIDE 8U

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
    bool m_bVAO;
protected:
    Texture *m_texture;
    GLuint m_VAO;
    unsigned char m_type;
    glm::vec4 m_params;
    Material();
    ~Material();
    inline bool IsDoubleSided() { return ((m_type&MATERIAL_BIT_DOUBLESIDE)==MATERIAL_BIT_DOUBLESIDE); }
    inline bool IsTransparent() { return ((m_type&MATERIAL_BIT_TRANSPARENT)==MATERIAL_BIT_TRANSPARENT); }
    inline bool IsShady() { return ((m_type&MATERIAL_BIT_SHADING)==MATERIAL_BIT_SHADING); }
    inline bool IsDepthable() { return ((m_type&MATERIAL_BIT_DEPTH)==MATERIAL_BIT_DEPTH); }
    void LoadVertices(std::vector<glm::vec3> &f_vector);
    void LoadUVs(std::vector<glm::vec2> &f_vector);
    void LoadNormals(std::vector<glm::vec3> &f_vector);
    void LoadWeights(std::vector<glm::vec4> &f_vector);
    void LoadIndices(std::vector<glm::ivec4> &f_vector);
    void LoadTexture(std::string &f_path,bool f_compressed = false);
    void GenerateVAO();
    void Draw(bool f_texturize,bool f_binding);
    friend class RenderManager;
    friend class Geometry;
};

}
