#pragma once
#include "Interfaces/IElementManager.h"
#include "Elements/Element.h"
#include "Elements/File.h"

namespace ROC
{

class Core;
class Client;

class ElementManager final : public IElementManager
{
    Core *m_core;

    std::unordered_set<void*> m_elementSet;
    std::unordered_set<void*>::iterator m_elementSetEnd;

    ElementManager(const ElementManager &that) = delete;
    ElementManager& operator=(const ElementManager &that) = delete;

    void AddElementToSet(void *f_ptr);
    void RemoveElementFromSet(void *f_ptr);

    // Interfaces reroute
    bool IsValidElement(IElement *f_ptr);
    bool DestroyElement(IElement *f_element);
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
