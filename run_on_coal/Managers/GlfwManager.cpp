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

ROC::LuaArguments ROC::GlfwManager::m_argument = ROC::LuaArguments();;

void ROC::GlfwManager::WindowResizeCallback(GLFWwindow *window, int width, int height)
{
    EventManager *l_eventManager = static_cast<GlfwManager*>(glfwGetWindowUserPointer(window))->m_core->GetLuaManager()->GetEventManager();
    if(!l_eventManager->IsEventExists(EventType::WindowResize)) return;
    m_argument.PushArgument(width);
    m_argument.PushArgument(height);
    l_eventManager->CallEvent(EventType::WindowResize,m_argument);
    m_argument.Clear();
}
void ROC::GlfwManager::KeyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    EventManager *l_eventManager = static_cast<GlfwManager*>(glfwGetWindowUserPointer(window))->m_core->GetLuaManager()->GetEventManager();
    if(!l_eventManager->IsEventExists(EventType::KeyPress)) return;
    m_argument.PushArgument(key);
    m_argument.PushArgument(scancode);
    m_argument.PushArgument(action);
    m_argument.PushArgument(mods);
    l_eventManager->CallEvent(EventType::KeyPress,m_argument);
    m_argument.Clear();
}
void ROC::GlfwManager::MouseKeyCallback(GLFWwindow *window, int button, int action, int mods)
{
    EventManager *l_eventManager = static_cast<GlfwManager*>(glfwGetWindowUserPointer(window))->m_core->GetLuaManager()->GetEventManager();
    if(!l_eventManager->IsEventExists(EventType::MouseKeyPress)) return;
    m_argument.PushArgument(button);
    m_argument.PushArgument(action);
    m_argument.PushArgument(mods);
    l_eventManager->CallEvent(EventType::MouseKeyPress,m_argument);
    m_argument.Clear();
}
void ROC::GlfwManager::MouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    EventManager *l_eventManager = static_cast<GlfwManager*>(glfwGetWindowUserPointer(window))->m_core->GetLuaManager()->GetEventManager();
    if(!l_eventManager->IsEventExists(EventType::MouseScroll)) return;
    m_argument.PushArgument(xoffset);
    m_argument.PushArgument(yoffset);
    l_eventManager->CallEvent(EventType::MouseScroll,m_argument);
    m_argument.Clear();
}
void ROC::GlfwManager::CursorPosCallback(GLFWwindow *window, double xpos, double ypos)
{
    EventManager *l_eventManager = static_cast<GlfwManager*>(glfwGetWindowUserPointer(window))->m_core->GetLuaManager()->GetEventManager();
    if(!l_eventManager->IsEventExists(EventType::CursorMove)) return;
    m_argument.PushArgument(xpos);
    m_argument.PushArgument(ypos);
    l_eventManager->CallEvent(EventType::CursorMove,m_argument);
    m_argument.Clear();
}
void ROC::GlfwManager::CursorEnterCallback(GLFWwindow *window, int entered)
{
    EventManager *l_eventManager = static_cast<GlfwManager*>(glfwGetWindowUserPointer(window))->m_core->GetLuaManager()->GetEventManager();
    if(!l_eventManager->IsEventExists(EventType::CursorEnter)) return;
    m_argument.PushArgument(entered);
    l_eventManager->CallEvent(EventType::CursorEnter,m_argument);
    m_argument.Clear();
}
void ROC::GlfwManager::TextInputCallback(GLFWwindow* window, unsigned int codepoint)
{
    GlfwManager *l_glfwManager = static_cast<GlfwManager*>(glfwGetWindowUserPointer(window));
    EventManager *l_eventManager = l_glfwManager->m_core->GetLuaManager()->GetEventManager();
    if(!l_eventManager->IsEventExists(EventType::TextInput)) return;
    try
    {
        utf8::utf32to8(&codepoint,&codepoint+1,std::back_inserter(l_glfwManager->m_input));
    }
    catch(const std::exception &e)
    {
        l_glfwManager->m_input.push_back('\0');
    }
    m_argument.PushArgument(l_glfwManager->m_input);
    l_eventManager->CallEvent(EventType::TextInput,m_argument);
    m_argument.Clear();
    l_glfwManager->m_input.clear();
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
            int l_count = 0;
            for(int i=GLFW_JOYSTICK_1; i < GLFW_JOYSTICK_LAST; i++)
            {
                if(!glfwJoystickPresent(i)) break;
                m_argument.PushArgument(i);

                const unsigned char *l_padButtons = glfwGetJoystickButtons(i,&l_count);
                m_argument.PushArgument(l_padButtons,static_cast<size_t>(l_count));

                const float *l_padAxes = glfwGetJoystickAxes(i,&l_count);
                m_argument.PushArgument(l_padAxes,static_cast<size_t>(l_count));

                l_eventManager->CallEvent(EventType::Joypad,m_argument);
                m_argument.Clear();
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
    f_cursor ? glfwSetCursor(m_window,f_cursor->GetCursor()) : glfwSetCursor(m_window,NULL);
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
