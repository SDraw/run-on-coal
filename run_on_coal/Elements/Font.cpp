#include "stdafx.h"

#include "Elements/Font.h"

#include "Utils/MathUtils.h"
#include "Utils/GLBinder.h"

namespace ROC
{

const float g_FontDefaultAtlasOffset = 1.f / static_cast<float>(ROC_FONT_ATLAS_SIZE);
const GLint g_FontSwizzleMask[] = { GL_ONE, GL_ONE, GL_ONE, GL_RED };

}

#define ROC_FONT_CHAR_VERTICES 6
#define ROC_FONT_TEXT_BLOCK 512U

FT_Library ROC::Font::ms_library = FT_Library();

std::vector<glm::vec3> ROC::Font::ms_vertices;
GLuint ROC::Font::ms_vertexVBO = 0U;
std::vector<glm::vec2> ROC::Font::ms_uv;
GLuint ROC::Font::ms_uvVBO = 0U;
GLuint ROC::Font::ms_VAO = 0U;

ROC::Font::Font()
{
    m_elementType = ET_Font;
    m_elementTypeName.assign("Font");

    m_loaded = false;
    m_face = FT_Face();
    m_size = 0.f;

    m_atlasTexture = 0U;
    m_atlasOffset = glm::vec2(0.f);
    m_atlasSize = glm::ivec2(0);
    m_atlasPack = nullptr;

    m_charIter = m_charMap.begin();
    m_charMapEnd = m_charMap.end();

    m_filteringType = 0;
}
ROC::Font::~Font()
{
    if(m_loaded)
    {
        if(GLBinder::IsTextureBinded(m_atlasTexture)) GLBinder::ResetTexture();
        glDeleteTextures(1, &m_atlasTexture);
        delete m_atlasPack;
        FT_Done_Face(m_face);
    }
}

void ROC::Font::CreateLibrary()
{
    FT_Init_FreeType(&ms_library);
}
void ROC::Font::DestroyLibrary()
{
    FT_Done_FreeType(ms_library);
}

void ROC::Font::CreateVAO()
{
    glGenVertexArrays(1, &ms_VAO);
    glGenBuffers(1, &ms_vertexVBO);
    glGenBuffers(1, &ms_uvVBO);

    glBindVertexArray(ms_VAO);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, ms_vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * ROC_FONT_CHAR_VERTICES * ROC_FONT_TEXT_BLOCK, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), NULL);
    ms_vertices.assign(ROC_FONT_CHAR_VERTICES * ROC_FONT_TEXT_BLOCK, glm::vec3(0.f, 0.f, 1.f));

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, ms_uvVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * ROC_FONT_CHAR_VERTICES * ROC_FONT_TEXT_BLOCK, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), NULL);
    ms_uv.assign(ROC_FONT_CHAR_VERTICES * ROC_FONT_TEXT_BLOCK, glm::vec2(0.f));

    glBindVertexArray(NULL);
}
void ROC::Font::DestroyVAO()
{
    glDeleteBuffers(1, &ms_vertexVBO);
    glDeleteBuffers(1, &ms_uvVBO);
    glDeleteVertexArrays(1, &ms_VAO);

    ms_vertices.clear();
    ms_uv.clear();
}

bool ROC::Font::Load(const std::string &f_path, int f_size, const glm::ivec2 &f_atlas, int f_filter)
{
    if(!m_loaded)
    {
        if(!FT_New_Face(ms_library, f_path.c_str(), 0, &m_face))
        {
            FT_Select_Charmap(m_face, ft_encoding_unicode);
            FT_Set_Pixel_Sizes(m_face, 0, f_size);
            m_size = static_cast<float>(f_size);
            m_filteringType = f_filter;
            btClamp(m_filteringType, static_cast<int>(FFT_Nearest), static_cast<int>(FFT_Linear));

            if(MathUtils::IsPowerOfTwo(f_atlas.x))
            {
                m_atlasSize.x = f_atlas.x;
                m_atlasOffset.x = 1.f / static_cast<float>(f_atlas.x);
            }
            else
            {
                m_atlasSize.x = ROC_FONT_ATLAS_SIZE;
                m_atlasOffset.x = g_FontDefaultAtlasOffset;
            }
            if(MathUtils::IsPowerOfTwo(f_atlas.y))
            {
                m_atlasSize.y = f_atlas.y;
                m_atlasOffset.y = 1.f / static_cast<float>(f_atlas.y);
            }
            else
            {
                m_atlasSize.y = ROC_FONT_ATLAS_SIZE;
                m_atlasOffset.y = g_FontDefaultAtlasOffset;
            }

            // Generate atlas texture
            GLint l_lastTexture = 0;
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &l_lastTexture);

            glGenTextures(1, &m_atlasTexture);
            glBindTexture(GL_TEXTURE_2D, m_atlasTexture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST + m_filteringType);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST + m_filteringType);
            glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, g_FontSwizzleMask);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_atlasSize.x, m_atlasSize.y, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);

            if(l_lastTexture) glBindTexture(GL_TEXTURE_2D, l_lastTexture);

            // Generate atlas
            m_atlasPack = new rbp::MaxRectsBinPack();
            m_atlasPack->Init(m_atlasSize.x, m_atlasSize.y, false);

            m_loaded = true;
        }
    }
    return m_loaded;
}
bool ROC::Font::LoadChar(unsigned int f_char)
{
    bool l_result = false;
    if(!FT_Load_Char(m_face, f_char, FT_LOAD_RENDER))
    {
        charData *l_charData = new charData();
        l_charData->m_size.x = m_face->glyph->bitmap.width;
        l_charData->m_size.y = m_face->glyph->bitmap.rows;
        l_charData->m_advance = static_cast<float>(m_face->glyph->advance.x >> 6);
        l_charData->m_bearing.x = m_face->glyph->bitmap_left;
        l_charData->m_bearing.y = m_face->glyph->bitmap_top;

        if(l_charData->m_size.x > 0 && l_charData->m_size.y > 0)
        {
            rbp::Rect l_rectangle = m_atlasPack->Insert(l_charData->m_size.x, l_charData->m_size.y, rbp::MaxRectsBinPack::RectBestShortSideFit);
            if(l_rectangle.height > 0)
            {
                l_charData->m_atlasPosition.x = static_cast<float>(l_rectangle.x) * m_atlasOffset.x;
                l_charData->m_atlasPosition.y = static_cast<float>(l_rectangle.y) * m_atlasOffset.y;
                l_charData->m_atlasPosition.z = l_charData->m_atlasPosition.x + static_cast<float>(l_charData->m_size.x) * m_atlasOffset.x;
                l_charData->m_atlasPosition.w = l_charData->m_atlasPosition.y + static_cast<float>(l_charData->m_size.y) * m_atlasOffset.y;
                glTexSubImage2D(GL_TEXTURE_2D, 0, l_rectangle.x, l_rectangle.y, l_rectangle.width, l_rectangle.height, GL_RED, GL_UNSIGNED_BYTE, m_face->glyph->bitmap.buffer);
            }
        }

        m_charIter = m_charMap.insert(std::make_pair(f_char, l_charData)).first;
        m_charMapEnd = m_charMap.end();

        l_result = true;
    }
    return l_result;
}

void ROC::Font::Draw(const sf::String &f_text, const glm::vec2 &f_pos)
{
    if(m_loaded)
    {
        GLBinder::BindVertexArray(ms_VAO);
        GLBinder::BindTexture2D(m_atlasTexture);

        glm::vec4 l_linePos(f_pos, 0.f, 0.f); // [line_x, line_y, char_x, char_y]
        glm::tvec3<size_t> l_textRange(0U); // [range_min, range_max, subpart]
        size_t l_stringLength = f_text.getSize();

        while(l_textRange.x < l_stringLength)
        {
            int l_charCount = 0;
            l_textRange.y = glm::min((l_textRange.z + 1U)*ROC_FONT_TEXT_BLOCK, l_stringLength);

            for(size_t i = l_textRange.x; i < l_textRange.y; i++)
            {
                unsigned int l_character = f_text[i];
                if(l_character == 0xA) // New line
                {
                    l_linePos.x = f_pos.x;
                    l_linePos.y -= m_size;
                    continue;
                }

                m_charIter = m_charMap.find(l_character);
                if(m_charIter == m_charMapEnd)
                {
                    if(!LoadChar(l_character)) continue;
                }

                charData *l_charData = m_charIter->second;
                if(l_charData->m_size.x > 0 && l_charData->m_size.y > 0)
                {
                    int l_charVertexIndex = l_charCount * ROC_FONT_CHAR_VERTICES;

                    l_linePos.z = l_linePos.x + l_charData->m_bearing.x;
                    l_linePos.w = l_linePos.y - (l_charData->m_size.y - l_charData->m_bearing.y);

                    ms_vertices[l_charVertexIndex].x = ms_vertices[l_charVertexIndex + 1].x = ms_vertices[l_charVertexIndex + 3].x = l_linePos.z;
                    ms_vertices[l_charVertexIndex + 1].y = ms_vertices[l_charVertexIndex + 2].y = ms_vertices[l_charVertexIndex + 4].y = l_linePos.w;
                    ms_vertices[l_charVertexIndex + 2].x = ms_vertices[l_charVertexIndex + 4].x = ms_vertices[l_charVertexIndex + 5].x = l_linePos.z + l_charData->m_size.x;
                    ms_vertices[l_charVertexIndex].y = ms_vertices[l_charVertexIndex + 3].y = ms_vertices[l_charVertexIndex + 5].y = l_linePos.w + l_charData->m_size.y;

                    ms_uv[l_charVertexIndex].x = ms_uv[l_charVertexIndex + 1].x = ms_uv[l_charVertexIndex + 3].x = l_charData->m_atlasPosition.x;
                    ms_uv[l_charVertexIndex].y = ms_uv[l_charVertexIndex + 3].y = ms_uv[l_charVertexIndex + 5].y = l_charData->m_atlasPosition.y;
                    ms_uv[l_charVertexIndex + 2].x = ms_uv[l_charVertexIndex + 4].x = ms_uv[l_charVertexIndex + 5].x = l_charData->m_atlasPosition.z;
                    ms_uv[l_charVertexIndex + 1].y = ms_uv[l_charVertexIndex + 2].y = ms_uv[l_charVertexIndex + 4].y = l_charData->m_atlasPosition.w;

                    l_charCount++;
                }
                l_linePos.x += m_charIter->second->m_advance;
            }

            if(l_charCount > 0)
            {
                if(GLBinder::IsArrayBufferBinded(ms_vertexVBO))
                {
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * ROC_FONT_CHAR_VERTICES * l_charCount, ms_vertices.data());
                    GLBinder::BindArrayBuffer(ms_uvVBO);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2) * ROC_FONT_CHAR_VERTICES * l_charCount, ms_uv.data());
                }
                else
                {
                    if(!GLBinder::IsArrayBufferBinded(ms_uvVBO)) GLBinder::BindArrayBuffer(ms_uvVBO);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2) * ROC_FONT_CHAR_VERTICES * l_charCount, ms_uv.data());
                    GLBinder::BindArrayBuffer(ms_vertexVBO);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * ROC_FONT_CHAR_VERTICES * l_charCount, ms_vertices.data());
                }
                glDrawArrays(GL_TRIANGLES, 0, ROC_FONT_CHAR_VERTICES * l_charCount);
            }
            l_textRange.x = ++l_textRange.z * ROC_FONT_TEXT_BLOCK;
        }
    }
}
