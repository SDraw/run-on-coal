#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Elements/File.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.File.h"

namespace ROC
{
namespace Lua
{

int fileCreate(lua_State *f_vm)
{
    std::string l_path;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_path);
    if(!argStream.HasErrors() && !l_path.empty())
    {
        File *l_file = LuaManager::GetCore()->GetElementManager()->CreateFile_(l_path);
        l_file ? argStream.PushPointer(l_file) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int fileOpen(lua_State *f_vm)
{
    std::string l_path;
    bool l_ro = true;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_path);
    argStream.ReadNextBoolean(l_ro);
    if(!argStream.HasErrors() && !l_path.empty())
    {
        File *l_file = LuaManager::GetCore()->GetElementManager()->OpenFile(l_path, l_ro);
        l_file ? argStream.PushPointer(l_file) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int fileClose(lua_State *f_vm)
{
    File *l_file = NULL;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_file);
    if(!argStream.HasErrors())
    {
        LuaManager::GetCore()->GetElementManager()->CloseFile(l_file);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int fileRead(lua_State *f_vm)
{
    File *l_file;
    lua_Integer l_length = 0;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_file);
    argStream.ReadInteger(l_length);
    if(!argStream.HasErrors() && l_length > 0)
    {
        std::string l_data;
        size_t l_read = l_file->Read(l_data, static_cast<size_t>(l_length));
        if(l_read > 0U)
        {
            argStream.PushText(l_data);
            argStream.PushInteger(l_read);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int fileWrite(lua_State *f_vm)
{
    File *l_file = NULL;
    std::string l_data;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_file);
    argStream.ReadText(l_data);
    if(!argStream.HasErrors() && !l_data.empty())
    {
        size_t l_written = l_file->Write(l_data);
        (l_written > 0U) ? argStream.PushInteger(l_written) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int fileGetSize(lua_State *f_vm)
{
    File *l_file = NULL;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_file);
    if(!argStream.HasErrors())
    {
        size_t l_size = l_file->GetSize();
        argStream.PushInteger(l_size);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int fileSetPosition(lua_State *f_vm)
{
    File *l_file;
    lua_Integer l_pos;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_file);
    argStream.ReadInteger(l_pos);
    if(!argStream.HasErrors() && l_pos >= 0)
    {
        bool l_result = l_file->SetPosition(static_cast<size_t>(l_pos));
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int fileGetPosition(lua_State *f_vm)
{
    File *l_file;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_file);
    !argStream.HasErrors() ? argStream.PushInteger(l_file->GetPosition()) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int fileFlush(lua_State *f_vm)
{
    File *l_file;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_file);
    if(!argStream.HasErrors())
    {
        l_file->Flush();
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int fileGetPath(lua_State *f_vm)
{
    File *l_file;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_file);
    if(!argStream.HasErrors())
    {
        std::string l_path;
        l_file->GetPath(l_path);
        argStream.PushText(l_path);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int fileIsEOF(lua_State *f_vm)
{
    File *l_file;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_file);
    argStream.PushBoolean(!argStream.HasErrors() ? l_file->IsEOF() : false);
    return argStream.GetReturnValue();
}
int fileDelete(lua_State *f_vm)
{
    std::string l_path;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_path);
    if(!argStream.HasErrors() && !l_path.empty())
    {
        bool l_result = File::Delete(LuaManager::GetCore(), l_path);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int fileRename(lua_State *f_vm)
{
    std::string l_old, l_new;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_old);
    argStream.ReadText(l_new);
    if(!argStream.HasErrors() && !l_old.empty() && !l_new.empty())
    {
        bool l_result = File::Rename(LuaManager::GetCore(), l_old, l_new);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

}
}
