#include "stdafx.h"
#include "Scene/Font.h"

ROC::Font::Font()
{
    m_loaded = false;
    m_library = FT_Library();
    m_face = FT_Face();

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_vertexVBO);
    glGenBuffers(1, &m_uvVBO);

    glBindVertexArray(m_VAO);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 3, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_uvVBO);
    GLfloat l_uvs[6][2] = {
        { 0.0, 0.0 }, { 0.0, 1.0 }, { 1.0, 1.0 },
        { 0.0, 0.0 }, { 1.0, 1.0 }, { 1.0, 0.0 }
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(l_uvs), l_uvs, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    for(auto &iter : m_vertices) iter.z = 0.5f;
}
ROC::Font::~Font()
{
    glDeleteBuffers(1, &m_vertexVBO);
    glDeleteBuffers(1, &m_uvVBO);
    glDeleteVertexArrays(1, &m_VAO);
    if(m_loaded)
    {
        for(auto iter : m_charMap)
        {
            glDeleteTextures(1, &(iter.second->m_texture));
            delete iter.second;
        }
        FT_Done_Face(m_face);
        FT_Done_FreeType(m_library);
    }
}

bool ROC::Font::LoadTTF(std::string &f_path, int f_size)
{
    if(m_loaded) return false;
    if(FT_Init_FreeType(&m_library)) return false;
    if(FT_New_Face(m_library, f_path.c_str(), 0, &m_face))
    {
        FT_Done_FreeType(m_library);
        return false;
    }
    FT_Select_Charmap(m_face, ft_encoding_unicode);
    FT_Set_Pixel_Sizes(m_face, 0, f_size);
    m_loaded = true;
    return true;
}
bool ROC::Font::LoadChar(unsigned int l_char)
{
    if(FT_Load_Char(m_face, l_char, FT_LOAD_RENDER)) return false;
    charData *l_charData = new charData();
    glGenTextures(1, &l_charData->m_texture);
    glBindTexture(GL_TEXTURE_2D, l_charData->m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, m_face->glyph->bitmap.buffer);
    l_charData->m_size = glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows);
    l_charData->m_advance = m_face->glyph->advance.x;
    l_charData->m_breaing = glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top);
    m_charMap.insert(std::pair<unsigned int, charData*>(l_char, l_charData));
    return true;
}

bool ROC::Font::Draw(sf::String &f_text, glm::vec2 &f_pos, bool f_bind)
{
    if(!m_loaded) return false;
    if(f_bind)
    {
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
        glActiveTexture(GL_TEXTURE0);
    }
    glm::vec2 l_pos(f_pos);
    glm::vec2 l_result;
    GLuint l_lastTexture = 0U;
    for(auto iter : f_text)
    {
        if(m_charMap.find(iter) == m_charMap.end())
        {
            if(!LoadChar(iter)) continue;
        }
        charData *l_charData = m_charMap[iter];
        l_result.x = l_pos.x + l_charData->m_breaing.x;
        l_result.y = l_pos.y - (l_charData->m_size.y - l_charData->m_breaing.y);

        m_vertices[0].x = m_vertices[1].x = m_vertices[3].x = l_result.x;
        m_vertices[1].y = m_vertices[2].y = m_vertices[4].y = l_result.y;
        m_vertices[2].x = m_vertices[4].x = m_vertices[5].x = l_result.x + l_charData->m_size.x;
        m_vertices[0].y = m_vertices[3].y = m_vertices[5].y = l_result.y + l_charData->m_size.y;

        if(l_lastTexture != l_charData->m_texture)
        {
            l_lastTexture = l_charData->m_texture;
            glBindTexture(GL_TEXTURE_2D, l_charData->m_texture);
        }
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_vertices), m_vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        l_pos.x += (l_charData->m_advance >> 6);
    }
    return true;
}