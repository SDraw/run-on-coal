#include "stdafx.h"

#include "Managers/ElementManager.h"
#include "Core/Core.h"
#include "Elements/Client.h"

#include "Managers/LogManager.h"
#include "Managers/NetworkManager.h"

ROC::ElementManager::ElementManager(Core *f_core)
{
    m_core = f_core;
    m_elementSetEnd = m_elementSet.end();
}
ROC::ElementManager::~ElementManager()
{
    for(auto iter : m_elementSet) delete reinterpret_cast<Element*>(iter);
}

void ROC::ElementManager::AddElementToSet(void *f_ptr)
{
    m_elementSet.insert(f_ptr);
    m_elementSetEnd = m_elementSet.end();
}
void ROC::ElementManager::RemoveElementFromSet(void *f_ptr)
{
    auto l_checkIterator = m_elementSet.find(f_ptr);
    if(l_checkIterator != m_elementSetEnd)
    {
        m_elementSet.erase(l_checkIterator);
        m_elementSetEnd = m_elementSet.end();
    }
}

ROC::Client* ROC::ElementManager::CreateClient(const RakNet::SystemAddress &f_address)
{
    Client *l_client = new Client(f_address);
    AddElementToSet(l_client);
    return l_client;
}
void ROC::ElementManager::DestroyClient(Client *f_client)
{
    if(IsValidElement(f_client))
    {
        RemoveElementFromSet(f_client);
        delete f_client;
    }
}

ROC::File* ROC::ElementManager::CreateFile_(const std::string &f_path)
{
    File *l_file = new File();

    if(l_file->Create(f_path)) AddElementToSet(l_file);
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

    if(l_file->Open(f_path, f_ro)) AddElementToSet(l_file);
    else
    {
        delete l_file;
        l_file = nullptr;
    }
    return l_file;
}

bool ROC::ElementManager::IsValidElement(IElement *f_ptr)
{
    return IsValidElement(dynamic_cast<Element*>(f_ptr));
}
bool ROC::ElementManager::IsValidElement(Element *f_ptr)
{
    auto iter = m_elementSet.find(f_ptr);
    return (iter != m_elementSetEnd);
}

bool ROC::ElementManager::DestroyElement(IElement *f_element)
{
    return DestroyElement(dynamic_cast<Element*>(f_element));
}
bool ROC::ElementManager::DestroyElement(Element *f_element)
{
    bool l_result = false;
    if(IsValidElement(f_element))
    {
        switch(f_element->GetElementType())
        {
            case Element::ET_File:
            {
                RemoveElementFromSet(f_element);
                delete f_element;
                l_result = true;
            } break;
        }
    }
    return l_result;
}
