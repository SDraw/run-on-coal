#include "stdafx.h"
#include "RocInc.h"

#ifdef _DEBUG || __linux__
int main(int argc, char *argv[])
#elif _WIN32
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
#endif
{
    ROC::Core *l_core = ROC::Core::GetCore();
    pugi::xml_document *l_meta = new pugi::xml_document();
    if(l_meta->load_file("scripts/meta.xml"))
    {
        ROC::LuaManager *l_luaManager = l_core->GetLuaManager();
        pugi::xml_node l_metaRoot = l_meta->child("meta");
        if(l_metaRoot)
        {
            for(pugi::xml_node l_node = l_metaRoot.child("script"); l_node; l_node = l_node.next_sibling("script"))
            {
                pugi::xml_attribute l_attrib = l_node.attribute("src");
                if(l_attrib)
                {
                    std::string l_path("scripts/");
                    l_path.append(l_attrib.as_string());
                    l_luaManager->OpenFile(l_path);
                }
            }
        }
    }
    delete l_meta;
    while(l_core->DoPulse());
    ROC::Core::Terminate();
    return EXIT_SUCCESS;
}
