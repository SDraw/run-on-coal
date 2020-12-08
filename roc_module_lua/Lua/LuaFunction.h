#pragma once

// Warning: Not thread-safe, only for single VM

class LuaFunction
{
    lua_State *m_vm;
    void *m_ptr;
    int m_ref;

    // Global map for Lua functions references
    struct LuaFunctionData
    {
        int m_ref = 0;
        unsigned int m_refCount = 0U;
    };
    static std::unordered_map<void*, LuaFunctionData> ms_funcMap;

    static int RetrieveGlobalReference(lua_State *f_vm, void *f_ptr);
    static void RemoveGlobalReference(lua_State *f_vm, void *f_ptr, int f_ref);
public:
    LuaFunction();
    LuaFunction(const LuaFunction &f_func);
    ~LuaFunction();

    LuaFunction& operator=(const LuaFunction& f_func);
    bool operator==(const LuaFunction& f_func) const;

    lua_State* GetVM() const;
    bool IsValid() const;
protected:
    void Retrieve(lua_State *f_vm, void *f_ptr);

    int GetReference() const;

    friend class LuaArgReader;
    friend class LuaVM;
};
