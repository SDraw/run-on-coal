#pragma once
#include "Elements/Element.h"
#define FONT_FILTER_NONE -1
#define FONT_FILTER_NEAREST 0
#define FONT_FILTER_LINEAR 1
#define FONT_ATLAS_SIZE 512
#define FONT_MAX_TEXT_LENGTH 256

namespace ROC
{

class Font : public Element
{
    FT_Library m_library;
    FT_Face m_face;

    GLuint m_atlasTexture;
    rbp::MaxRectsBinPack *m_atlasPack;

    struct charData
    {
        glm::vec4 m_atlasPosition;
        glm::ivec2 m_size;
        glm::ivec2 m_bearing;
        float m_advance;
    };
    std::map<unsigned int, charData*> m_charMap;
    std::map<unsigned int, charData*>::iterator m_charIter;
    std::map<unsigned int, charData*>::iterator m_charMapEnd;

    std::vector<glm::vec3> *m_vertices;
    GLuint m_vertexVBO;
    std::vector<glm::vec2> *m_uv;
    GLuint m_uvVBO;
    GLuint m_VAO;

    int m_filteringType;

    bool m_loaded;

    bool LoadChar(unsigned int f_char);

    void Clear();
public:
    inline int GetFiltering() const { return m_filteringType; }
protected:
    Font();
    ~Font();
    bool LoadTTF(const std::string &f_path, int f_size, int f_filter);
    inline GLuint GetVAO() const { return m_VAO; }
    void Draw(const sf::String &f_text, const glm::vec2 &f_pos, bool f_bind);
    friend class ElementManager;
    friend class RenderManager;
};

}
