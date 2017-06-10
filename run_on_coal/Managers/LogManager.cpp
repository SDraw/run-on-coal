#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ConfigManager.h"
#include "Managers/LogManager.h"

ROC::LogManager::LogManager(Core *f_core)
{
    m_enabled = f_core->GetConfigManager()->IsLogEnabled();
    if(m_enabled) m_log.open("log.txt", std::ofstream::out);
}
ROC::LogManager::~LogManager()
{
    if(m_enabled)
    {
        std::string l_log("Application closed");
        Log(l_log);
        m_log.close();
    }
}

void ROC::LogManager::Log(const std::string &f_text)
{
    if(m_enabled)
    {
        std::time_t l_time = std::time(nullptr);
        tm *l_tmTime = std::localtime(&l_time);
        m_log << "[" << l_tmTime->tm_hour << ":" << l_tmTime->tm_min << ":" << l_tmTime->tm_sec << "] " << f_text << std::endl;
#ifdef _DEBUG
        std::cout << "[" << l_tmTime->tm_hour << ":" << l_tmTime->tm_min << ":" << l_tmTime->tm_sec << "] " << f_text << std::endl;
#endif
    }
}
