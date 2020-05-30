#include "stdafx.h"

#include "Managers/ConfigManager.h"

#include "Utils/EnumUtils.h"

namespace ROC
{

const std::vector<std::string> g_configAttributes
{
    "logging", "ip", "port", "max_clients", "pulse_tick", "module"
};
enum ConfigAttribute : size_t
{
    CA_Logging = 0U,
    CA_IP,
    CA_Port,
    CA_MaxClients,
    CA_PulseTick,
    CA_Module
};

}

ROC::ConfigManager::ConfigManager()
{
    m_configParsed = false;
    m_logging = true;
    m_bindIP.assign("0.0.0.0");
    m_bindPort = 4200U;
    m_maxClients = 10U;
    m_pulseTick = 10U;

    pugi::xml_document *l_settings = new pugi::xml_document();
    if(l_settings->load_file("server_settings.xml"))
    {
        pugi::xml_node l_root = l_settings->child("settings");
        if(l_root)
        {
            for(pugi::xml_node l_node = l_root.child("setting"); l_node; l_node = l_node.next_sibling("setting"))
            {
                pugi::xml_attribute l_attribName = l_node.attribute("name");
                pugi::xml_attribute l_attribValue = l_node.attribute("value");
                if(l_attribName && l_attribValue)
                {
                    switch(EnumUtils::ReadEnumVector(l_attribName.as_string(), g_configAttributes))
                    {
                        case ConfigAttribute::CA_Logging:
                            m_logging = l_attribValue.as_bool(true);
                            break;
                        case ConfigAttribute::CA_IP:
                            m_bindIP.assign(l_attribValue.as_string("0.0.0.0"));
                            break;
                        case ConfigAttribute::CA_Port:
                            m_bindPort = static_cast<unsigned short>(l_attribValue.as_uint(4200U));
                            break;
                        case ConfigAttribute::CA_MaxClients:
                            m_maxClients = static_cast<unsigned short>(l_attribValue.as_uint(10U));
                            break;
                        case ConfigAttribute::CA_PulseTick:
                            m_pulseTick = l_attribValue.as_uint(10U);
                            break;
                        case ConfigAttribute::CA_Module:
                        {
                            std::string l_module(l_attribValue.as_string());
                            if(!l_module.empty()) m_modules.push_back(l_module);
                        } break;
                    }
                }
            }
        }
        m_configParsed = true;
    }
    delete l_settings;
}

ROC::ConfigManager::~ConfigManager()
{
}
