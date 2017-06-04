#pragma once

namespace ROC
{

class MemoryManager
{
    std::unordered_set<void*> m_memorySet;
    std::unordered_set<void*>::iterator m_memorySetEnd;
public:
    bool IsValidMemoryPointer(void *f_pointer);
protected:
    MemoryManager();
    ~MemoryManager();

    void AddMemoryPointer(void *f_pointer);
    void RemoveMemoryPointer(void *f_pointer);

    friend class Core;
    friend class AsyncManager;
    friend class ElementManager;
};

}
