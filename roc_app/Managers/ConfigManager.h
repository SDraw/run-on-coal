#pragma once

namespace ROC
{

class ConfigManager final
{
    bool m_logging;
    bool m_fullscreen;
    int m_antialiasing;
    glm::ivec2 m_windowSize;
    unsigned int m_fpsLimit;
    bool m_vsync;
    bool m_vrMode;
    std::vector<std::string> m_modules;

    ConfigManager(const ConfigManager &that) = delete;
    ConfigManager& operator=(const ConfigManager &that) = delete;
public:
    bool IsLogEnabled() const;
    bool IsFullscreenEnabled() const;
    int GetAntialiasing() const;
    const glm::ivec2& GetWindowSize() const;
    unsigned int GetFPSLimit() const;
    bool GetVSync() const;
    bool IsVRModeEnabled() const;
    const std::vector<std::string>& GetModules() const;
protected:
    ConfigManager();
    ~ConfigManager();

    friend class Core;
};

}
