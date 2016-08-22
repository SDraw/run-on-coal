#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ConfigManager.h"
#include "Utils/Utils.h"

ROC::ConfigManager::ConfigManager(Core* f_core)
{
    m_core = f_core;

    m_logging = true;
    m_fullscreen = false;
    m_antialiasing = 0;
    m_windowSize = glm::ivec2(854,480);
    m_fpsLimit = 60U;
    m_vsync = false;

    pugi::xml_document *l_settings = new pugi::xml_document();
    if(l_settings->load_file("settings.xml"))
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
                        switch(Utils::ReadEnumString(l_param,"antialiasing,dimension,fullscreen,logging,fpslimit,vsync"))
                        {
                        case 0:
                            m_antialiasing = l_attrib.as_int(0);
                            break;
                        case 1:
                        {
                            std::string l_param = l_attrib.as_string("854x480");
                            if(l_param.length()) sscanf(l_param.c_str(),"%dx%d",&m_windowSize.x,&m_windowSize.y);
                        } break;
                        case 2:
                            m_fullscreen = l_attrib.as_bool(false);
                            break;
                        case 3:
                            m_logging = l_attrib.as_bool(true);
                            break;
                        case 4:
                            m_fpsLimit = l_attrib.as_uint(60U);
                            break;
                        case 5:
                            m_vsync = l_attrib.as_bool(false);
                        }
                    }
                }
            }
        }
    }
    delete l_settings;
}
ROC::ConfigManager::~ConfigManager()
{
}
