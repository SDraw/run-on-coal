#include "stdafx.h"

#include "Managers/ConfigManager.h"

#include "Utils/EnumUtils.h"

#define ROC_CONFIG_ATTRIB_LOGGING 0
#define ROC_CONFIG_ATTRIB_IP 1
#define ROC_CONFIG_ATTRIB_PORT 2
#define ROC_CONFIG_ATTRIB_MAXCLIENTS 3
#define ROC_CONFIG_ATTRIB_PULSETICK 4
#define ROC_CONFIG_ATTRIB_SCRIPTSDIR 5

namespace ROC
{

const std::vector<std::string> g_configAttributeTable
{
    "logging", "ip", "port", "max_clients", "pulse_tick", "scripts_dir"
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
    m_scriptsDir.assign("server_scripts");

    pugi::xml_document *l_settings = new pugi::xml_document();
    if(l_settings->load_file("server_settings.xml"))
    {
        pugi::xml_node l_metaRoot = l_settings->child("settings");
        if(l_metaRoot)
        {
            for(pugi::xml_node l_node = l_metaRoot.child("param"); l_node; l_node = l_node.next_sibling("param"))
            {
                pugi::xml_attribute l_attrib = l_node.attribute("name");
                if(l_attrib)
                {
                    std::string l_param = l_attrib.as_string();
                    l_attrib = l_node.attribute("value");
                    if(l_attrib)
                    {
                        switch(EnumUtils::ReadEnumVector(l_param, g_configAttributeTable))
                        {
                            case ROC_CONFIG_ATTRIB_LOGGING:
                                m_logging = l_attrib.as_bool(true);
                                break;
                            case ROC_CONFIG_ATTRIB_IP:
                                m_bindIP.assign(l_attrib.as_string("0.0.0.0"));
                                break;
                            case ROC_CONFIG_ATTRIB_PORT:
                                m_bindPort = static_cast<unsigned short>(l_attrib.as_uint(4200U));
                                break;
                            case ROC_CONFIG_ATTRIB_MAXCLIENTS:
                                m_maxClients = static_cast<unsigned short>(l_attrib.as_uint(10U));
                                break;
                            case ROC_CONFIG_ATTRIB_PULSETICK:
                                m_pulseTick = l_attrib.as_uint(10U);
                                break;
                            case ROC_CONFIG_ATTRIB_SCRIPTSDIR:
                                m_scriptsDir.assign(l_attrib.as_string("server_scripts"));
                                break;
                        }
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
