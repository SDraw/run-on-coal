#pragma once
#include "Interfaces/IFont.h"
#include "Elements/Element.h"

class GLTexture2D;
class GLArrayBuffer;
class GLVertexArray;

namespace ROC
{

class Font final : public Element, public virtual IFont
{
    static FT_Library ms_library;
    FT_Face m_face;
    float m_size;

    GLTexture2D *m_atlasTexture;
    rbp::MaxRectsBinPack *m_atlasPack;
    glm::vec2 m_atlasOffset;
    glm::ivec2 m_atlasSize;

    struct FontCharacterData
    {
        glm::vec4 m_atlasPosition;
        glm::ivec2 m_size;
        glm::ivec2 m_bearing;
        float m_advance;
    };
    std::unordered_map<unsigned int, FontCharacterData*> m_charMap;
    std::unordered_map<unsigned int, FontCharacterData*>::iterator m_charIter;
    std::unordered_map<unsigned int, FontCharacterData*>::iterator m_charMapEnd;

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
    static GLArrayBuffer *ms_arrayBuffers[FBI_BufferCount];
    static GLVertexArray *ms_vertexArray;

    static std::vector<glm::vec3> ms_vertices;
    static std::vector<glm::vec2> ms_uv;

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

    bool Load(const std::string &f_path, int f_size, const glm::ivec2 &f_atlas, unsigned char f_filter);

    void Draw(const sf::String &f_text, const glm::vec2 &f_pos);

    static void InitStaticResources();
    static void ReleaseStaticResources();

    friend class ElementManager;
    friend class RenderManager;
};

}
