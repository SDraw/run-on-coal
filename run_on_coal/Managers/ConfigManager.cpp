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
    m_windowSize = glm::uvec2(854U,480U);

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
                        switch(Utils::ReadEnumString(l_param,"antialiasing,dimension,fullscreen,logging"))
                        {
                            case 0:
                                m_antialiasing = int(l_attrib.as_uint());
                                break;
                            case 1:
                            {
                                std::string l_param = l_attrib.as_string();
                                if(l_param.length()) sscanf(l_param.c_str(),"%ux%u",&m_windowSize.x,&m_windowSize.y);
                            } break;
                            case 2:
                                m_fullscreen = l_attrib.as_bool();
                                break;
                            case 3:
                                m_logging = l_attrib.as_bool();
                                break;
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

bool ROC::ConfigManager::IsLoggingEnabled_()
{
    return m_logging;
}
bool ROC::ConfigManager::IsFullscreenEnabled()
{
    return m_fullscreen;
}
int ROC::ConfigManager::GetAntialiasing()
{
    return m_antialiasing;
}
void ROC::ConfigManager::GetWindowSize(glm::uvec2 &f_vec)
{
    std::memcpy(&f_vec,&m_windowSize,sizeof(glm::uvec2));
}
