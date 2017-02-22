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

bool ROC::MemoryManager::IsValidMemoryPointer(void *f_pointer)
{
    return (m_memorySet.find(f_pointer) != m_memorySetEnd);
}

void ROC::MemoryManager::AddMemoryPointer(void *f_pointer)
{
    m_memorySet.insert(f_pointer);
    m_memorySetEnd = m_memorySet.end();
}
void ROC::MemoryManager::RemoveMemoryPointer(void *f_pointer)
{
    auto l_checkIterator = m_memorySet.find(f_pointer);
    if(l_checkIterator != m_memorySetEnd)
    {
        m_memorySet.erase(l_checkIterator);
        m_memorySetEnd = m_memorySet.end();
    }
}
