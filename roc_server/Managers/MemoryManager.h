#pragma once

namespace ROC
{

class MemoryManager final
{
    std::set<void*> m_memorySet;
    std::set<void*>::iterator m_memorySetEnd;
public:
    bool IsValidMemoryPointer(void *f_ptr);
protected:
    MemoryManager();
    ~MemoryManager();

    void AddMemoryPointer(void *f_ptr);
    void RemoveMemoryPointer(void *f_ptr);
    friend class Core;
    friend class ElementManager;
};

}
