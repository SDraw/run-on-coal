#pragma once

namespace ROC
{

enum ElementType : unsigned char { ClientElement = 0U, FileElement };

class Client;
class File;
class ElementManager
{
    Core *m_core;
public:
    File* CreateFile_(std::string &f_path);
    File* OpenFile(std::string &f_path, bool f_ro);
    bool DestroyFile(File *f_file);
protected:
    explicit ElementManager(Core *f_core);
    ~ElementManager();

    Client* CreateClient();
    void DestroyClient(Client *f_client);

    void DestroyByPointer(void* f_pointer, unsigned char f_type);

    friend Core;
    friend class MemoryManager;
    friend class NetworkManager;
};

}
