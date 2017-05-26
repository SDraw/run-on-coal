#pragma once
#define CURSOR_LOCK_BIT 1U
#define CURSOR_VISIBILITY_BIT 2U

namespace ROC
{

class Core;
class LuaArguments;
class SfmlManager
{
    Core *m_core;

    sf::Window *m_window;
    bool m_active;

    sf::Clock m_clock;
    float m_time;

    unsigned int m_frameLimit;

    sf::Event m_event;
    LuaArguments *m_argument;

    unsigned char m_cursorMode;

    SfmlManager(const SfmlManager& that);
    SfmlManager &operator =(const SfmlManager &that);
public:
    void GetWindowPosition(glm::ivec2 &f_pos);
    void GetWindowSize(glm::ivec2 &f_size);
    inline void CloseWindow() { m_active = false; }
    inline void SetVSync(bool f_sync) { m_window->setVerticalSyncEnabled(f_sync); }
    void SetFramelimit(unsigned int f_fps);
    inline unsigned int GetFramelimit() const { return m_frameLimit; }
    inline void SetTitle(const sf::String &f_title) { m_window->setTitle(f_title); }
    bool SetIcon(const std::string &f_path);
    inline void RequestFocus() { m_window->requestFocus(); }
    inline bool GetFocusState() const { return m_window->hasFocus(); }

    void SetCursorMode(unsigned char f_mode);
    void GetCursorPosition(glm::ivec2 &f_pos);
    void SetCursorPosition(const glm::ivec2 &f_pos);

    static bool IsKeyPressed(int f_key);
    static bool IsMouseKeyPressed(int f_key);

    static bool IsJoypadConnected(unsigned int f_jp);
    static bool GetJoypadButtonState(unsigned int f_jp, unsigned int f_button);
    static unsigned int GetJoypadButtonCount(unsigned int f_jp);
    static bool CheckJoypadAxis(unsigned int f_jp, unsigned int f_axis);
    static float GetJoypadAxisValue(unsigned int f_jp, unsigned int f_axis);

    inline float GetTime() const { return m_time; }
protected:
    explicit SfmlManager(Core *f_core);
    ~SfmlManager();

    inline void SwapBuffers() { m_window->display(); }

    bool DoPulse();

    friend Core;
    friend class RenderManager;
};

}
