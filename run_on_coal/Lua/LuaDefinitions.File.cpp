#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Scene/File.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.File.h"

namespace ROC
{
namespace Lua
{

int fileCreate(lua_State *f_vm)
{
    std::string l_path;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadText(l_path);
    if(argStream.HasErrors() || !l_path.length())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    File *l_file = LuaManager::m_corePointer->GetElementManager()->CreateFile_(l_path);
    l_file ? lua_pushlightuserdata(f_vm,l_file) : lua_pushboolean(f_vm,0);
    return 1;
}
int fileOpen(lua_State *f_vm)
{
    std::string l_path;
    bool l_ro = true;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadText(l_path);
    argStream.ReadNextBoolean(l_ro);
    if(argStream.HasErrors() || !l_path.length())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    File *l_file = LuaManager::m_corePointer->GetElementManager()->OpenFile(l_path,l_ro);
    l_file ? lua_pushlightuserdata(f_vm,l_file) : lua_pushboolean(f_vm,0);
    return 1;
}
int fileClose(lua_State *f_vm)
{
    File *l_file;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_file,ElementType::FileElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    bool result = LuaManager::m_corePointer->GetElementManager()->DestroyFile(l_file);
    lua_pushboolean(f_vm,result);
    return 1;
}
int fileRead(lua_State *f_vm)
{
    File *l_file;
    LUA_INTEGER l_length = 0;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_file,ElementType::FileElement);
    argStream.ReadInteger(l_length);
    if(argStream.HasErrors() || l_length <= 0U)
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    std::string l_data;
    LUA_INTEGER l_read = l_file->Read(l_data,size_t(l_length));
    if(l_read <= 0)
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    lua_pushstring(f_vm,l_data.c_str());
    lua_pushinteger(f_vm,l_read);
    return 2;
}
int fileWrite(lua_State *f_vm)
{
    File *l_file = NULL;
    std::string l_data;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_file,ElementType::FileElement);
    argStream.ReadText(l_data);
    if(argStream.HasErrors() || !l_data.length())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    LUA_INTEGER l_written = l_file->Write(l_data);
    if(l_written <= 0)
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    lua_pushinteger(f_vm,l_written);
    return 2;
}
int fileGetSize(lua_State *f_vm)
{
    File *l_file = NULL;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_file,ElementType::FileElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    LUA_INTEGER l_size = l_file->GetSize();
    lua_pushinteger(f_vm,l_size);
    return 1;
}
int fileSetPosition(lua_State *f_vm)
{
    File *l_file;
    LUA_INTEGER l_pos;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_file,ElementType::FileElement);
    argStream.ReadInteger(l_pos);
    if(argStream.HasErrors() || l_pos < 0)
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    bool result = l_file->SetPosition(size_t(l_pos));
    lua_pushboolean(f_vm,result);
    return 1;
}
int fileGetPosition(lua_State *f_vm)
{
    File *l_file;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_file,ElementType::FileElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    LUA_INTEGER l_pos = l_file->GetPosition();
    lua_pushinteger(f_vm,l_pos);
    return 1;
}
int fileGetPath(lua_State *f_vm)
{
    File *l_file;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_file,ElementType::FileElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    std::string l_path;
    l_file->GetPath(l_path);
    lua_pushstring(f_vm,l_path.c_str());
    return 1;
}
int fileIsEOF(lua_State *f_vm)
{
    File *l_file;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_file,ElementType::FileElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    bool l_eof = l_file->IsEOF();
    lua_pushboolean(f_vm,l_eof);
    return 1;
}
int fileDelete(lua_State *f_vm)
{
    std::string l_path;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadText(l_path);
    if(argStream.HasErrors() || !l_path.length())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    bool result = File::Delete(l_path);
    lua_pushboolean(f_vm,result);
    return 1;
}
int fileRename(lua_State *f_vm)
{
    std::string l_old,l_new;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadText(l_old);
    argStream.ReadText(l_new);
    if(argStream.HasErrors() || !l_old.length() || !l_new.length())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    bool result = File::Rename(l_old,l_new);
    lua_pushboolean(f_vm,result);
    return 1;
}

}
}
