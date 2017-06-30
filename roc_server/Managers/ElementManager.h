#pragma once

namespace ROC
{

enum ElementType : unsigned char { ClientElement = 0U, FileElement };

class Core;
class Client;
class Element;
class File;
class ElementManager final
{
    Core *m_core;
public:
    File* CreateFile_(const std::string &f_path);
    File* OpenFile(const std::string &f_path, bool f_ro);
    void CloseFile(File *f_file);

    bool DestroyElement(Element *f_element);
protected:
    explicit ElementManager(Core *f_core);
    ~ElementManager();

    Client* CreateClient(const RakNet::SystemAddress &f_address);
    void DestroyClient(Client *f_client);

    static void DestroyElementByPointer(void* f_ptr);

    friend class Core;
    friend class MemoryManager;
    friend class NetworkManager;
};

}
