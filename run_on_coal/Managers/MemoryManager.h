#pragma once

namespace ROC
{
class Core;
class MemoryManager
{
    Core *m_core;

    std::map<void*, unsigned char> m_memoryMap;
public:
    bool CheckMemoryPointer(void *f_pointer, unsigned char f_type);
    int GetMemoryPointerType(void *f_pointer);
    inline bool IsValidMemoryPointer(void *f_pointer) { return (m_memoryMap.find(f_pointer) != m_memoryMap.end()); }
protected:
    explicit MemoryManager(Core *f_core);
    ~MemoryManager();

    inline void AddMemoryPointer(void *f_pointer, unsigned char f_type) { m_memoryMap.insert(std::pair<void*, unsigned int>(f_pointer, f_type)); }
    void RemoveMemoryPointer(void *f_pointer, unsigned char f_type);
    friend Core;
    friend class ElementManager;
};
}
