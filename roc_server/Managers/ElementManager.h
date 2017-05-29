#pragma once

namespace ROC
{

enum ElementType : unsigned char { ClientElement = 0U, FileElement };

class Core;
class Client;
class Element;
class File;
class ElementManager
{
    Core *m_core;
public:
    File* CreateFile_(const std::string &f_path);
    File* OpenFile(const std::string &f_path, bool f_ro);
    void CloseFile(File *f_file);
protected:
    explicit ElementManager(Core *f_core);
    ~ElementManager();

    Client* CreateClient();
    void DestroyClient(Client *f_client);

    static void DestroyElementByPointer(void* f_pointer);

    friend Core;
    friend class MemoryManager;
    friend class NetworkManager;
};

}
