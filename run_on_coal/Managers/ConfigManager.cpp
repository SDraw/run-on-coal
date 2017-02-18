#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ConfigManager.h"
#include "Utils/Utils.h"

namespace ROC
{

const std::vector<std::string> g_configAttributeTable
{
    "antialiasing", "dimension", "fullscreen", "logging", "fpslimit", "vsync"
};

}

ROC::ConfigManager::ConfigManager()
{
    m_logging = false;
    m_fullscreen = false;
    m_antialiasing = 0;
    m_windowSize = glm::ivec2(854, 480);
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
                        switch(Utils::ReadEnumVector(g_configAttributeTable, l_param))
                        {
                            case 0:
                            {
                                m_antialiasing = l_attrib.as_int(0);
                                if(m_antialiasing < 0) m_antialiasing = 0;
                            } break;
                            case 1:
                            {
                                std::string l_size = l_attrib.as_string("854x480");
                                std::replace(l_size.begin(), l_size.end(), 'x', ' ');
                                std::stringstream l_sizeStream(l_size);
                                l_sizeStream >> m_windowSize.x >> m_windowSize.y;
                            } break;
                            case 2:
                                m_fullscreen = l_attrib.as_bool(false);
                                break;
                            case 3:
                                m_logging = l_attrib.as_bool(false);
                                break;
                            case 4:
                            {
                                m_fpsLimit = l_attrib.as_uint(60U);
                                if(m_fpsLimit == 0U) m_fpsLimit = 60U;
                            } break;
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
