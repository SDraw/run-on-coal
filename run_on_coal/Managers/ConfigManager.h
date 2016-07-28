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
    bool IsLogEnabled();
    bool IsFullscreenEnabled();
    int GetAntialiasing();
    void GetWindowSize(glm::ivec2 &f_vec);
    unsigned int GetFPSLimit();
    bool GetVSync();
protected:
    ConfigManager(Core *f_core);
    ~ConfigManager();
    friend Core;
};

}
