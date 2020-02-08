#pragma once

class Model final
{
    enum ModelType : unsigned char
    {
        MT_Static = 0x1U,
        MT_Animated = 0x2U
    };
    ModelType m_type;
    bool m_loaded;

    std::vector<glm::vec3> m_vertexData;
    std::vector<glm::vec2> m_uvData;
    std::vector<glm::vec3> m_normalData;
    std::vector<glm::vec4> m_weightData;
    std::vector<glm::ivec4> m_indexData;

    enum MaterialTypeBit : unsigned char
    {
        MTB_Shading = 0U,
        MTB_Depth,
        MTB_Transparency,
        MTB_Doubleside,
        MTB_Filtering,
        MTB_Compression
    };
    struct Face
    {
        int m_materialIndex[9U];
    };
    struct Material
    {
        std::bitset<8U> m_type;
        glm::vec4 m_params;
        std::string m_diffuseTexture;
        std::vector<Face> m_faces;
    };
    std::vector<Material> m_materials;

    struct Bone
    {
        std::string m_name;
        int m_parent;
        glm::vec3 m_position;
        glm::quat m_rotation;
        glm::vec3 m_scale;
    };
    std::vector<Bone> m_bones;

    void Clean();
public:
    Model();
    ~Model();

    bool LoadJSON(const std::string &f_path);
    bool LoadOBJ(const std::string &f_path);

    bool Generate(const std::string &f_path);
};

