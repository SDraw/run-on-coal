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
public:
    inline bool IsLogEnabled() const { return m_logging; }
    inline bool IsFullscreenEnabled() const { return m_fullscreen; }
    inline int GetAntialiasing() const { return m_antialiasing; }
    inline void GetWindowSize(glm::ivec2 &f_vec) { std::memcpy(&f_vec, &m_windowSize, sizeof(glm::ivec2)); }
    inline unsigned int GetFPSLimit() const { return m_fpsLimit; }
    inline bool GetVSync() const { return m_vsync; }
protected:
    ConfigManager();
    ~ConfigManager();

    friend class Core;
};

}
