#pragma once

namespace ROC
{

class Core;
class Client;
class Element;
class File;

class ElementManager final
{
    Core *m_core;

    std::unordered_set<void*> m_elementSet;
    std::unordered_set<void*>::iterator m_elementSetEnd;

    ElementManager(const ElementManager &that);
    ElementManager& operator=(const ElementManager &that);

    void AddElementToSet(void *f_ptr);
    void RemoveElementFromSet(void *f_ptr);
public:
    File* CreateFile_(const std::string &f_path);
    File* OpenFile(const std::string &f_path, bool f_ro);
    void CloseFile(File *f_file);

    bool IsValidElement(void *f_ptr);
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
