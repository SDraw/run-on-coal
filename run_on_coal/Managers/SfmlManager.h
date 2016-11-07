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
    sf::Image m_icon;

    sf::Clock m_clock;
    float m_time;

    unsigned char m_cursorMode;

    sf::Event m_event;
    LuaArguments *m_argument;
    std::string m_input;

    SfmlManager(const SfmlManager& that);
public:
    void GetWindowPosition(glm::ivec2 &f_pos);
    void GetWindowSize(glm::ivec2 &f_size);
    inline void CloseWindow() { m_active = false; }
    inline void SetVSync(bool f_sync) { m_window->setVerticalSyncEnabled(f_sync); }
    void SetFramelimit(unsigned int f_fps);
    inline void SetTitle(sf::String &f_title) { m_window->setTitle(f_title); }
    bool SetIcon(std::string &f_path);
    inline void RequestFocus() { m_window->requestFocus(); }
    inline bool GetFocusState() { return m_window->hasFocus(); }

    void SetCursorMode(unsigned char f_mode);
    void GetCursorPosition(glm::ivec2 &f_pos);
    void SetCursorPosition(glm::ivec2 &f_pos);

    bool IsKeyPressed(int f_key);
    bool IsMouseKeyPressed(int f_key);

    inline bool IsJoypadConnected(unsigned int f_jp) { return sf::Joystick::isConnected(f_jp); }
    inline bool GetJoypadButtonState(unsigned int f_jp, unsigned int f_button) { return sf::Joystick::isButtonPressed(f_jp, f_button); }
    inline unsigned int GetJoypadButtonCount(unsigned int f_jp) { return sf::Joystick::getButtonCount(f_jp); }
    inline bool CheckJoypadAxis(unsigned int f_jp, unsigned int f_axis) { return sf::Joystick::hasAxis(f_jp, static_cast<sf::Joystick::Axis>(f_axis)); }
    inline float GetJoypadAxisValue(unsigned int f_jp, unsigned int f_axis) { return sf::Joystick::getAxisPosition(f_jp, static_cast<sf::Joystick::Axis>(f_axis)); }

    inline float GetTime() { return m_time; }
protected:
    explicit SfmlManager(Core *f_core);
    ~SfmlManager();

    bool DoPulse();
    inline void SwapBuffers() { m_window->display(); }
    friend Core;
    friend class RenderManager;
};
}
