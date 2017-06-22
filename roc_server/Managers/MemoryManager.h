#pragma once

namespace ROC
{

class MemoryManager final
{
    std::set<void*> m_memorySet;
    std::set<void*>::iterator m_memorySetEnd;
public:
    bool IsValidMemoryPointer(void *f_pointer);
protected:
    MemoryManager();
    ~MemoryManager();

    void AddMemoryPointer(void *f_pointer);
    void RemoveMemoryPointer(void *f_pointer);
    friend class Core;
    friend class ElementManager;
};

}
