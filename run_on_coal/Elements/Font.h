#pragma once
#include "Elements/Element.h"
#define FONT_FILTER_NONE -1
#define FONT_FILTER_NEAREST 0
#define FONT_FILTER_LINEAR 1

namespace ROC
{

class Font : public Element
{
    FT_Library m_library;
    FT_Face m_face;

    struct charData
    {
        GLuint m_texture;
        glm::ivec2 m_size;
        glm::ivec2 m_breaing;
        float m_advance;
    };
    std::unordered_map<unsigned int, charData*> m_charMap;
    std::unordered_map<unsigned int, charData*>::iterator m_charIter;
    std::unordered_map<unsigned int, charData*>::iterator m_charMapEnd;

    glm::vec3 m_vertices[6];
    GLuint m_vertexVBO;
    GLuint m_uvVBO;
    GLuint m_VAO;
    int m_filteringType;

    bool m_loaded;

    bool LoadChar(unsigned int l_char);

    void Clear();
public:
    inline int GetFiltering() const { return m_filteringType; }
    int GetTextWidth(sf::String &f_text);
    int GetTextHeight(sf::String &f_text);
protected:
    Font();
    ~Font();
    bool LoadTTF(std::string &f_path, int f_size, int f_filter);
    inline GLuint GetVAO() const { return m_VAO; }
    void Draw(sf::String &f_text, glm::vec2 &f_pos, bool f_bind);
    friend class ElementManager;
    friend class RenderManager;
};

}
