#include "stdafx.h"

#include "Managers/ConfigManager.h"

#include "Core/Core.h"
#include "Utils/EnumUtils.h"

#define ROC_CONFIG_ATTRIB_ANTIALIASING 0
#define ROC_CONFIG_ATTRIB_DIMENSION 1
#define ROC_CONFIG_ATTRIB_FULLSCREEN 2
#define ROC_CONFIG_ATTRIB_LOGGING 3
#define ROC_CONFIG_ATTRIB_FPSLIMIT 4
#define ROC_CONFIG_ATTRIB_VSYNC 5
#define ROC_CONFIG_ATTRIB_VR 6
#define ROC_CONFIG_ATTRIB_SCRIPTSDIR 7

namespace ROC
{

const std::vector<std::string> g_configAttributeTable
{
    "antialiasing", "dimension", "fullscreen", "logging", "fpslimit", "vsync", "vr", "scripts_dir"
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
    m_vrMode = false;
    m_scriptsDir.assign("scripts");

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
                        switch(EnumUtils::ReadEnumVector(l_param, g_configAttributeTable))
                        {
                            case ROC_CONFIG_ATTRIB_ANTIALIASING:
                            {
                                m_antialiasing = l_attrib.as_int(0);
                                if(m_antialiasing < 0) m_antialiasing = 0;
                            } break;
                            case ROC_CONFIG_ATTRIB_DIMENSION:
                            {
                                std::string l_size = l_attrib.as_string("854x480");
                                std::replace(l_size.begin(), l_size.end(), 'x', ' ');
                                std::stringstream l_sizeStream(l_size);
                                l_sizeStream >> m_windowSize.x >> m_windowSize.y;
                            } break;
                            case ROC_CONFIG_ATTRIB_FULLSCREEN:
                                m_fullscreen = l_attrib.as_bool(false);
                                break;
                            case ROC_CONFIG_ATTRIB_LOGGING:
                                m_logging = l_attrib.as_bool(false);
                                break;
                            case ROC_CONFIG_ATTRIB_FPSLIMIT:
                            {
                                m_fpsLimit = l_attrib.as_uint(60U);
                                if(m_fpsLimit == 0U) m_fpsLimit = 60U;
                            } break;
                            case ROC_CONFIG_ATTRIB_VSYNC:
                                m_vsync = l_attrib.as_bool(false);
                                break;
                            case ROC_CONFIG_ATTRIB_VR:
                                m_vrMode = l_attrib.as_bool(false);
                                break;
                            case ROC_CONFIG_ATTRIB_SCRIPTSDIR:
                                m_scriptsDir.assign(l_attrib.as_string("scripts"));
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
