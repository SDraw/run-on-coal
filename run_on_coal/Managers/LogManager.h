#pragma once

namespace ROC
{

class Core;
class LogManager
{
    std::ofstream m_log;
    bool m_enabled;
public:
    void Log(std::string &f_text);
protected:
    LogManager(Core *f_core);
    ~LogManager();
    friend Core;
};

}
