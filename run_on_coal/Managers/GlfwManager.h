#pragma once

namespace ROC
{

class Core;
class Cursor;
class GlfwManager
{
    Core *m_core;

    GLFWwindow *m_window;
public:
    static void WindowResizeCallback(GLFWwindow *window, int width, int height);
    static void KeyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void MouseKeyCallback(GLFWwindow *window, int button, int action, int mods);
    static void MouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
    static void CursorPosCallback(GLFWwindow *window, double xpos, double ypos);
    static void CursorEnterCallback(GLFWwindow *window, int entered);
    static void TextInputCallback(GLFWwindow* window, unsigned int codepoint);
    void SetCursor(Cursor *f_cursor);
    void SetCursorMode(int f_mode);
    void GetCursorPosition(glm::dvec2 &f_pos);
    void SetCursorPosition(glm::dvec2 &f_pos);

    void GetWindowPosition(glm::ivec2 &f_pos);
    void GetWindowSize(glm::ivec2 &f_size);
    void CloseWindow();

    void GetFramebufferSize(glm::ivec2 &f_size);

    double GetTime();
protected:
    GlfwManager(Core *f_core);
    ~GlfwManager();
    void SwapBuffers();
    bool DoPulse();
    friend Core;
    friend class RenderManager;
};

}
