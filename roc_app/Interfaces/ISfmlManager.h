#pragma once

namespace ROC
{

class ISfmlManager
{
public:
    virtual void GetWindowPosition(glm::ivec2 &f_pos) const = 0;
    virtual void SetWindowPosition(const glm::ivec2 &f_pos) = 0;
    virtual void GetWindowSize(glm::ivec2 &f_size) const = 0;
    virtual void CloseWindow() = 0;
    virtual void SetVSync(bool f_sync) = 0;
    virtual void SetFramelimit(unsigned int f_fps) = 0;
    virtual unsigned int GetFramelimit() const = 0;
    virtual void SetTitle(const std::string &f_title) = 0;
    virtual bool SetIcon(const std::string &f_path) = 0;
    virtual void RequestFocus() = 0;
    virtual bool GetFocusState() const = 0;
    virtual void SetInputEnabled(bool f_state) = 0;

    virtual void SetCursorMode(bool f_visible, bool f_lock) = 0;
    virtual void GetCursorPosition(glm::ivec2 &f_pos) const = 0;
    virtual void SetCursorPosition(const glm::ivec2 &f_pos) = 0;

    virtual void GetClipboardString(std::string &f_str) const = 0;
    virtual void SetClipboardString(const std::string &f_str) = 0;

    virtual bool IsKeyPressed(int f_key) = 0;
    virtual bool IsMouseKeyPressed(int f_key) = 0;

    virtual bool IsJoypadConnected(unsigned int f_jp) = 0;
    virtual bool GetJoypadButtonState(unsigned int f_jp, unsigned int f_button) = 0;
    virtual unsigned int GetJoypadButtonCount(unsigned int f_jp) = 0;
    virtual bool CheckJoypadAxis(unsigned int f_jp, unsigned int f_axis) = 0;
    virtual float GetJoypadAxisValue(unsigned int f_jp, unsigned int f_axis) = 0;

    virtual float GetTime() const = 0;
};

}