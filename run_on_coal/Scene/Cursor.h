#pragma once

namespace ROC
{

class Cursor
{
    GLFWcursor *m_cursor;
protected:
    Cursor();
    ~Cursor();
    GLFWcursor* GetCursor();
    bool Load(std::string &f_path, bool f_sys);
    friend class ElementManager;
    friend class GlfwManager;
};

}
