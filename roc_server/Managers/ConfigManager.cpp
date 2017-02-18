#include "stdafx.h"
#include "Managers/ConfigManager.h"
#include "Utils/Utils.h"

namespace ROC
{

const std::vector<std::string> g_configAttributeTable
{
    "logging", "ip", "port", "max_clients", "pulse_tick"
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
                        switch(Utils::ReadEnumVector(g_configAttributeTable, l_param))
                        {
                            case 0:
                                m_logging = l_attrib.as_bool(true);
                                break;
                            case 1:
                                m_bindIP.assign(l_attrib.as_string("0.0.0.0"));
                                break;
                            case 2:
                                m_bindPort = static_cast<unsigned short>(l_attrib.as_uint(4200U));
                                break;
                            case 3:
                                m_maxClients = static_cast<unsigned short>(l_attrib.as_uint(10U));
                                break;
                            case 4:
                                m_pulseTick = l_attrib.as_uint(10U);
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
