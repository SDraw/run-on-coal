#include "stdafx.h"

#include "Lua/LuaFunction.h"

std::map<lua_State*, LuaFunction::LuaFunctionMap*> LuaFunction::ms_vmFuncMap;

int LuaFunction::RetrieveGlobalReference(lua_State *f_vm, void *f_ptr)
{
    int l_ref = 0;
    LuaFunctionMap *l_funcMap = nullptr;

    auto l_funcMapIter = ms_vmFuncMap.find(f_vm);
    if(l_funcMapIter != ms_vmFuncMap.end()) l_funcMap = l_funcMapIter->second;
    else
    {
        l_funcMap = new LuaFunctionMap();
        ms_vmFuncMap.insert(std::make_pair(f_vm, l_funcMap));
    }

    auto l_funcDataMapIter = l_funcMap->find(f_ptr);
    if(l_funcDataMapIter != l_funcMap->end())
    {
        LuaFunctionData &l_funcData = l_funcDataMapIter->second;
        l_funcData.m_refCount++;

        l_ref = l_funcData.m_ref;
    }
    else
    {
        LuaFunctionData l_funcData;
        l_funcData.m_ref = luaL_ref(f_vm, LUA_REGISTRYINDEX);
        lua_rawgeti(f_vm, LUA_REGISTRYINDEX, l_funcData.m_ref); // Restore function in stack
        l_funcData.m_refCount++;

        l_funcMap->emplace(f_ptr, l_funcData);
        l_ref = l_funcData.m_ref;
    }

    return l_ref;
}

void LuaFunction::RemoveGlobalReference(lua_State *f_vm, void *f_ptr, int f_ref)
{
    auto l_funcMapIter = ms_vmFuncMap.find(f_vm);
    if(l_funcMapIter != ms_vmFuncMap.end())
    {
        LuaFunctionMap *l_funcMap = l_funcMapIter->second;
        auto l_funcDataMapIter = l_funcMap->find(f_ptr);
        if(l_funcDataMapIter != l_funcMap->end())
        {
            LuaFunctionData &l_funcData = l_funcDataMapIter->second;
            if(l_funcData.m_ref == f_ref)
            {
                l_funcData.m_refCount--;
                if(l_funcData.m_refCount == 0U)
                {
                    luaL_unref(f_vm, LUA_REGISTRYINDEX, l_funcData.m_ref);
                    l_funcMap->erase(l_funcDataMapIter);
                }
            }
        }
    }
}

LuaFunction::LuaFunction()
{
    m_vm = nullptr;
    m_ptr = nullptr;
    m_ref = 0;
}
LuaFunction::LuaFunction(const LuaFunction &f_func)
{
    if(f_func.IsValid())
    {
        m_vm = f_func.m_vm;
        m_ptr = f_func.m_ptr;
        m_ref = RetrieveGlobalReference(m_vm, m_ptr);
    }
    else
    {
        m_vm = nullptr;
        m_ptr = nullptr;
        m_ref = 0;
    }
}

LuaFunction::~LuaFunction()
{
    if(IsValid()) RemoveGlobalReference(m_vm, m_ptr, m_ref);
}

LuaFunction& LuaFunction::operator=(const LuaFunction& f_func)
{
    if(f_func.IsValid())
    {
        m_vm = f_func.m_vm;
        m_ptr = f_func.m_ptr;
        m_ref = RetrieveGlobalReference(m_vm, m_ptr);
    }
    return *this;
}
bool LuaFunction::operator==(const LuaFunction& f_func) const
{
    return ((m_vm == f_func.m_vm) && (m_ref == f_func.m_ref));
}

void LuaFunction::Retrieve(lua_State *f_vm, void *f_ptr)
{
    if(!IsValid())
    {
        m_vm = f_vm;
        m_ptr = f_ptr;
        m_ref = RetrieveGlobalReference(m_vm, m_ptr);
    }
}
