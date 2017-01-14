#pragma once

namespace ROC
{

class Core;
class ConfigManager
{
    Core* m_core;

    bool m_configParsed;
    bool m_logging;
    std::string m_bindIP;
    unsigned short m_bindPort;
    unsigned short m_maxClients;
    unsigned int m_pulseTick;
public:
    inline bool IsLogEnabled() { return m_logging; }
    inline void GetBindIP(std::string &f_ip) { f_ip.assign(m_bindIP); }
    inline unsigned short GetBindPort() { return m_bindPort; }
    inline unsigned short GetMaxClients() { return m_maxClients; }
    inline unsigned int GetPulseTick() { return m_pulseTick; }
    inline bool IsConfigParsed() { return m_configParsed; }
protected:
    explicit ConfigManager(Core *f_core);
    ~ConfigManager();
    friend Core;
};

}
