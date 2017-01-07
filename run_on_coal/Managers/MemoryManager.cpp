#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/MemoryManager.h"

ROC::MemoryManager::MemoryManager(Core *f_core)
{
    m_core = f_core;
    m_memoryMapEnd = m_memoryMap.end();
}
ROC::MemoryManager::~MemoryManager()
{
    ElementManager *l_elementManager = m_core->GetElementManager();
    for(auto iter : m_memoryMap) l_elementManager->DestroyByPointer(iter.first, iter.second);
}

bool ROC::MemoryManager::CheckMemoryPointer(void *f_pointer, unsigned char f_type)
{
    bool l_result = false;
    auto l_checkIterator = m_memoryMap.find(f_pointer);
    if(l_checkIterator != m_memoryMapEnd) l_result = (l_checkIterator->second == f_type);
    return l_result;
}

int ROC::MemoryManager::GetMemoryPointerType(void *f_pointer)
{
    auto iter = m_memoryMap.find(f_pointer);
    return ((iter != m_memoryMapEnd) ? iter->second : -1);
}

void ROC::MemoryManager::AddMemoryPointer(void *f_pointer, unsigned char f_type)
{ 
    m_memoryMap.insert(std::pair<void*, unsigned int>(f_pointer, f_type));
    m_memoryMapEnd = m_memoryMap.end();
}
void ROC::MemoryManager::RemoveMemoryPointer(void *f_pointer, unsigned char f_type)
{
    auto l_checkIterator = m_memoryMap.find(f_pointer);
    if(l_checkIterator != m_memoryMapEnd)
    {
        if(l_checkIterator->second == f_type)
        {
            m_memoryMap.erase(l_checkIterator);
            m_memoryMapEnd = m_memoryMap.end();
        }
    }
}
