#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LogManager.h"
#include "Managers/MemoryManager.h"
#include "Elements/Client.h"

ROC::ElementManager::ElementManager(Core *f_core)
{
    m_core = f_core;
}
ROC::ElementManager::~ElementManager()
{
}

ROC::Client* ROC::ElementManager::CreateClient()
{
    Client *l_client = new Client();
    m_core->GetMemoryManager()->AddMemoryPointer(l_client, ElementType::ClientElement);
    return l_client;
}
void ROC::ElementManager::DestroyClient(Client *f_client)
{
    m_core->GetMemoryManager()->RemoveMemoryPointer(f_client, ElementType::ClientElement);
    delete f_client;
}

void ROC::ElementManager::DestroyByPointer(void *f_pointer, unsigned char f_type)
{
    //Called only at the end of work
    switch(f_type)
    {
        case ElementType::ClientElement:
            break;
    }
}
