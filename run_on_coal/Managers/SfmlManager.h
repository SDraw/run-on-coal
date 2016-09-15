#pragma once
#define CURSOR_LOCK_BIT 1U
#define CURSOR_VISIBILITY_BIT 2U

namespace ROC
{
class Core;
class EventManager;
class LuaArguments;
class SfmlManager
{
    Core *m_core;
    EventManager *m_eventManager;

    sf::Window *m_window;
    bool m_active;
    sf::Event m_event;
    sf::Clock m_clock;
    unsigned char m_cursorMode;

    LuaArguments *m_argument;
    std::string m_input;
public:
    void SetCursorMode(unsigned char f_mode);
    void GetCursorPosition(glm::ivec2 &f_pos);
    void SetCursorPosition(glm::ivec2 &f_pos);

    void GetWindowPosition(glm::ivec2 &f_pos);
    void GetWindowSize(glm::ivec2 &f_size);
    inline void CloseWindow() { m_active = false; }

    float GetTime();

    bool IsKeyPressed(int f_key);
    bool IsMouseKeyPressed(int f_key);
protected:
    SfmlManager(Core *f_core);
    ~SfmlManager();
    inline void SwapBuffers() { m_window->display(); }
    bool DoPulse();
    friend Core;
    friend class RenderManager;
};
}
