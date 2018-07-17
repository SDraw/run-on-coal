#pragma once

namespace ROC
{

class Texture;
class Material final
{
    int m_verticesCount;
    GLuint m_VBO[5];
    GLuint m_VAO;

    unsigned char m_type;
    glm::vec4 m_params;
    Texture *m_texture;
public:
    enum MaterialPropertyBit : unsigned char
    {
        MPB_Shading = (1U << 0),
        MPB_Depth = (1U << 1),
        MPB_Transparency = (1U << 2),
        MPB_Doubleside = (1U << 3),
        MPB_Filtering = (1U << 4),
        MPB_Compression = (1U << 5)
    };

    inline unsigned char GetType() const { return m_type; }
    inline const glm::vec4& GetParams() const { return m_params; }

    inline bool IsDoubleSided() const { return ((m_type&MPB_Doubleside) != 0U); }
    inline bool IsTransparent() const { return ((m_type&MPB_Transparency) != 0U); }
    inline bool IsShady() const { return ((m_type&MPB_Shading) != 0U); }
    inline bool HasDepth() const { return ((m_type&MPB_Depth) != 0U); }
    inline bool IsCompressed() const { return ((m_type&MPB_Compression) != 0U); }
    inline unsigned char GetFilteringType() const { return ((m_type&MPB_Filtering) >> 4); }
    inline bool HasTexture() const { return (m_texture != nullptr); }
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

    inline Texture* GetTexture() { return m_texture; }

    void Draw();

    friend class RenderManager;
    friend class Geometry;
};

}
