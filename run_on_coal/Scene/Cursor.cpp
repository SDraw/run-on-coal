#include "stdafx.h"
#include "Scene/Cursor.h"
#include "Utils/Utils.h"

ROC::Cursor::Cursor()
{
    m_cursor = NULL;
}
ROC::Cursor::~Cursor()
{
    if(m_cursor) glfwDestroyCursor(m_cursor);
}

bool ROC::Cursor::Load(std::string &f_path, bool f_sys)
{
    if(m_cursor) return false;
    if(!f_sys)
    {
        unsigned l_width, l_height;
        std::vector<unsigned char> l_texture;
        if(lodepng::decode(l_texture,l_width,l_height,f_path,LCT_RGBA)) return false;

        GLFWimage l_image;
        l_image.height = l_height;
        l_image.width = l_width;
        l_image.pixels = l_texture.data();
        m_cursor = glfwCreateCursor(&l_image,0,0);
        if(!m_cursor) return false;
    }
    else
    {
        int l_type = Utils::ReadEnumString(f_path,"arrow,ibeam,crosshair,hand,hresize,vresize");
        if(l_type == -1) return false;
        m_cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR+l_type);
        if(!m_cursor) return false;
    }
    return true;
}

GLFWcursor* ROC::Cursor::GetCursor()
{
    return m_cursor;
}
