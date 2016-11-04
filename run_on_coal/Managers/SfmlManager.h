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
    float m_time;
    unsigned char m_cursorMode;
    sf::Image m_icon;

    LuaArguments *m_argument;
    std::string m_input;
public:
    void SetCursorMode(unsigned char f_mode);
    void GetCursorPosition(glm::ivec2 &f_pos);
    void SetCursorPosition(glm::ivec2 &f_pos);

    void GetWindowPosition(glm::ivec2 &f_pos);
    void GetWindowSize(glm::ivec2 &f_size);
    inline void CloseWindow() { m_active = false; }

    inline float GetTime() { return m_time; }

    bool IsKeyPressed(int f_key);
    bool IsMouseKeyPressed(int f_key);

    inline void SetVSync(bool f_sync) { m_window->setVerticalSyncEnabled(f_sync); }
    void SetFramelimit(unsigned int f_fps);
    inline void SetTitle(sf::String &f_title) { m_window->setTitle(f_title); }
    bool SetIcon(std::string &f_path);
    inline void RequestFocus() { m_window->requestFocus(); }
    inline bool GetFocusState() { return m_window->hasFocus(); }
protected:
    SfmlManager(Core *f_core);
    ~SfmlManager();
    inline void SwapBuffers() { m_window->display(); }
    bool DoPulse();
    friend Core;
    friend class RenderManager;
};
}
