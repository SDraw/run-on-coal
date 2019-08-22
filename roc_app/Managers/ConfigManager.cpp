#include "stdafx.h"

#include "Managers/ConfigManager.h"

#include "Utils/EnumUtils.h"

namespace ROC
{

const std::vector<std::string> g_ConfigAttributes
{
    "antialiasing", "dimension", "fullscreen", "logging", "fpslimit", "vsync", "vr", "module"
};
enum ConfigAttribute : size_t
{
    CA_Antialiasing = 0U,
    CA_Dimension,
    CA_Fullscreen,
    CA_Logging,
    CA_FPSLimit,
    CA_VSync,
    CA_VR,
    CA_Module
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
                        switch(EnumUtils::ReadEnumVector(l_param, g_ConfigAttributes))
                        {
                            case ConfigAttribute::CA_Antialiasing:
                            {
                                m_antialiasing = l_attrib.as_int(0);
                                if(m_antialiasing < 0) m_antialiasing = 0;
                            } break;
                            case ConfigAttribute::CA_Dimension:
                            {
                                std::string l_size = l_attrib.as_string("854x480");
                                std::replace(l_size.begin(), l_size.end(), 'x', ' ');
                                std::stringstream l_sizeStream(l_size);
                                l_sizeStream >> m_windowSize.x >> m_windowSize.y;
                            } break;
                            case ConfigAttribute::CA_Fullscreen:
                                m_fullscreen = l_attrib.as_bool(false);
                                break;
                            case ConfigAttribute::CA_Logging:
                                m_logging = l_attrib.as_bool(false);
                                break;
                            case ConfigAttribute::CA_FPSLimit:
                            {
                                m_fpsLimit = l_attrib.as_uint(60U);
                                if(m_fpsLimit == 0U) m_fpsLimit = 60U;
                            } break;
                            case ConfigAttribute::CA_VSync:
                                m_vsync = l_attrib.as_bool(false);
                                break;
                            case ConfigAttribute::CA_VR:
                                m_vrMode = l_attrib.as_bool(false);
                                break;
                            case ConfigAttribute::CA_Module:
                            {
                                std::string l_module = l_attrib.as_string();
                                if(!l_module.empty()) m_modules.push_back(l_module);
                            } break;
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
