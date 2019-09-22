#pragma once
#include "Interfaces/IElementManager.h"

namespace ROC
{

class Core;
class Element;
class Client;
class File;

class ElementManager final : public IElementManager
{
    Core *m_core;

    std::unordered_set<Element*> m_elements;
    std::unordered_set<IElement*> m_interfaces;
    std::unordered_set<Element*>::iterator m_elementsEnd;
    std::unordered_set<IElement*>::iterator m_interfacesEnd;

    ElementManager(const ElementManager &that) = delete;
    ElementManager& operator=(const ElementManager &that) = delete;

    void AddElementToSet(Element *f_ptr);
    void RemoveElementFromSet(Element *f_ptr);

    // ROC::IElementManager
    IFile* CreateIFile(const std::string &f_path);
    IFile* OpenIFile(const std::string &f_path, bool f_ro);
    bool IsValidIElement(IElement *f_ptr);
    bool DestroyIElement(IElement *f_element);
public:
    File* CreateFile_(const std::string &f_path);
    File* OpenFile(const std::string &f_path, bool f_ro);

    bool IsValidElement(Element *f_ptr);
    bool DestroyElement(Element *f_element);
protected:
    explicit ElementManager(Core *f_core);
    ~ElementManager();

    Client* CreateClient(const RakNet::SystemAddress &f_address);
    void DestroyClient(Client *f_client);

    friend class Core;
    friend class NetworkManager;
};

}
