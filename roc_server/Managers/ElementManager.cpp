#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LogManager.h"
#include "Managers/MemoryManager.h"
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
    m_core->GetMemoryManager()->AddMemoryPointer(l_client, ElementType::ClientElement);
    return l_client;
}
void ROC::ElementManager::DestroyClient(Client *f_client)
{
    m_core->GetMemoryManager()->RemoveMemoryPointer(f_client, ElementType::ClientElement);
    delete f_client;
}

ROC::File* ROC::ElementManager::CreateFile_(std::string &f_path)
{
    File *l_file = new File();

    std::string l_path, l_work;
    m_core->GetWorkingDirectory(l_work);
    Utils::AnalyzePath(f_path, l_path);
    Utils::JoinPaths(l_work, l_path);

    if(l_file->Create(l_work, f_path)) m_core->GetMemoryManager()->AddMemoryPointer(l_file, ElementType::FileElement);
    else
    {
        delete l_file;
        l_file = NULL;
    }
    return l_file;
}
ROC::File* ROC::ElementManager::OpenFile(std::string &f_path, bool f_ro)
{
    File *l_file = new File();

    std::string l_path, l_work;
    m_core->GetWorkingDirectory(l_work);
    Utils::AnalyzePath(f_path, l_path);
    Utils::JoinPaths(l_work, l_path);

    if(l_file->Open(l_work, f_path, f_ro)) m_core->GetMemoryManager()->AddMemoryPointer(l_file, ElementType::FileElement);
    else
    {
        delete l_file;
        l_file = NULL;
    }
    return l_file;
}
bool ROC::ElementManager::DestroyFile(File *f_file)
{
    m_core->GetMemoryManager()->RemoveMemoryPointer(f_file, ElementType::FileElement);
    delete f_file;
    return true;
}

void ROC::ElementManager::DestroyByPointer(void *f_pointer, unsigned char f_type)
{
    //Called only at the end of work
    switch(f_type)
    {
        case ElementType::ClientElement:
            delete reinterpret_cast<Client*>(f_pointer);
            break;
        case ElementType::FileElement:
            delete reinterpret_cast<File*>(f_pointer);
            break;
    }
}
