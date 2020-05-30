#include "stdafx.h"

SFML_DEFINE_DISCRETE_GPU_PREFERENCE

#include "Managers/SfmlManager.h"
#include "Core/Core.h"
#include "Utils/CustomArguments.h"

#include "Managers/ConfigManager.h"
#include "Managers/LogManager.h"
#include "Managers/ModuleManager.h"
#include "Managers/PhysicsManager.h"
#include "Managers/RenderManager/RenderManager.h"
#include "Interfaces/IModule.h"
#include "GL/GLSetting.h"
#include "GL/GLState.h"

namespace ROC
{

const size_t g_GLMinimalVersion = 31U;
const char g_GLMinimalVersionString[] = "3.1";

const std::string g_DefaultIconPath("icons/coal.png");

const std::string g_KeyNames[]
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
const std::string g_MouseKeyNames[]
{
    "left", "right", "middle",
        "x1", "x2"
};
const std::string g_JoypadAxisNames[]
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

    ConfigManager *l_configManager = m_core->GetConfigManager();
    const glm::ivec2 &l_windowSize = l_configManager->GetWindowSize();
    m_windowVideoMode = sf::VideoMode(l_windowSize.x, l_windowSize.y);

    m_contextSettings.antialiasingLevel = static_cast<unsigned int>(l_configManager->GetAntialiasing());
    m_contextSettings.depthBits = 24U;
#ifdef _DEBUG
    m_contextSettings.attributeFlags = (sf::ContextSettings::Attribute::Core | sf::ContextSettings::Attribute::Debug);
#else
    m_contextSettings.attributeFlags = sf::ContextSettings::Attribute::Core;
#endif
    m_contextSettings.majorVersion = 0U;
    m_contextSettings.minorVersion = 0U;
    m_contextSettings.sRgbCapable = false;
    m_contextSettings.stencilBits = 0U;

    m_windowStyle = (l_configManager->IsFullscreenEnabled() ? sf::Style::Fullscreen : sf::Style::Default);

    m_window = new sf::Window(m_windowVideoMode, "RunOnCoal Engine Application", m_windowStyle, m_contextSettings);
    m_window->setActive(true);
    m_active = true;

    std::string l_log;
    if(GLSetting::GetString(GL_VERSION) == NULL)
    {
        l_log.assign("SFML: Unable to create OpenGL ");
        l_log.append(std::to_string(m_contextSettings.majorVersion));
        l_log.push_back('.');
        l_log.append(std::to_string(m_contextSettings.minorVersion));
        l_log.append(" core context. Check GPU for OpenGL support.");
        m_core->GetLogManager()->Log(l_log);

        MessageBoxA(m_window->getSystemHandle(), l_log.c_str(), NULL, MB_OK | MB_ICONEXCLAMATION);
        exit(EXIT_FAILURE);
    }
    else
    {
        sf::ContextSettings l_createdContextSettings = m_window->getSettings();
        if(l_createdContextSettings.majorVersion * 10U + l_createdContextSettings.minorVersion < g_GLMinimalVersion)
        {
            l_log.assign("SFML: Minimal supported OpenGL version - ");
            l_log.append(g_GLMinimalVersionString);
            l_log.push_back('\n');
            l_log.append("System OpenGL version - ");
            l_log.append(std::to_string(l_createdContextSettings.majorVersion));
            l_log.push_back('.');
            l_log.append(std::to_string(l_createdContextSettings.minorVersion));
            m_core->GetLogManager()->Log(l_log);

            MessageBoxA(m_window->getSystemHandle(), l_log.c_str(), NULL, MB_OK | MB_ICONEXCLAMATION);
            exit(EXIT_FAILURE);
        }
    }
    m_frameLimit = l_configManager->GetFPSLimit();
    m_window->setFramerateLimit(m_frameLimit);
    m_window->setVerticalSyncEnabled(l_configManager->GetVSync());
    m_window->setKeyRepeatEnabled(false);
    SetIcon(g_DefaultIconPath);

    GLenum l_error = glewInit();
    if(l_error != GLEW_OK)
    {
        l_log.assign("GLEW: ");
        l_log.append(reinterpret_cast<const char*>(glewGetErrorString(l_error)));
        m_core->GetLogManager()->Log(l_log);

        MessageBoxA(m_window->getSystemHandle(), l_log.c_str(), NULL, MB_OK | MB_ICONEXCLAMATION);
        exit(EXIT_FAILURE);
    }

    GLState::Finish(); // Wait for something

    l_log.assign("OpenGL ");
    l_log.append(reinterpret_cast<const char*>(GLSetting::GetString(GL_VERSION)));
    l_log.append(", ");
    l_log.append(reinterpret_cast<const char*>(GLSetting::GetString(GL_RENDERER)));
    l_log.append(", ");
    l_log.append("GLEW ");
    l_log.append(reinterpret_cast<const char*>(glewGetString(GLEW_VERSION)));
    m_core->GetLogManager()->Log(l_log);

    m_inputEnabled = false;
    std::memset(&m_event, 0, sizeof(sf::Event));

    m_arguments = new CustomArguments();
}

ROC::SfmlManager::~SfmlManager()
{
    GLSetting::MemoryCleanup();
    m_window->setActive(false);
    m_window->close();
    delete m_window;
    delete m_arguments;
}

void ROC::SfmlManager::GetWindowPosition(glm::ivec2 &f_pos) const
{
    sf::Vector2i l_position = m_window->getPosition();
    f_pos.x = l_position.x;
    f_pos.y = l_position.y;
}

void ROC::SfmlManager::SetWindowPosition(const glm::ivec2 &f_pos)
{
    sf::Vector2i l_position(f_pos.x, f_pos.y);
    m_window->setPosition(l_position);
}

void ROC::SfmlManager::GetWindowSize(glm::ivec2 &f_size) const
{
    sf::Vector2u l_size = m_window->getSize();
    f_size.x = static_cast<int>(l_size.x);
    f_size.y = static_cast<int>(l_size.y);
}

void ROC::SfmlManager::CloseWindow()
{
    m_active = false;
}

void ROC::SfmlManager::SetVSync(bool f_sync)
{
    m_window->setVerticalSyncEnabled(f_sync);
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

unsigned int ROC::SfmlManager::GetFramelimit() const
{
    return m_frameLimit;
}

void ROC::SfmlManager::SetTitle(const std::string &f_title)
{
    sf::String l_title = sf::String::fromUtf8(f_title.begin(), f_title.end());
    m_window->setTitle(l_title);
}

bool ROC::SfmlManager::SetIcon(const std::string &f_path)
{
    bool l_result = false;

    sf::Image l_image;
    if(l_image.loadFromFile(f_path))
    {
        sf::Vector2u l_size = l_image.getSize();
        m_window->setIcon(l_size.x, l_size.y, l_image.getPixelsPtr());
        l_result = true;
    }
    return l_result;
}

void ROC::SfmlManager::RequestFocus()
{
    m_window->requestFocus();
}

bool ROC::SfmlManager::GetFocusState() const
{
    return m_window->hasFocus();
}

void ROC::SfmlManager::SetInputEnabled(bool f_state)
{
    m_inputEnabled = f_state;
}

void ROC::SfmlManager::SetCursorMode(bool f_visible, bool f_lock)
{
    m_window->setMouseCursorGrabbed(f_lock);
    m_window->setMouseCursorVisible(f_visible);
}

void ROC::SfmlManager::GetCursorPosition(glm::ivec2 &f_pos) const
{
    sf::Vector2i l_position = sf::Mouse::getPosition(*m_window);
    std::memcpy(&f_pos, &l_position, sizeof(glm::ivec2));
}

void ROC::SfmlManager::SetCursorPosition(const glm::ivec2 &f_pos)
{
    sf::Mouse::setPosition((sf::Vector2i&)f_pos, *m_window);
}

void ROC::SfmlManager::GetClipboardString(std::string &f_str) const
{
    sf::String l_string = sf::Clipboard::getString();
    f_str.assign(l_string);
}

void ROC::SfmlManager::SetClipboardString(const std::string &f_str)
{
    sf::Clipboard::setString(f_str);
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

float ROC::SfmlManager::GetTime() const
{
    return m_time;
}

bool ROC::SfmlManager::DoPulse()
{
    m_time = m_clock.getElapsedTime().asSeconds();

    bool l_ignoreCursorEvent = false;
    while(m_window->pollEvent(m_event))
    {
        switch(m_event.type)
        {
            case sf::Event::Closed:
                m_active = false;
                break;
            case sf::Event::Resized:
            {
                glm::ivec2 l_size(static_cast<int>(m_event.size.width), static_cast<int>(m_event.size.height));
                m_core->GetRenderManager()->UpdateViewportSize(l_size);

                m_arguments->Push(l_size.x);
                m_arguments->Push(l_size.y);
                m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnWindowResize, m_arguments);
                m_arguments->Clear();
            } break;
            case sf::Event::GainedFocus: case sf::Event::LostFocus:
            {
                m_arguments->Push(m_event.type == sf::Event::GainedFocus ? 1 : 0);
                m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnWindowFocus, m_arguments);
                m_arguments->Clear();
            } break;
            case sf::Event::KeyPressed: case sf::Event::KeyReleased:
            {
                if(m_event.key.code != -1)
                {
                    m_arguments->Push(g_KeyNames[m_event.key.code]);
                    m_arguments->Push(m_event.type == sf::Event::KeyPressed ? 1 : 0);
                    m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnKeyPress, m_arguments);
                    m_arguments->Clear();
                }
            } break;
            case sf::Event::TextEntered:
            {
                if(m_inputEnabled)
                {
                    if(m_event.text.unicode > 31 && !(m_event.text.unicode >= 127 && m_event.text.unicode <= 160))
                    {
                        sf::String l_text(m_event.text.unicode);
                        std::basic_string<unsigned char> l_utf8 = l_text.toUtf8();
                        std::string l_input(l_utf8.begin(), l_utf8.end());

                        m_arguments->Push(l_input);
                        m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnTextInput, m_arguments);
                        m_arguments->Clear();
                    }
                }
            } break;
            case sf::Event::MouseMoved:
            {
                if(!l_ignoreCursorEvent) // Prevent loop if cursor position is changed in onCursorMove event
                {
                    m_arguments->Push(m_event.mouseMove.x);
                    m_arguments->Push(m_event.mouseMove.y);
                    m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnCursorMove, m_arguments);
                    m_arguments->Clear();
                    l_ignoreCursorEvent = true;
                }
            } break;
            case sf::Event::MouseEntered: case sf::Event::MouseLeft:
            {
                m_arguments->Push(m_event.type == sf::Event::MouseEntered ? 1 : 0);
                m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnCursorEnter, m_arguments);
                m_arguments->Clear();
            } break;
            case sf::Event::MouseButtonPressed: case sf::Event::MouseButtonReleased:
            {
                m_arguments->Push(g_MouseKeyNames[m_event.mouseButton.button]);
                m_arguments->Push(m_event.type == sf::Event::MouseButtonPressed ? 1 : 0);
                m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnMouseKeyPress, m_arguments);
                m_arguments->Clear();
            } break;
            case sf::Event::MouseWheelScrolled:
            {
                m_arguments->Push(m_event.mouseWheelScroll.wheel);
                m_arguments->Push(m_event.mouseWheelScroll.delta);
                m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnMouseScroll, m_arguments);
                m_arguments->Clear();
            } break;
            case sf::Event::JoystickConnected: case sf::Event::JoystickDisconnected:
            {
                m_arguments->Push(static_cast<int>(m_event.joystickConnect.joystickId));
                m_arguments->Push(m_event.type == sf::Event::JoystickConnected ? 1 : 0);
                m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnJoypadStateChange, m_arguments);
                m_arguments->Clear();
            } break;
            case sf::Event::JoystickButtonPressed: case sf::Event::JoystickButtonReleased:
            {
                m_arguments->Push(static_cast<int>(m_event.joystickButton.joystickId));
                m_arguments->Push(static_cast<int>(m_event.joystickButton.button));
                m_arguments->Push(m_event.type == sf::Event::JoystickButtonPressed ? 1 : 0);
                m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnJoypadButton, m_arguments);
                m_arguments->Clear();
            } break;
            case sf::Event::JoystickMoved:
            {
                m_arguments->Push(static_cast<int>(m_event.joystickMove.joystickId));
                m_arguments->Push(g_JoypadAxisNames[m_event.joystickMove.axis]);
                m_arguments->Push(m_event.joystickMove.position);
                m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnJoypadAxis, m_arguments);
                m_arguments->Clear();
            } break;
        }
    }
    return m_active;
}
