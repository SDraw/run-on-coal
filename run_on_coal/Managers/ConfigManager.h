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
    glm::uvec2 m_windowSize;
public:
    bool IsLoggingEnabled_();
    bool IsFullscreenEnabled();
    int GetAntialiasing();
    void GetWindowSize(glm::uvec2 &f_vec);
protected:
    ConfigManager(Core *f_core);
    ~ConfigManager();
    friend Core;
};

}
