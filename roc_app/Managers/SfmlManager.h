#pragma once
#include "Interfaces/ISfmlManager.h"

namespace ROC
{

class Core;
class CustomArguments;

class SfmlManager final : public ISfmlManager
{
    Core *m_core;

    sf::VideoMode m_windowVideoMode;
    sf::ContextSettings m_contextSettings;
    sf::Uint32 m_windowStyle;
    unsigned int m_frameLimit;
    sf::Window *m_window;
    sf::Event m_event;
    bool m_active;

    sf::Clock m_clock;
    float m_time;

    bool m_inputEnabled;

    CustomArguments *m_arguments;

    SfmlManager(const SfmlManager &that) = delete;
    SfmlManager& operator=(const SfmlManager &that) = delete;
public:
    void GetWindowPosition(glm::ivec2 &f_pos) const;
    void SetWindowPosition(const glm::ivec2 &f_pos);
    void GetWindowSize(glm::ivec2 &f_size) const;
    void CloseWindow();
    void SetVSync(bool f_sync);
    void SetFramelimit(unsigned int f_fps);
    unsigned int GetFramelimit() const;
    void SetTitle(const std::string &f_title);
    bool SetIcon(const std::string &f_path);
    void RequestFocus();
    bool GetFocusState() const;
    void SetInputEnabled(bool f_state);

    void SetCursorMode(bool f_visible, bool f_lock);
    void GetCursorPosition(glm::ivec2 &f_pos) const;
    void SetCursorPosition(const glm::ivec2 &f_pos);

    void GetClipboardString(std::string &f_str) const;
    void SetClipboardString(const std::string &f_str);

    bool IsKeyPressed(int f_key);
    bool IsMouseKeyPressed(int f_key);

    bool IsJoypadConnected(unsigned int f_jp);
    bool GetJoypadButtonState(unsigned int f_jp, unsigned int f_button);
    unsigned int GetJoypadButtonCount(unsigned int f_jp);
    bool CheckJoypadAxis(unsigned int f_jp, unsigned int f_axis);
    float GetJoypadAxisValue(unsigned int f_jp, unsigned int f_axis);

    float GetTime() const;
protected:
    explicit SfmlManager(Core *f_core);
    ~SfmlManager();

    void UpdateWindow();

    bool DoPulse();

    friend class Core;
    friend class RenderManager;
};

}
