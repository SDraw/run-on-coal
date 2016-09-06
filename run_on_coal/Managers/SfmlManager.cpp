#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ConfigManager.h"
#include "Managers/EventManager.h"
#include "Managers/LogManager.h"
#include "Managers/LuaManager.h"
#include "Managers/SfmlManager.h"
#include "Lua/LuaArguments.h"
#include "Utils/Utils.h"

ROC::SfmlManager::SfmlManager(Core *f_core)
{
    m_core = f_core;

    std::string l_log;

    ConfigManager *l_configManager = m_core->GetConfigManager();
    glm::ivec2 l_windowSize;
    l_configManager->GetWindowSize(l_windowSize);
    sf::VideoMode l_videoMode(l_windowSize.x, l_windowSize.y);

    sf::ContextSettings l_contextSettings = sf::ContextSettings();
    l_contextSettings.antialiasingLevel = static_cast<unsigned int>(l_configManager->GetAntialiasing());
    l_contextSettings.depthBits = 32U;
    l_contextSettings.majorVersion = 4U;
    l_contextSettings.minorVersion = 0U;

    m_window = new sf::Window();
    m_window->create(l_videoMode, "ROC", l_configManager->IsFullscreenEnabled() ? sf::Style::Fullscreen : sf::Style::Default, l_contextSettings);
    m_window->setFramerateLimit(l_configManager->GetFPSLimit());
    m_window->setVerticalSyncEnabled(l_configManager->GetVSync());
    m_window->setKeyRepeatEnabled(false);

    glewExperimental = true;
    GLenum l_error = glewInit();
    if(l_error != GLEW_OK)
    {
        l_log.append((char*)glewGetErrorString(l_error));
        f_core->GetLogManager()->Log(l_log);
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

    m_active = true;
    m_argument = new LuaArguments();

    m_cursorMode = CURSOR_VISIBILITY_BIT;
}

ROC::SfmlManager::~SfmlManager()
{
    m_window->close();
    delete m_window;
    delete m_argument;
}

bool ROC::SfmlManager::DoPulse()
{
    ROC::EventManager *m_eventManager = m_core->GetLuaManager()->GetEventManager();

    while(m_window->pollEvent(m_event))
    {
        switch(m_event.type)
        {
            case sf::Event::Closed:
                m_active = false;
                break;
            case sf::Event::Resized:
            {
                m_argument->PushArgument(static_cast<int>(m_event.size.width));
                m_argument->PushArgument(static_cast<int>(m_event.size.height));
                m_eventManager->CallEvent(EventType::WindowResize, m_argument);
                m_argument->Clear();
            } break;
            case sf::Event::KeyPressed: case sf::Event::KeyReleased:
            {
                m_argument->PushArgument(m_event.key.code);
                m_argument->PushArgument(m_event.type == sf::Event::KeyPressed ? 1 : 0);
                m_eventManager->CallEvent(EventType::KeyPress, m_argument);
                m_argument->Clear();
            } break;
            case sf::Event::TextEntered:
            {
                if(m_event.text.unicode > 31 && !(m_event.text.unicode >= 127 && m_event.text.unicode <= 160))
                {
                    sf::String l_text(m_event.text.unicode);
                    auto l_utf8 = l_text.toUtf8();
                    m_input.insert(m_input.begin(), l_utf8.begin(), l_utf8.end());
                    m_argument->PushArgument(m_input);
                    m_eventManager->CallEvent(EventType::TextInput, m_argument);
                    m_argument->Clear();
                    m_input.clear();
                }
            } break;
            case sf::Event::MouseMoved:
            {
                m_argument->PushArgument(m_event.mouseMove.x);
                m_argument->PushArgument(m_event.mouseMove.y);
                m_eventManager->CallEvent(EventType::CursorMove, m_argument);
                m_argument->Clear();
            } break;
            case sf::Event::MouseEntered: case sf::Event::MouseLeft:
            {
                m_argument->PushArgument(m_event.type == sf::Event::MouseEntered ? 1 : 0);
                m_eventManager->CallEvent(EventType::CursorEnter, m_argument);
                m_argument->Clear();
            } break;
            case sf::Event::MouseButtonPressed: case sf::Event::MouseButtonReleased:
            {
                m_argument->PushArgument(m_event.mouseButton.button);
                m_argument->PushArgument(m_event.type == sf::Event::MouseButtonPressed ? 1 : 0);
                m_eventManager->CallEvent(EventType::MouseKeyPress, m_argument);
                m_argument->Clear();
            } break;
            case sf::Event::MouseWheelScrolled:
            {
                m_argument->PushArgument(m_event.mouseWheelScroll.wheel);
                m_argument->PushArgument(m_event.mouseWheelScroll.delta);
                m_eventManager->CallEvent(EventType::MouseScroll, m_argument);
                m_argument->Clear();
            } break;
            case sf::Event::JoystickConnected: case sf::Event::JoystickDisconnected:
            {
                m_argument->PushArgument(static_cast<int>(m_event.joystickConnect.joystickId));
                m_argument->PushArgument(m_event.type == sf::Event::JoystickConnected ? 1 : 0);
                m_eventManager->CallEvent(EventType::JoypadConnect, m_argument);
                m_argument->Clear();
            } break;
            case sf::Event::JoystickButtonPressed: case sf::Event::JoystickButtonReleased:
            {
                m_argument->PushArgument(static_cast<int>(m_event.joystickButton.joystickId));
                m_argument->PushArgument(static_cast<int>(m_event.joystickButton.button));
                m_argument->PushArgument(m_event.type == sf::Event::JoystickButtonPressed ? 1 : 0);
                m_eventManager->CallEvent(EventType::JoypadButton, m_argument);
                m_argument->Clear();
            } break;
            case sf::Event::JoystickMoved:
            {
                m_argument->PushArgument(static_cast<int>(m_event.joystickMove.joystickId));
                m_argument->PushArgument(static_cast<int>(m_event.joystickMove.axis));
                m_argument->PushArgument(m_event.joystickMove.position);
                m_eventManager->CallEvent(EventType::JoypadAxis, m_argument);
                m_argument->Clear();
            } break;
        }
    }
    return m_active;
}

void ROC::SfmlManager::SetCursorMode(unsigned char f_mode)
{
    if(f_mode == m_cursorMode) return;
    m_cursorMode = f_mode;
    m_window->setMouseCursorGrabbed((m_cursorMode & CURSOR_LOCK_BIT) == CURSOR_LOCK_BIT);
    m_window->setMouseCursorVisible((m_cursorMode & CURSOR_VISIBILITY_BIT) == CURSOR_VISIBILITY_BIT);
}
void ROC::SfmlManager::GetCursorPosition(glm::ivec2 &f_pos)
{
    sf::Vector2i l_position = sf::Mouse::getPosition();
    std::memcpy(&f_pos, &l_position, sizeof(glm::ivec2));
}
void ROC::SfmlManager::SetCursorPosition(glm::ivec2 &f_pos)
{
    sf::Mouse::setPosition((sf::Vector2i&)f_pos, *m_window);
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

float ROC::SfmlManager::GetTime()
{
    sf::Time l_time = m_clock.getElapsedTime();
    return l_time.asSeconds();
}