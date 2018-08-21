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
    std::string m_scriptsDir;

    ConfigManager(const ConfigManager &that);
    ConfigManager& operator=(const ConfigManager &that);
public:
    inline bool IsLogEnabled() const { return m_logging; }
    inline bool IsFullscreenEnabled() const { return m_fullscreen; }
    inline int GetAntialiasing() const { return m_antialiasing; }
    inline const glm::ivec2& GetWindowSize() const { return m_windowSize; }
    inline unsigned int GetFPSLimit() const { return m_fpsLimit; }
    inline bool GetVSync() const { return m_vsync; }
    inline bool IsVRModeEnabled() const { return m_vrMode; }
    inline const std::string& GetScriptsDirectory() const { return m_scriptsDir; }
protected:
    ConfigManager();
    ~ConfigManager();

    friend class Core;
};

}
