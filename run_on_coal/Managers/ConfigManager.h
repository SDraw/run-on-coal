#pragma once

namespace ROC
{

class Core;
class ConfigManager
{
    Core* m_core;
    bool m_logging;
    bool m_fullscreen;
    int m_antialiasing;
    glm::ivec2 m_windowSize;
    unsigned int m_fpsLimit;
    bool m_vsync;
public:
    inline bool IsLogEnabled() { return m_logging; }
    inline bool IsFullscreenEnabled() { return m_fullscreen; }
    inline int GetAntialiasing() { return m_antialiasing; }
    inline void GetWindowSize(glm::ivec2 &f_vec) { std::memcpy(&f_vec,&m_windowSize,sizeof(glm::ivec2)); }
    inline unsigned int GetFPSLimit() { return m_fpsLimit; }
    inline bool GetVSync() { return m_vsync; }
protected:
    ConfigManager(Core *f_core);
    ~ConfigManager();
    friend Core;
};

}
