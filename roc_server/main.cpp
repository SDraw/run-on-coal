#include "stdafx.h"
#include "RocInc.h"

volatile sig_atomic_t g_quitSetter = 0;
void SignalHandleFunction(int f_sig)
{
    signal(f_sig, SignalHandleFunction);
    g_quitSetter = 1;
}

int main(int argc, char *argv[])
{
    signal(SIGINT, SignalHandleFunction);
    signal(SIGTERM, SignalHandleFunction);
#ifdef SIGBREAK
    signal(SIGBREAK, SignalHandleFunction);
#endif
    ROC::Core *l_core = ROC::Core::Init();
    pugi::xml_document *l_meta = new pugi::xml_document();
    if(l_meta->load_file("server_scripts/meta.xml"))
    {
        pugi::xml_node l_metaRoot = l_meta->child("meta");
        if(l_metaRoot)
        {
            unsigned int l_counter = 0U;
            for(pugi::xml_node l_node = l_metaRoot.child("script"); l_node; l_node = l_node.next_sibling("script"))
            {
                pugi::xml_attribute l_attrib = l_node.attribute("src");
                if(l_attrib)
                {
                    std::string l_path("server_scripts/");
                    l_path.append(l_attrib.as_string());
                    l_core->GetLuaManager()->OpenFile(l_path);
                }
                else
                {
                    std::string l_text("Unable to find attribute 'src' at 'script' subnodes with ID ");
                    l_text.append(std::to_string(l_counter));
                    l_text.append(" in 'server_scripts/meta.xml'");
                    l_core->GetLogManager()->Log(l_text);
                }
                l_counter++;
            }
            if(!l_counter)
            {
                std::string l_text("Unable to find any 'script' subnode in 'server_scripts/meta.xml'");
                l_core->GetLogManager()->Log(l_text);
            }
        }
        else
        {
            std::string l_text("Unable to find root node 'meta' in 'server_scripts/meta.xml'.");
            l_core->GetLogManager()->Log(l_text);
        }
    }
    else
    {
        std::string l_text("Unable to find 'server_scripts/meta.xml'.");
        l_core->GetLogManager()->Log(l_text);
    }
    delete l_meta;

    ROC::LuaArguments *l_emptyArgs = new ROC::LuaArguments();
    l_core->GetLuaManager()->GetEventManager()->CallEvent(ROC::EventType::ServerStart, l_emptyArgs);

    while(!g_quitSetter) l_core->DoPulse();

    l_core->GetLuaManager()->GetEventManager()->CallEvent(ROC::EventType::ServerStop, l_emptyArgs);
    delete l_emptyArgs;
    ROC::Core::Terminate();
    return EXIT_SUCCESS;
}
