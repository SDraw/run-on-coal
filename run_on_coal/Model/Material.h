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
    GLuint m_VAO;
    bool m_bVAO;

    Texture *m_texture;

    glm::vec4 m_params;
    unsigned char m_type;
protected:
    Material();
    ~Material();
    void SetType(unsigned char f_type);
    unsigned char GetType();
    bool IsDoubleSided();
    bool IsTransparent();
    bool IsShady();
    bool IsDepthable();
    void GetParams(glm::vec4 &f_params);
    void SetParams(glm::vec4 &f_params);
    void LoadVertices(std::vector<glm::vec3> &f_vector);
    void LoadUVs(std::vector<glm::vec2> &f_vector);
    void LoadNormals(std::vector<glm::vec3> &f_vector);
    void LoadWeights(std::vector<glm::vec4> &f_vector);
    void LoadIndices(std::vector<glm::ivec4> &f_vector);
    void LoadTexture(std::string &f_path, bool f_compressed = false);
    void GenerateVAO();
    void Draw(bool f_texturize, bool f_binding);
    GLuint GetVAO();
    GLuint GetTexture();
    friend class Geometry;
};

}
