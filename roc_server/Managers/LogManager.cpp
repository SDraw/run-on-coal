#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ConfigManager.h"
#include "Managers/LogManager.h"

ROC::LogManager::LogManager(Core *f_core)
{
    m_enabled = f_core->GetConfigManager()->IsLogEnabled();
    if(m_enabled) m_log.open("server_log.txt", std::ofstream::out);
    Log("Server is starting");
    if(!f_core->GetConfigManager()->IsConfigParsed()) Log("Unable to parse 'server_settings.xml'. Default configuration is used.");
}
ROC::LogManager::~LogManager()
{
    Log("Server is shutting down");
    if(m_enabled) m_log.close();
}

void ROC::LogManager::Log(const std::string &f_text)
{
    std::time_t l_time = std::time(NULL);
    tm *l_tmTime = std::localtime(&l_time);
    std::cout << "[" << l_tmTime->tm_hour << ":" << l_tmTime->tm_min << ":" << l_tmTime->tm_sec << "] " << f_text.c_str() << std::endl;
    if(m_enabled) m_log << "[" << l_tmTime->tm_hour << ":" << l_tmTime->tm_min << ":" << l_tmTime->tm_sec << "] " << f_text.c_str() << std::endl;
}
