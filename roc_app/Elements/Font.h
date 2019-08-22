#pragma once
#include "Interfaces/IFont.h"
#include "Elements/Element.h"

namespace ROC
{

class Font final : public Element, public virtual IFont
{
    static FT_Library ms_library;
    FT_Face m_face;
    float m_size;

    GLuint m_atlasTexture;
    rbp::MaxRectsBinPack *m_atlasPack;
    glm::vec2 m_atlasOffset;
    glm::ivec2 m_atlasSize;

    struct charData
    {
        glm::vec4 m_atlasPosition;
        glm::ivec2 m_size;
        glm::ivec2 m_bearing;
        float m_advance;
    };
    std::unordered_map<unsigned int, charData*> m_charMap;
    std::unordered_map<unsigned int, charData*>::iterator m_charIter;
    std::unordered_map<unsigned int, charData*>::iterator m_charMapEnd;

    enum FontBufferIndex : size_t
    {
        FBI_Vertex = 0U,
        FBI_UV,

        FBI_BufferCount
    };
    enum FontBufferAttribute : size_t
    {
        FBA_Vertex = 0U,
        FBA_UV = 2U
    };
    static GLuint ms_VBO[FBI_BufferCount];
    static std::vector<glm::vec3> ms_vertices;
    static std::vector<glm::vec2> ms_uv;
    static GLuint ms_VAO;

    unsigned char m_filteringType;

    bool m_loaded;

    Font(const Font &that) = delete;
    Font& operator=(const Font &that) = delete;

    bool LoadChar(unsigned int f_char);
public:
    unsigned char GetFiltering() const;
    float GetGlyphSize() const;
protected:
    Font();
    ~Font();

    static void CreateVAO();
    static void DestroyVAO();
    static void CreateLibrary();
    static void DestroyLibrary();
    bool Load(const std::string &f_path, int f_size, const glm::ivec2 &f_atlas, unsigned char f_filter);

    void Draw(const sf::String &f_text, const glm::vec2 &f_pos);

    friend class ElementManager;
    friend class RenderManager;
};

}
