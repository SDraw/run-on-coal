#include "stdafx.h"

#include "Managers/LogManager.h"
#include "Core/Core.h"

#include "Managers/ConfigManager.h"

ROC::LogManager::LogManager(Core *f_core)
{
    m_enabled = f_core->GetConfigManager()->IsLogEnabled();
    if(m_enabled)
    {
        m_log.open("server_log.txt", std::ofstream::out);
        if(m_log.fail())
        {
            m_log.clear();
            m_enabled = false;
        }
    }
    Log("Server is starting...");
    if(!f_core->GetConfigManager()->IsConfigParsed()) Log("Unable to parse 'server_settings.xml'. Default configuration is used.");
}
ROC::LogManager::~LogManager()
{
    if(m_enabled) m_log.close();
}

void ROC::LogManager::Log(const std::string &f_text)
{
    std::time_t l_time = std::time(nullptr);
    tm *l_tmTime = std::localtime(&l_time);
    std::cout << "[" << l_tmTime->tm_hour << ":" << l_tmTime->tm_min << ":" << l_tmTime->tm_sec << "] " << f_text << std::endl;
    if(m_enabled) m_log << "[" << l_tmTime->tm_hour << ":" << l_tmTime->tm_min << ":" << l_tmTime->tm_sec << "] " << f_text << std::endl;
}
