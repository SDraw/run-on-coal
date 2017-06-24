#include "stdafx.h"
#include "Managers/ElementManager.h"
#include "Managers/MemoryManager.h"

ROC::MemoryManager::MemoryManager()
{
    m_memorySetEnd = m_memorySet.end();
}
ROC::MemoryManager::~MemoryManager()
{
    for(auto iter : m_memorySet) ElementManager::DestroyElementByPointer(iter);
}

bool ROC::MemoryManager::IsValidMemoryPointer(void *f_ptr)
{
    return (m_memorySet.find(f_ptr) != m_memorySetEnd);
}

void ROC::MemoryManager::AddMemoryPointer(void *f_ptr)
{
    m_memorySet.insert(f_ptr);
    m_memorySetEnd = m_memorySet.end();
}
void ROC::MemoryManager::RemoveMemoryPointer(void *f_ptr)
{
    auto l_checkIterator = m_memorySet.find(f_ptr);
    if(l_checkIterator != m_memorySetEnd)
    {
        m_memorySet.erase(l_checkIterator);
        m_memorySetEnd = m_memorySet.end();
    }
}
