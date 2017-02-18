#pragma once

namespace ROC
{

class ConfigManager
{
    bool m_configParsed;
    bool m_logging;
    std::string m_bindIP;
    unsigned short m_bindPort;
    unsigned short m_maxClients;
    unsigned int m_pulseTick;
public:
    inline bool IsLogEnabled() const { return m_logging; }
    inline void GetBindIP(std::string &f_ip) const { f_ip.assign(m_bindIP); }
    inline unsigned short GetBindPort() const { return m_bindPort; }
    inline unsigned short GetMaxClients() const { return m_maxClients; }
    inline unsigned int GetPulseTick() const { return m_pulseTick; }
    inline bool IsConfigParsed() const { return m_configParsed; }
protected:
    explicit ConfigManager();
    ~ConfigManager();
    friend class Core;
};

}
