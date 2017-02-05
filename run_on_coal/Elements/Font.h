#pragma once

namespace ROC
{

class Font
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
    int GetTextWidth(sf::String &f_text);
    int GetTextHeight(sf::String &f_text);
protected:
    Font();
    ~Font();
    bool LoadTTF(std::string &f_path, int f_size, unsigned char f_filter);
    inline GLuint GetVAO() { return m_VAO; }
    void Draw(sf::String &f_text, glm::vec2 &f_pos, bool f_bind);
    friend class ElementManager;
    friend class RenderManager;
};

}
