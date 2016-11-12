#include "stdafx.h"
#include "RocInc.h"

#ifdef _WIN32
#ifdef _DEBUG
int main(int argc, char *argv[])
#else
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
#endif
#elif __linux__
int main(int argc, char *argv[])
#endif
{
    ROC::Core *l_core = ROC::Core::GetCore();
    ROC::LogManager *l_logManager = l_core->GetLogManager();
    pugi::xml_document *l_meta = new pugi::xml_document();
    if(l_meta->load_file("scripts/meta.xml"))
    {
        ROC::LuaManager *l_luaManager = l_core->GetLuaManager();
        pugi::xml_node l_metaRoot = l_meta->child("meta");
        if(l_metaRoot)
        {
            unsigned int l_counter = 0U;
            for(pugi::xml_node l_node = l_metaRoot.child("script"); l_node; l_node = l_node.next_sibling("script"))
            {
                pugi::xml_attribute l_attrib = l_node.attribute("src");
                if(l_attrib)
                {
                    std::string l_path("scripts/");
                    l_path.append(l_attrib.as_string());
                    l_luaManager->OpenFile(l_path);
                }
                else
                {
                    std::string l_text("Unable to find attribute 'src' at 'script' subnodes with ID ");
                    l_text.append(std::to_string(l_counter));
                    l_text.append(" in 'scripts/meta.xml'");
                    l_logManager->Log(l_text);
                }
                l_counter++;
            }
            if(!l_counter)
            {
                std::string l_text("Unable to find any 'script' subnode in 'scripts/meta.xml'");
                l_logManager->Log(l_text);
            }
        }
        else
        {
            std::string l_text("Unable to find root node 'meta' in 'scripts/meta.xml'.");
            l_logManager->Log(l_text);
        }
    }
    else
    {
        std::string l_text("Unable to find 'scripts/meta.xml'.");
        l_logManager->Log(l_text);
    }
    delete l_meta;

    ROC::LuaArguments *l_emptyArgs = new ROC::LuaArguments();
    ROC::EventManager *l_eventManager = l_core->GetLuaManager()->GetEventManager();
    l_eventManager->CallEvent(ROC::EventType::AppStart, l_emptyArgs);

    while(l_core->DoPulse());

    l_eventManager->CallEvent(ROC::EventType::AppStop, l_emptyArgs);
    delete l_emptyArgs;

    ROC::Core::Terminate();
    return EXIT_SUCCESS;
}