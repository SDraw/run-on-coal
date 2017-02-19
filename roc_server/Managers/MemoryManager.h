#pragma once

namespace ROC
{

class MemoryManager
{
    std::unordered_map<void*, unsigned char> m_memoryMap;
    std::unordered_map<void*, unsigned char>::iterator m_memoryMapEnd;
public:
    bool CheckMemoryPointer(void *f_pointer, unsigned char f_type);
    int GetMemoryPointerType(void *f_pointer);
    inline bool IsValidMemoryPointer(void *f_pointer) { return (m_memoryMap.find(f_pointer) != m_memoryMapEnd); }
protected:
    MemoryManager();
    ~MemoryManager();

    void AddMemoryPointer(void *f_pointer, unsigned char f_type);
    void RemoveMemoryPointer(void *f_pointer, unsigned char f_type);
    friend class Core;
    friend class ElementManager;
};

}
