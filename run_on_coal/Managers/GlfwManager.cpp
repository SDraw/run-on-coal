#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ConfigManager.h"
#include "Managers/EventManager.h"
#include "Managers/GlfwManager.h"
#include "Managers/LogManager.h"
#include "Managers/LuaManager.h"
#include "Scene/Cursor.h"
#include "Lua/LuaArguments.h"
#include "Utils/Utils.h"

void ROC::GlfwManager::WindowResizeCallback(GLFWwindow *window, int width, int height)
{
    GlfwManager *l_glfwManager = (GlfwManager*)glfwGetWindowUserPointer(window);
    if(!l_glfwManager) return;
    EventManager *l_eventManager = l_glfwManager->m_core->GetLuaManager()->GetEventManager();
    if(!l_eventManager->IsEventExists(EventType::WindowResize)) return;
    ROC::LuaArguments l_args;
    l_args.PushArgument(width);
    l_args.PushArgument(height);
    l_eventManager->CallEvent(EventType::WindowResize,l_args);
}
void ROC::GlfwManager::KeyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    GlfwManager *l_glfwManager = (GlfwManager*)glfwGetWindowUserPointer(window);
    if(!l_glfwManager) return;
    EventManager *l_eventManager = l_glfwManager->m_core->GetLuaManager()->GetEventManager();
    if(!l_eventManager->IsEventExists(EventType::KeyPress)) return;
    ROC::LuaArguments l_args;
    l_args.PushArgument(key);
    l_args.PushArgument(scancode);
    l_args.PushArgument(action);
    l_args.PushArgument(mods);
    l_eventManager->CallEvent(EventType::KeyPress,l_args);
}
void ROC::GlfwManager::MouseKeyCallback(GLFWwindow *window, int button, int action, int mods)
{
    GlfwManager *l_glfwManager = (GlfwManager*)glfwGetWindowUserPointer(window);
    if(!l_glfwManager) return;
    EventManager *l_eventManager = l_glfwManager->m_core->GetLuaManager()->GetEventManager();
    if(!l_eventManager->IsEventExists(EventType::MouseKeyPress)) return;
    ROC::LuaArguments l_args;
    l_args.PushArgument(button);
    l_args.PushArgument(action);
    l_args.PushArgument(mods);
    l_eventManager->CallEvent(EventType::MouseKeyPress,l_args);
}
void ROC::GlfwManager::MouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    GlfwManager *l_glfwManager = (GlfwManager*)glfwGetWindowUserPointer(window);
    if(!l_glfwManager) return;
    EventManager *l_eventManager = l_glfwManager->m_core->GetLuaManager()->GetEventManager();
    if(!l_eventManager->IsEventExists(EventType::MouseScroll)) return;
    ROC::LuaArguments l_args;
    l_args.PushArgument(xoffset);
    l_args.PushArgument(yoffset);
    l_eventManager->CallEvent(EventType::MouseScroll,l_args);
}
void ROC::GlfwManager::CursorPosCallback(GLFWwindow *window, double xpos, double ypos)
{
    GlfwManager *l_glfwManager = (GlfwManager*)glfwGetWindowUserPointer(window);
    if(!l_glfwManager) return;
    EventManager *l_eventManager = l_glfwManager->m_core->GetLuaManager()->GetEventManager();
    if(!l_eventManager->IsEventExists(EventType::CursorMove)) return;
    ROC::LuaArguments l_args;
    l_args.PushArgument(xpos);
    l_args.PushArgument(ypos);
    l_eventManager->CallEvent(EventType::CursorMove,l_args);
}
void ROC::GlfwManager::CursorEnterCallback(GLFWwindow *window, int entered)
{
    GlfwManager *l_glfwManager = (GlfwManager*)glfwGetWindowUserPointer(window);
    if(!l_glfwManager) return;
    EventManager *l_eventManager = l_glfwManager->m_core->GetLuaManager()->GetEventManager();
    if(!l_eventManager->IsEventExists(EventType::CursorEnter)) return;
    ROC::LuaArguments l_args;
    l_args.PushArgument(entered);
    l_eventManager->CallEvent(EventType::CursorEnter,l_args);
}
void ROC::GlfwManager::TextInputCallback(GLFWwindow* window, unsigned int codepoint)
{
    GlfwManager *l_glfwManager = (GlfwManager*)glfwGetWindowUserPointer(window);
    if(!l_glfwManager) return;
    EventManager *l_eventManager = l_glfwManager->m_core->GetLuaManager()->GetEventManager();
    if(!l_eventManager->IsEventExists(EventType::TextInput)) return;
    ROC::LuaArguments l_args;
    std::string l_text;
    utf8::utf32to8(&codepoint,&codepoint+1,std::back_inserter(l_text));
    l_args.PushArgument(l_text);
    l_eventManager->CallEvent(EventType::TextInput,l_args);
}

ROC::GlfwManager::GlfwManager(Core *f_core)
{
    m_core = f_core;

    std::string l_log;
    if(!glfwInit())
    {
        l_log.append("Unable to start GLFW3. Exit.");
        f_core->GetLogManager()->Log(l_log);
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_DEPTH_BITS,32);

    glm::ivec2 l_windowSize;
    f_core->GetConfigManager()->GetWindowSize(l_windowSize);

    glfwWindowHint(GLFW_SAMPLES,f_core->GetConfigManager()->GetAntialiasing());
    if(f_core->GetConfigManager()->IsFullscreenEnabled())
    {
        GLFWmonitor *primary = glfwGetPrimaryMonitor();
        m_window = glfwCreateWindow(l_windowSize.x,l_windowSize.y,"ROC",primary,NULL);
    }
    else m_window = glfwCreateWindow(l_windowSize.x,l_windowSize.y,"ROC",NULL,NULL);
    if(!m_window)
    {
        l_log.append("Unable to open window with GLFW3. Exit.");
        f_core->GetLogManager()->Log(l_log);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetWindowUserPointer(m_window,this);

    glfwSetWindowSizeCallback(m_window,GlfwManager::WindowResizeCallback);
    glfwSetKeyCallback(m_window,GlfwManager::KeyboardCallback);
    glfwSetMouseButtonCallback(m_window,GlfwManager::MouseKeyCallback);
    glfwSetScrollCallback(m_window,GlfwManager::MouseScrollCallback);
    glfwSetCursorPosCallback(m_window,GlfwManager::CursorPosCallback);
    glfwSetCursorEnterCallback(m_window,GlfwManager::CursorEnterCallback);
    glfwSetCharCallback(m_window,GlfwManager::TextInputCallback);

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);

    glewExperimental = true;
    GLenum l_error = glewInit();
    if (l_error != GLEW_OK)
    {
        l_log.append((char*)glewGetErrorString(l_error));
        f_core->GetLogManager()->Log(l_log);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    l_log.append("OpenGL ");
    l_log.append((char*)glGetString(GL_VERSION));
    f_core->GetLogManager()->Log(l_log);
    l_log.clear();
    l_log.append("Renderer ");
    l_log.append((char*)glGetString(GL_RENDERER));
    f_core->GetLogManager()->Log(l_log);
    l_log.clear();
    l_log.append("GLEW ");
    l_log.append((char*)glewGetString(GLEW_VERSION));
    f_core->GetLogManager()->Log(l_log);
}

ROC::GlfwManager::~GlfwManager()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

bool ROC::GlfwManager::DoPulse()
{
    glfwPollEvents();

    if(glfwJoystickPresent(0)) //There is connected joypad!
    {
        EventManager *l_eventManager = m_core->GetLuaManager()->GetEventManager();
        if(l_eventManager->IsEventExists(EventType::Joypad))
        {
            LuaArguments l_args;
            std::string l_name;
            int l_count = 0;
            for(int i=GLFW_JOYSTICK_1; i < GLFW_JOYSTICK_LAST; i++)
            {
                if(!glfwJoystickPresent(i)) break;
                const unsigned char *l_padButtons = glfwGetJoystickButtons(i,&l_count);
                std::vector<unsigned char> l_buttons{l_padButtons,l_padButtons+l_count};

                const float *l_padAxes = glfwGetJoystickAxes(i,&l_count);
                std::vector<float> l_axes{l_padAxes,l_padAxes+l_count};

                l_args.PushArgument(i);
                l_name.append(glfwGetJoystickName(i));
                l_args.PushArgument(l_name);
                l_args.PushArgument(l_buttons);
                l_args.PushArgument(l_axes);
                l_eventManager->CallEvent(EventType::Joypad,l_args);
                l_args.Clear();
                l_name.clear();
            }
        }
    }

    return !glfwWindowShouldClose(m_window);
}

void ROC::GlfwManager::SwapBuffers()
{
    glfwSwapBuffers(m_window);
}

void ROC::GlfwManager::SetCursor(Cursor *f_cursor)
{
    if(!f_cursor) glfwSetCursor(m_window,NULL);
    else glfwSetCursor(m_window,f_cursor->GetCursor());
}
void ROC::GlfwManager::SetCursorMode(int f_mode)
{
    glfwSetInputMode(m_window,GLFW_CURSOR,f_mode);
}
void ROC::GlfwManager::GetCursorPosition(glm::dvec2 &f_pos)
{
    glfwGetCursorPos(m_window,&f_pos.x,&f_pos.y);
}
void ROC::GlfwManager::SetCursorPosition(glm::dvec2 &f_pos)
{
    glfwSetCursorPos(m_window,f_pos.x,f_pos.y);
}

void ROC::GlfwManager::GetWindowPosition(glm::ivec2 &f_pos)
{
    glfwGetWindowPos(m_window,&f_pos.x,&f_pos.y);
}
void ROC::GlfwManager::GetWindowSize(glm::ivec2 &f_size)
{
    glfwGetWindowSize(m_window,&f_size.x,&f_size.y);
}
void ROC::GlfwManager::CloseWindow()
{
    glfwSetWindowShouldClose(m_window,true);
}

void ROC::GlfwManager::GetFramebufferSize(glm::ivec2 &f_size)
{
    glfwGetFramebufferSize(m_window,&f_size.x,&f_size.y);
}

double ROC::GlfwManager::GetTime()
{
    return glfwGetTime();
}
