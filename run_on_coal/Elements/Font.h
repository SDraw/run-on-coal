#pragma once
#include "Elements/Element.h"
#define ROC_FONT_FILTER_NONE -1
#define ROC_FONT_FILTER_NEAREST 0
#define ROC_FONT_FILTER_LINEAR 1
#define ROC_FONT_ATLAS_SIZE 256
#define ROC_FONT_TEXT_LENGTH 256

namespace ROC
{

class Font final : public Element
{
    static FT_Library s_library;
    FT_Face m_face;

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

    std::vector<glm::vec3> m_vertices;
    GLuint m_vertexVBO;
    std::vector<glm::vec2> m_uv;
    GLuint m_uvVBO;
    GLuint m_VAO;
    bool m_switch;

    int m_filteringType;

    bool m_loaded;

    bool LoadChar(unsigned int f_char);

    void Clear();
public:
    inline int GetFiltering() const { return m_filteringType; }
protected:
    Font();
    ~Font();
    static void InitLibrary();
    static void TerminateLibrary();
    bool Load(const std::string &f_path, int f_size, const glm::ivec2 &f_atlas, int f_filter);

    inline GLuint GetVAO() const { return m_VAO; }

    void Draw(const sf::String &f_text, const glm::vec2 &f_pos, bool f_bind);

    friend class ElementManager;
    friend class RenderManager;
};

}
