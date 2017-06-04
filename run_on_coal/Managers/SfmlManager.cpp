#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ConfigManager.h"
#include "Managers/EventManager.h"
#include "Managers/LogManager.h"
#include "Managers/LuaManager.h"
#include "Managers/PhysicsManager.h"
#include "Managers/SfmlManager.h"
#include "Elements/Shader.h"
#include "Lua/LuaArguments.h"
#include "Utils/Utils.h"

#define ROC_OPENGL_MIN_VERSION 31U
#define ROC_OPENGL_MIN_VERSION_STRING "3.1"

namespace ROC
{

extern const std::vector<std::string> g_keysTable
{
    "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
    "esc", "lctrl", "lshift", "lalt", "lsys", "rctrl", "rshift", "ralt", "rsys", "menu",
    "lbracket", "rbracket", "semicolon", "comma", "period", "quote", "slash", "backslash", "tilde", "equal", "dash",
    "space", "return", "backspace", "tab", "pgup", "pgdn", "end", "home", "insert", "delete",
    "num+", "num-", "num*", "num/",
    "arrow_l", "arrow_r", "arrow_u", "arrow_d",
    "num0", "num1", "num2", "num3", "num4", "num5", "num6", "num7", "num8", "num9",
    "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15",
    "pause"
};
extern const std::vector<std::string> g_mouseKeysTable
{
    "left", "right", "middle",
    "x1", "x2"
};
extern const std::vector<std::string> g_axisNames
{
    "X", "Y", "Z",
    "R", "U", "V",
    "PovX", "PovY"
};

}

ROC::SfmlManager::SfmlManager(Core *f_core)
{
    m_core = f_core;

    m_time = 0.f;

    std::string l_log;

    ConfigManager *l_configManager = m_core->GetConfigManager();
    glm::ivec2 l_windowSize;
    l_configManager->GetWindowSize(l_windowSize);
    m_windowVideoMode = sf::VideoMode(l_windowSize.x, l_windowSize.y);

    m_contextSettings.antialiasingLevel = static_cast<unsigned int>(l_configManager->GetAntialiasing());
    m_contextSettings.depthBits = 24U;
#ifdef _DEBUG
    m_contextSettings.attributeFlags = sf::ContextSettings::Attribute::Debug;
#endif
    m_windowStyle = (l_configManager->IsFullscreenEnabled() ? sf::Style::Fullscreen : sf::Style::Default);

    std::chrono::milliseconds l_windowInitWait(10U);
    m_active = true;
    m_eventPollThread = new std::thread(&ROC::SfmlManager::EventPollingThread, this);
    while(!m_created) std::this_thread::sleep_for(l_windowInitWait);
    m_window->setActive(true);

    if(glGetString(GL_VERSION) == NULL)
    {
        l_log.assign("SFML error: Unable to create OpenGL ");
        l_log.append(std::to_string(m_contextSettings.majorVersion));
        l_log.push_back('.');
        l_log.append(std::to_string(m_contextSettings.minorVersion));
        l_log.append(" context. Check supported version for your videocard");
        m_core->GetLogManager()->Log(l_log);

        MessageBoxA(m_window->getSystemHandle(), l_log.c_str(), NULL, MB_OK | MB_ICONSTOP);
        exit(EXIT_FAILURE);
    }
    else
    {
        sf::ContextSettings l_createdContextSettings = m_window->getSettings();
        if(l_createdContextSettings.majorVersion * 10U + l_createdContextSettings.minorVersion < ROC_OPENGL_MIN_VERSION)
        {
            l_log.assign("Minimal supported version of OpenGL is ");
            l_log.append(ROC_OPENGL_MIN_VERSION_STRING);
            m_core->GetLogManager()->Log(l_log);

            MessageBoxA(m_window->getSystemHandle(), l_log.c_str(), NULL, MB_OK | MB_ICONSTOP);
            exit(EXIT_FAILURE);
        }
    }
    m_frameLimit = l_configManager->GetFPSLimit();
    m_window->setFramerateLimit(m_frameLimit);
    m_window->setVerticalSyncEnabled(l_configManager->GetVSync());
    m_window->setKeyRepeatEnabled(false);

    GLenum l_error = glewInit();
    if(l_error != GLEW_OK)
    {
        l_log.assign("GLEW error: ");
        l_log.append(reinterpret_cast<const char*>(glewGetErrorString(l_error)));
        m_core->GetLogManager()->Log(l_log);

        MessageBoxA(m_window->getSystemHandle(), l_log.c_str(), NULL, MB_OK | MB_ICONSTOP);
        exit(EXIT_FAILURE);
    }

    l_log.assign("OpenGL ");
    l_log.append(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    l_log.append(", ");
    l_log.append(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    l_log.append(", ");
    l_log.append("GLEW ");
    l_log.append(reinterpret_cast<const char*>(glewGetString(GLEW_VERSION)));
    m_core->GetLogManager()->Log(l_log);

    m_argument = new LuaArguments();

    m_cursorMode = ROC_CURSOR_BIT_VISIBILITY;

    // Detect current GPU in list of bugged Sandy Bridge GPUs. Need to add more.
    if(l_log.find("HD Graphics 3000") != std::string::npos)  Shader::EnableUBOFix();

    m_recieveEvents = true;
}

ROC::SfmlManager::~SfmlManager()
{
    m_window->setActive(false);
    m_recieveEvents = false;
    m_eventPollThread->join();
    delete m_argument;
}

void ROC::SfmlManager::GetWindowPosition(glm::ivec2 &f_pos)
{
    sf::Vector2i l_position = m_window->getPosition();
    f_pos.x = l_position.x;
    f_pos.y = l_position.y;
}
void ROC::SfmlManager::GetWindowSize(glm::ivec2 &f_size)
{
    sf::Vector2u l_size = m_window->getSize();
    f_size.x = static_cast<int>(l_size.x);
    f_size.y = static_cast<int>(l_size.y);
}
void ROC::SfmlManager::SetFramelimit(unsigned int f_fps)
{
    if(f_fps != m_frameLimit)
    {
        m_frameLimit = f_fps;
        m_window->setFramerateLimit(m_frameLimit);
        m_core->GetPhysicsManager()->UpdateWorldSteps(m_frameLimit);
    }
}
bool ROC::SfmlManager::SetIcon(const std::string &f_path)
{
    bool l_result = false;

    std::string l_work, l_path(f_path);
    m_core->GetWorkingDirectory(l_work);
    Utils::EscapePath(l_path);
    l_path.insert(0U, l_work);

    sf::Image l_image;
    if(l_image.loadFromFile(l_path))
    {
        sf::Vector2u l_size = l_image.getSize();
        m_window->setIcon(l_size.x, l_size.y, l_image.getPixelsPtr());
        l_result = true;
    }
    return l_result;
}

void ROC::SfmlManager::SetCursorMode(unsigned char f_mode)
{
    if(f_mode != m_cursorMode)
    {
        m_cursorMode = f_mode;
        m_window->setMouseCursorGrabbed(CheckBit(m_cursorMode, ROC_CURSOR_BIT_LOCK));
        m_window->setMouseCursorVisible(CheckBit(m_cursorMode, ROC_CURSOR_BIT_VISIBILITY));
    }
}
void ROC::SfmlManager::GetCursorPosition(glm::ivec2 &f_pos)
{
    sf::Vector2i l_position = sf::Mouse::getPosition(*m_window);
    std::memcpy(&f_pos, &l_position, sizeof(glm::ivec2));
}
void ROC::SfmlManager::SetCursorPosition(const glm::ivec2 &f_pos)
{
    sf::Mouse::setPosition((sf::Vector2i&)f_pos, *m_window);
}

bool ROC::SfmlManager::IsKeyPressed(int f_key)
{
    return sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(f_key));
}
bool ROC::SfmlManager::IsMouseKeyPressed(int f_key)
{
    return sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(f_key));
}

bool ROC::SfmlManager::IsJoypadConnected(unsigned int f_jp)
{
    return sf::Joystick::isConnected(f_jp);
}
bool ROC::SfmlManager::GetJoypadButtonState(unsigned int f_jp, unsigned int f_button)
{
    return sf::Joystick::isButtonPressed(f_jp, f_button);
}
unsigned int ROC::SfmlManager::GetJoypadButtonCount(unsigned int f_jp)
{
    return sf::Joystick::getButtonCount(f_jp);
}
bool ROC::SfmlManager::CheckJoypadAxis(unsigned int f_jp, unsigned int f_axis)
{
    return sf::Joystick::hasAxis(f_jp, static_cast<sf::Joystick::Axis>(f_axis));
}
float ROC::SfmlManager::GetJoypadAxisValue(unsigned int f_jp, unsigned int f_axis)
{
    return sf::Joystick::getAxisPosition(f_jp, static_cast<sf::Joystick::Axis>(f_axis));
}

void ROC::SfmlManager::EventPollingThread()
{
    m_window = new sf::Window();
    m_window->create(m_windowVideoMode, "RunOnCoal", m_windowStyle, m_contextSettings);
    m_window->setActive(false);
    m_created = true;

    auto l_sleepTime = std::chrono::milliseconds(10U);
    sf::Event l_event;
    while(m_active || m_recieveEvents)
    {
        if(m_recieveEvents)
        {
            m_eventMutex.lock();
            while(m_window->pollEvent(l_event)) m_eventQueue.push(l_event);
            m_eventMutex.unlock();
        }
        std::this_thread::sleep_for(l_sleepTime);
    }
    m_window->setActive(true);
    m_window->close();
    delete m_window;
    m_window = NULL;
    m_created = false;
}

bool ROC::SfmlManager::DoPulse()
{
    m_time = m_clock.getElapsedTime().asSeconds();

    if(m_eventMutex.try_lock())
    {
        while(!m_eventQueue.empty())
        {
            sf::Event &l_event = m_eventQueue.front();
            switch(l_event.type)
            {
                case sf::Event::Closed:
                    m_active = false;
                    break;
                case sf::Event::Resized:
                {
                    m_argument->PushArgument(static_cast<int>(l_event.size.width));
                    m_argument->PushArgument(static_cast<int>(l_event.size.height));
                    m_core->GetLuaManager()->GetEventManager()->CallEvent("onWindowResize", m_argument);
                    m_argument->Clear();
                } break;
                case sf::Event::GainedFocus: case sf::Event::LostFocus:
                {
                    m_argument->PushArgument(l_event.type == sf::Event::GainedFocus ? 1 : 0);
                    m_core->GetLuaManager()->GetEventManager()->CallEvent("onWindowFocus", m_argument);
                    m_argument->Clear();
                } break;
                case sf::Event::KeyPressed: case sf::Event::KeyReleased:
                {
                    if(l_event.key.code != -1)
                    {
                        m_argument->PushArgument(g_keysTable[l_event.key.code]);
                        m_argument->PushArgument(l_event.type == sf::Event::KeyPressed ? 1 : 0);
                        m_core->GetLuaManager()->GetEventManager()->CallEvent("onKeyPress", m_argument);
                        m_argument->Clear();
                    }
                } break;
                case sf::Event::TextEntered:
                {
                    if(l_event.text.unicode > 31 && !(l_event.text.unicode >= 127 && l_event.text.unicode <= 160))
                    {
                        sf::String l_text(l_event.text.unicode);
                        std::basic_string<unsigned char> l_utf8 = l_text.toUtf8();
                        std::string l_input(l_utf8.begin(), l_utf8.end());
                        m_argument->PushArgument(l_input);
                        m_core->GetLuaManager()->GetEventManager()->CallEvent("onTextInput", m_argument);
                        m_argument->Clear();
                    }
                } break;
                case sf::Event::MouseMoved:
                {
                    m_argument->PushArgument(l_event.mouseMove.x);
                    m_argument->PushArgument(l_event.mouseMove.y);
                    m_core->GetLuaManager()->GetEventManager()->CallEvent("onCursorMove", m_argument);
                    m_argument->Clear();
                } break;
                case sf::Event::MouseEntered: case sf::Event::MouseLeft:
                {
                    m_argument->PushArgument(l_event.type == sf::Event::MouseEntered ? 1 : 0);
                    m_core->GetLuaManager()->GetEventManager()->CallEvent("onCursorEnter", m_argument);
                    m_argument->Clear();
                } break;
                case sf::Event::MouseButtonPressed: case sf::Event::MouseButtonReleased:
                {
                    m_argument->PushArgument(g_mouseKeysTable[l_event.mouseButton.button]);
                    m_argument->PushArgument(l_event.type == sf::Event::MouseButtonPressed ? 1 : 0);
                    m_core->GetLuaManager()->GetEventManager()->CallEvent("onMouseKeyPress", m_argument);
                    m_argument->Clear();
                } break;
                case sf::Event::MouseWheelScrolled:
                {
                    m_argument->PushArgument(l_event.mouseWheelScroll.wheel);
                    m_argument->PushArgument(l_event.mouseWheelScroll.delta);
                    m_core->GetLuaManager()->GetEventManager()->CallEvent("onMouseScroll", m_argument);
                    m_argument->Clear();
                } break;
                case sf::Event::JoystickConnected: case sf::Event::JoystickDisconnected:
                {
                    m_argument->PushArgument(static_cast<int>(l_event.joystickConnect.joystickId));
                    m_argument->PushArgument(l_event.type == sf::Event::JoystickConnected ? 1 : 0);
                    m_core->GetLuaManager()->GetEventManager()->CallEvent("onJoypadConnect", m_argument);
                    m_argument->Clear();
                } break;
                case sf::Event::JoystickButtonPressed: case sf::Event::JoystickButtonReleased:
                {
                    m_argument->PushArgument(static_cast<int>(l_event.joystickButton.joystickId));
                    m_argument->PushArgument(static_cast<int>(l_event.joystickButton.button));
                    m_argument->PushArgument(l_event.type == sf::Event::JoystickButtonPressed ? 1 : 0);
                    m_core->GetLuaManager()->GetEventManager()->CallEvent("onJoypadButton", m_argument);
                    m_argument->Clear();
                } break;
                case sf::Event::JoystickMoved:
                {
                    m_argument->PushArgument(static_cast<int>(l_event.joystickMove.joystickId));
                    m_argument->PushArgument(g_axisNames[l_event.joystickMove.axis]);
                    m_argument->PushArgument(l_event.joystickMove.position);
                    m_core->GetLuaManager()->GetEventManager()->CallEvent("onJoypadAxis", m_argument);
                    m_argument->Clear();
                } break;
            }
            m_eventQueue.pop();
        }
        m_eventMutex.unlock();
    }
    return m_active;
}
