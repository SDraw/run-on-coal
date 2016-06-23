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
        FT_Pos m_advance;
    };
    std::map<wchar_t,charData*> m_charMap;

    glm::vec3 m_vertices[6];
    GLuint m_vertexVBO;
    GLuint m_uvVBO;
    GLuint m_VAO;

    bool m_loaded;
    void Clear();
    bool LoadChar(wchar_t l_char);
protected:
    Font();
    ~Font();
    bool LoadTTF(std::string &f_path, int f_size);
    bool Draw(std::wstring &f_text,glm::vec2 &f_pos, bool f_bind);
    GLuint GetVAO();
    friend class ElementManager;
    friend class RenderManager;
};

}
