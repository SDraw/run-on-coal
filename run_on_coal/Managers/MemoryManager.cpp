#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/MemoryManager.h"

ROC::MemoryManager::MemoryManager(Core *f_core)
{
    m_core = f_core;
}
ROC::MemoryManager::~MemoryManager()
{
    ElementManager *l_elementManager = m_core->GetElementManager();
    for(auto iter : m_memoryMap) l_elementManager->DestroyByPointer(iter.first, iter.second);
}

bool ROC::MemoryManager::CheckMemoryPointer(void *f_pointer, unsigned char f_type)
{
    auto l_checkIterator = m_memoryMap.find(f_pointer);
    if(l_checkIterator == m_memoryMap.end()) return false;
    if(l_checkIterator->second != f_type) return false;
    return true;
}

int ROC::MemoryManager::GetMemoryPointerType(void *f_pointer)
{
    auto iter = m_memoryMap.find(f_pointer);
    if(iter == m_memoryMap.end()) return -1;
    return iter->second;
}

void ROC::MemoryManager::RemoveMemoryPointer(void *f_pointer, unsigned char f_type)
{
    auto l_checkIterator = m_memoryMap.find(f_pointer);
    if(l_checkIterator != m_memoryMap.end())
    {
        if(l_checkIterator->second == f_type) m_memoryMap.erase(l_checkIterator);
    }
}