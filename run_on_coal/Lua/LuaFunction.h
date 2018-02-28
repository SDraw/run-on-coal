#pragma once

// Warning: Not thread-safe

namespace ROC
{

class LuaFunction
{
    lua_State *m_vm;
    void *m_ptr;
    int m_ref;

    static int RetrieveGlobalReference(lua_State *f_vm, void *f_ptr);
    static void RemoveGlobalReference(lua_State *f_vm, void *f_ptr, int f_ref);
public:
    LuaFunction();
    LuaFunction(const LuaFunction &f_func);
    ~LuaFunction();

    LuaFunction& operator=(const LuaFunction& f_func);
    bool operator==( const LuaFunction& f_func ) const;

    inline lua_State* GetVM() const { return m_vm; }
    inline bool IsValid() const { return ((m_vm != nullptr) && (m_ptr != nullptr) && (m_ref != 0)); }
protected:
    void Retrieve(lua_State *f_vm, void *f_ptr);

    inline int GetReference() const { return m_ref; }

    // Global map for Lua functions references
    struct LuaFunctionData
    {
        int m_ref = 0;
        unsigned int m_refCount = 0U;
    };
    typedef std::unordered_map<void*, LuaFunctionData> LuaFunctionMap;
    static std::map<lua_State*, LuaFunctionMap*> ms_vmFuncMap;

    friend class LuaManager;
    friend class ArgReader;
};

}
