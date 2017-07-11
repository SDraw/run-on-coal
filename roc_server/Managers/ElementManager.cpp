#include "stdafx.h"

#include "Managers/ElementManager.h"
#include "Core/Core.h"
#include "Elements/Client.h"
#include "Elements/File.h"

#include "Managers/LogManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/NetworkManager.h"
#include "Utils/PathUtils.h"

ROC::ElementManager::ElementManager(Core *f_core)
{
    m_core = f_core;
}
ROC::ElementManager::~ElementManager()
{
}

ROC::Client* ROC::ElementManager::CreateClient(const RakNet::SystemAddress &f_address)
{
    Client *l_client = new Client(f_address);
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

    std::string l_path(f_path), l_work;
    m_core->GetWorkingDirectory(l_work);
    PathUtils::EscapePath(l_path);
    l_path.insert(0U, l_work);

    if(l_file->Create(l_path, f_path)) m_core->GetMemoryManager()->AddMemoryPointer(l_file);
    else
    {
        delete l_file;
        l_file = nullptr;
    }
    return l_file;
}
ROC::File* ROC::ElementManager::OpenFile(const std::string &f_path, bool f_ro)
{
    File *l_file = new File();

    std::string l_path(f_path), l_work;
    m_core->GetWorkingDirectory(l_work);
    PathUtils::EscapePath(l_path);
    l_path.insert(0U, l_work);

    if(l_file->Open(l_path, f_path, f_ro)) m_core->GetMemoryManager()->AddMemoryPointer(l_file);
    else
    {
        delete l_file;
        l_file = nullptr;
    }
    return l_file;
}

bool ROC::ElementManager::DestroyElement(Element *f_element)
{
    bool l_result = false;
    if(m_core->GetMemoryManager()->IsValidMemoryPointer(f_element))
    {
        switch(f_element->GetElementType())
        {
            case ElementType::FileElement:
            {
                m_core->GetMemoryManager()->RemoveMemoryPointer(f_element);
                delete f_element;
                l_result = true;
            } break;
        }
    }
    return l_result;
}

void ROC::ElementManager::DestroyElementByPointer(void *f_element)
{
    //Called only at the end of work
    delete reinterpret_cast<Element*>(f_element);
}
