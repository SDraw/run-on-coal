#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LogManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/NetworkManager.h"
#include "Elements/Client.h"
#include "Elements/File.h"
#include "Utils/Utils.h"

ROC::ElementManager::ElementManager(Core *f_core)
{
    m_core = f_core;
}
ROC::ElementManager::~ElementManager()
{
}

ROC::Client* ROC::ElementManager::CreateClient()
{
    Client *l_client = new Client();
    m_core->GetMemoryManager()->AddMemoryPointer(l_client);
    return l_client;
}
void ROC::ElementManager::DestroyClient(Client *f_client)
{
    if(m_core->GetMemoryManager()->IsValidMemoryPointer(f_client))
    {
        m_core->GetMemoryManager()->RemoveMemoryPointer(f_client);
        delete f_client;
    }
}

ROC::File* ROC::ElementManager::CreateFile_(const std::string &f_path)
{
    File *l_file = new File();

    std::string l_path, l_work;
    m_core->GetWorkingDirectory(l_work);
    Utils::AnalyzePath(f_path, l_path);
    Utils::JoinPaths(l_work, l_path);

    if(l_file->Create(l_work, f_path)) m_core->GetMemoryManager()->AddMemoryPointer(l_file);
    else
    {
        delete l_file;
        l_file = NULL;
    }
    return l_file;
}
ROC::File* ROC::ElementManager::OpenFile(const std::string &f_path, bool f_ro)
{
    File *l_file = new File();

    std::string l_path, l_work;
    m_core->GetWorkingDirectory(l_work);
    Utils::AnalyzePath(f_path, l_path);
    Utils::JoinPaths(l_work, l_path);

    if(l_file->Open(l_work, f_path, f_ro)) m_core->GetMemoryManager()->AddMemoryPointer(l_file);
    else
    {
        delete l_file;
        l_file = NULL;
    }
    return l_file;
}
void ROC::ElementManager::CloseFile(File *f_file)
{
    if(m_core->GetMemoryManager()->IsValidMemoryPointer(f_file))
    {
        m_core->GetMemoryManager()->RemoveMemoryPointer(f_file);
        delete f_file;
    }
}

void ROC::ElementManager::DestroyElementByPointer(void *f_element)
{
    //Called only at the end of work
    delete reinterpret_cast<Element*>(f_element);
}
