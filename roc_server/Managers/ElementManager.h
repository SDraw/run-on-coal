#pragma once

namespace ROC
{

enum ElementType : unsigned char { ClientElement = 0U };

class Client;
class ElementManager
{
    Core *m_core;
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
