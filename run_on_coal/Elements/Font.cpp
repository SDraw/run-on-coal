#include "stdafx.h"
#include "Elements/Font.h"

ROC::Font::Font()
{
    m_elementType = ElementType::FontElement;

    m_loaded = false;
    m_library = FT_Library();
    m_face = FT_Face();

    m_charIter = m_charMap.begin();
    m_charMapEnd = m_charMap.end();

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

    for(auto &iter : m_vertices) iter.z = 1.0f;
    m_filteringType = 0;
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

bool ROC::Font::LoadTTF(std::string &f_path, int f_size, int f_filter)
{
    if(!m_loaded)
    {
        if(!FT_Init_FreeType(&m_library))
        {
            if(!FT_New_Face(m_library, f_path.c_str(), 0, &m_face))
            {
                FT_Select_Charmap(m_face, ft_encoding_unicode);
                FT_Set_Pixel_Sizes(m_face, 0, f_size);
                m_filteringType = f_filter;
                btClamp(m_filteringType, FONT_FILTER_NEAREST, FONT_FILTER_LINEAR);
                m_loaded = true;
            }
            else FT_Done_FreeType(m_library);
        }
    }
    return m_loaded;
}
bool ROC::Font::LoadChar(unsigned int l_char)
{
    bool l_result = !FT_Load_Char(m_face, l_char, FT_LOAD_RENDER);
    if(l_result)
    {
        charData *l_charData = new charData();
        glGenTextures(1, &l_charData->m_texture);
        glBindTexture(GL_TEXTURE_2D, l_charData->m_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST + m_filteringType);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST + m_filteringType);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, m_face->glyph->bitmap.buffer);
        l_charData->m_size.x = m_face->glyph->bitmap.width;
        l_charData->m_size.y = m_face->glyph->bitmap.rows;
        l_charData->m_advance = static_cast<float>(m_face->glyph->advance.x >> 6);
        l_charData->m_breaing.x = m_face->glyph->bitmap_left;
        l_charData->m_breaing.y = m_face->glyph->bitmap_top;
        m_charIter = m_charMap.insert(std::pair<unsigned int, charData*>(l_char, l_charData)).first;
        m_charMapEnd = m_charMap.end();
    }
    return l_result;
}

int ROC::Font::GetTextWidth(sf::String &f_text)
{
    float l_width = 0.f;
    for(auto iter : f_text)
    {
        m_charIter = m_charMap.find(iter);
        if(m_charIter == m_charMapEnd)
        {
            if(!LoadChar(iter)) continue;
        }
        l_width += m_charIter->second->m_advance;
    }
    return static_cast<int>(l_width);
}
int ROC::Font::GetTextHeight(sf::String &f_text)
{
    int l_height = 0;
    for(auto iter : f_text)
    {
        m_charIter = m_charMap.find(iter);
        if(m_charIter == m_charMapEnd)
        {
            if(!LoadChar(iter)) continue;
        }
        int l_charHeight = m_charIter->second->m_breaing.y + abs(m_charIter->second->m_breaing.y - m_charIter->second->m_size.y);
        if(l_charHeight > l_height) l_height = l_charHeight;
    }
    return l_height;
}

void ROC::Font::Draw(sf::String &f_text, glm::vec2 &f_pos, bool f_bind)
{
    if(m_loaded)
    {
        if(f_bind)
        {
            glBindVertexArray(m_VAO);
            glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
            glActiveTexture(GL_TEXTURE0);
        }
        float l_displacement = f_pos.x;
        glm::vec2 l_result;
        GLuint l_lastTexture = 0U;
        for(auto iter : f_text)
        {
            m_charIter = m_charMap.find(iter);
            if(m_charIter == m_charMapEnd)
            {
                if(!LoadChar(iter)) continue;
            }
            l_result.x = l_displacement + m_charIter->second->m_breaing.x;
            l_result.y = f_pos.y - (m_charIter->second->m_size.y - m_charIter->second->m_breaing.y);

            m_vertices[0].x = m_vertices[1].x = m_vertices[3].x = l_result.x;
            m_vertices[1].y = m_vertices[2].y = m_vertices[4].y = l_result.y;
            m_vertices[2].x = m_vertices[4].x = m_vertices[5].x = l_result.x + m_charIter->second->m_size.x;
            m_vertices[0].y = m_vertices[3].y = m_vertices[5].y = l_result.y + m_charIter->second->m_size.y;

            if(l_lastTexture != m_charIter->second->m_texture)
            {
                l_lastTexture = m_charIter->second->m_texture;
                glBindTexture(GL_TEXTURE_2D, m_charIter->second->m_texture);
            }
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_vertices), m_vertices);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            l_displacement += m_charIter->second->m_advance;
        }
    }
}
