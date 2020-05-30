#include "stdafx.h"

#include "Managers/ElementManager.h"
#include "Core/Core.h"
#include "Elements/Client.h"
#include "Elements/File.h"

#include "Managers/LogManager.h"
#include "Managers/NetworkManager.h"

ROC::ElementManager::ElementManager(Core *f_core)
{
    m_core = f_core;
    m_elementsEnd = m_elements.end();
    m_interfacesEnd = m_interfaces.end();
}

ROC::ElementManager::~ElementManager()
{
    for(auto iter : m_elements) delete reinterpret_cast<Element*>(iter);
}

void ROC::ElementManager::AddElementToSet(Element *f_ptr)
{
    m_elements.insert(f_ptr);
    m_interfaces.insert(f_ptr);
    m_elementsEnd = m_elements.end();
    m_interfacesEnd = m_interfaces.end();
}

void ROC::ElementManager::RemoveElementFromSet(Element *f_ptr)
{
    m_elements.erase(f_ptr);
    m_interfaces.erase(f_ptr);
    m_elementsEnd = m_elements.end();
    m_interfacesEnd = m_interfaces.end();
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

bool ROC::ElementManager::IsValidElement(Element *f_ptr)
{
    auto iter = m_elements.find(f_ptr);
    return (iter != m_elementsEnd);
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

// ROC::IElementManager
ROC::IFile* ROC::ElementManager::CreateIFile(const std::string &f_path)
{
    return CreateFile_(f_path);
}

ROC::IFile* ROC::ElementManager::OpenIFile(const std::string &f_path, bool f_ro)
{
    return OpenFile(f_path, f_ro);
}

bool ROC::ElementManager::IsValidIElement(IElement *f_ptr)
{
    auto l_checkIterator = m_interfaces.find(f_ptr);
    return (l_checkIterator != m_interfacesEnd);
}

bool ROC::ElementManager::DestroyIElement(IElement *f_element)
{
    bool l_result = false;
    auto l_checkIterator = m_interfaces.find(f_element);
    if(l_checkIterator != m_interfacesEnd) l_result = DestroyElement(dynamic_cast<Element*>(f_element));
    return l_result;
}
