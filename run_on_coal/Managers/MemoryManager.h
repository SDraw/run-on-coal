#pragma once

namespace ROC
{

class Core;
class MemoryManager
{
    Core *m_core;

    std::map<void*,unsigned char> m_memoryMap;
public:
    bool CheckMemoryPointer(void *f_pointer, unsigned char f_type);
    int GetMemoryPointerType(void *f_pointer);
    bool IsValidMemoryPointer(void *f_pointer);
protected:
    MemoryManager(Core *f_core);
    ~MemoryManager();
    void AddMemoryPointer(void *f_pointer, unsigned char f_type);
    void RemoveMemoryPointer(void *f_pointer, unsigned char f_type);
    friend Core;
    friend class ElementManager;
};

}
