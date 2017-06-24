#pragma once

namespace ROC
{

class MemoryManager final
{
    std::unordered_set<void*> m_memorySet;
    std::unordered_set<void*>::iterator m_memorySetEnd;
public:
    bool IsValidMemoryPointer(void *f_ptr);
protected:
    MemoryManager();
    ~MemoryManager();

    void AddMemoryPointer(void *f_ptr);
    void RemoveMemoryPointer(void *f_ptr);

    friend class Core;
    friend class AsyncManager;
    friend class ElementManager;
};

}
