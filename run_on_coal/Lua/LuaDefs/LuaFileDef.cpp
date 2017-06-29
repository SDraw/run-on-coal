#include "stdafx.h"

#include "Lua/LuaDefs/LuaFileDef.h"
#include "Lua/LuaDefs/LuaElementDef.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Elements/File.h"
#include "Lua/ArgReader.h"
#include "Utils/Utils.h"

#define ROC_FILE_MANAGE_CREATE 0
#define ROC_FILE_MANAGE_OPEN 1

namespace ROC
{

const std::vector<std::string> g_FileManageTypesTable
{
    "create", "open"
};

}

void ROC::LuaFileDef::Init(lua_State *f_vm)
{
    Utils::Lua::lua_registerClass(f_vm, "File", FileCreateOpen);
    Utils::Lua::lua_registerClassMethod(f_vm, "read", FileRead);
    Utils::Lua::lua_registerClassMethod(f_vm, "write", FileWrite);
    Utils::Lua::lua_registerClassMethod(f_vm, "getSize", FileGetSize);
    Utils::Lua::lua_registerClassMethod(f_vm, "setPosition", FileSetPosition);
    Utils::Lua::lua_registerClassMethod(f_vm, "getPosition", FileGetPosition);
    Utils::Lua::lua_registerClassMethod(f_vm, "flush", FileFlush);
    Utils::Lua::lua_registerClassMethod(f_vm, "getPath", FileGetPath);
    Utils::Lua::lua_registerClassMethod(f_vm, "isEOF", FileIsEOF);
    LuaElementDef::AddHierarchyMethods(f_vm);
    Utils::Lua::lua_registerClassFinish(f_vm);

    lua_register(f_vm, "fileDelete", FileDelete);
    lua_register(f_vm, "fileRename", FileRename);
}

int ROC::LuaFileDef::FileCreateOpen(lua_State *f_vm)
{
    std::string l_path;
    std::string l_manage;
    bool l_readOnly = true;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_path);
    argStream.ReadText(l_manage);
    argStream.ReadNextBoolean(l_readOnly);
    if(!argStream.HasErrors() && !l_path.empty() && !l_manage.empty())
    {

        File *l_file = nullptr;
        switch(Utils::Enum::ReadEnumVector(g_FileManageTypesTable, l_manage))
        {
            case ROC_FILE_MANAGE_CREATE:
                l_file = LuaManager::GetCore()->GetElementManager()->CreateFile_(l_path);
                break;
            case ROC_FILE_MANAGE_OPEN:
                l_file = LuaManager::GetCore()->GetElementManager()->OpenFile(l_path, l_readOnly);
                break;
        }
        l_file ? argStream.PushElement(l_file) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaFileDef::FileRead(lua_State *f_vm)
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
int ROC::LuaFileDef::FileWrite(lua_State *f_vm)
{
    File *l_file;
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
int ROC::LuaFileDef::FileGetSize(lua_State *f_vm)
{
    File *l_file;
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
int ROC::LuaFileDef::FileSetPosition(lua_State *f_vm)
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
int ROC::LuaFileDef::FileGetPosition(lua_State *f_vm)
{
    File *l_file;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_file);
    !argStream.HasErrors() ? argStream.PushInteger(l_file->GetPosition()) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaFileDef::FileFlush(lua_State *f_vm)
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
int ROC::LuaFileDef::FileGetPath(lua_State *f_vm)
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
int ROC::LuaFileDef::FileIsEOF(lua_State *f_vm)
{
    File *l_file;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_file);
    argStream.PushBoolean(!argStream.HasErrors() ? l_file->IsEOF() : false);
    return argStream.GetReturnValue();
}
int ROC::LuaFileDef::FileDelete(lua_State *f_vm)
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
int ROC::LuaFileDef::FileRename(lua_State *f_vm)
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
