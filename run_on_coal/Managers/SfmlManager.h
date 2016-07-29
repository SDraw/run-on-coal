#pragma once

namespace ROC
{

class Core;
class LuaArguments;
class SfmlManager
{
    Core *m_core;

    sf::Window *m_window;
    sf::Event m_event;
    sf::Clock m_clock;

    static LuaArguments m_argument;
    std::string m_input;

    bool m_active;

    bool m_cursorDisabled;
    sf::Vector2i m_cursorCenter;
public:
    void SetCursorMode(int f_mode);
    void GetCursorPosition(glm::dvec2 &f_pos);
    void SetCursorPosition(glm::dvec2 &f_pos);

    void GetWindowPosition(glm::ivec2 &f_pos);
    void GetWindowSize(glm::ivec2 &f_size);
    void CloseWindow();

    float GetTime();
protected:
    SfmlManager(Core *f_core);
    ~SfmlManager();
    inline void SwapBuffers() { m_window->display(); }
    bool DoPulse();
    friend Core;
    friend class RenderManager;
};

}
