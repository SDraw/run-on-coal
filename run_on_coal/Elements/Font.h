#pragma once
#include "Elements/Element.h"

#define ROC_FONT_ATLAS_SIZE 256

namespace ROC
{

class Font final : public Element
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

    int m_filteringType;

    bool m_loaded;

    Font(const Font &that) = delete;
    Font& operator=(const Font &that) = delete;

    bool LoadChar(unsigned int f_char);
public:
    enum FontFilteringType
    {
        FFT_None = -1,
        FFT_Nearest,
        FFT_Linear
    };

    inline int GetFiltering() const { return m_filteringType; }
protected:
    Font();
    ~Font();

    static void CreateVAO();
    static void DestroyVAO();
    static void CreateLibrary();
    static void DestroyLibrary();
    bool Load(const std::string &f_path, int f_size, const glm::ivec2 &f_atlas, int f_filter = FFT_Nearest);

    void Draw(const sf::String &f_text, const glm::vec2 &f_pos);

    friend class ElementManager;
    friend class RenderManager;
};

}
