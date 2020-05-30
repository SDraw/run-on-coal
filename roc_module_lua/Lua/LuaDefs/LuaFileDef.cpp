#include "stdafx.h"

#include "Lua/LuaDefs/LuaFileDef.h"

#include "Lua/ArgReader.h"
#include "Lua/LuaDefs/LuaElementDef.h"
#include "Module/LuaModule.h"
#include "Utils/EnumUtils.h"
#include "Utils/LuaUtils.h"

const std::vector<std::string> g_FileManageTypes
{
    "create", "open"
};

enum FileManageMode : size_t
{
    FMM_Create = 0U,
    FMM_Open
};

void LuaFileDef::Init(lua_State *f_vm)
{
    LuaUtils::AddClass(f_vm, "File", CreateOpen);
    LuaUtils::AddClassMethod(f_vm, "read", Read);
    LuaUtils::AddClassMethod(f_vm, "write", Write);
    LuaUtils::AddClassMethod(f_vm, "getSize", GetSize);
    LuaUtils::AddClassMethod(f_vm, "setPosition", SetPosition);
    LuaUtils::AddClassMethod(f_vm, "getPosition", GetPosition);
    LuaUtils::AddClassMethod(f_vm, "flush", Flush);
    LuaUtils::AddClassMethod(f_vm, "getPath", GetPath);
    LuaUtils::AddClassMethod(f_vm, "isEOF", IsEOF);
    LuaElementDef::AddHierarchyMethods(f_vm);
    LuaUtils::AddClassFinish(f_vm);

    lua_register(f_vm, "fileDelete", Delete);
    lua_register(f_vm, "fileRename", Rename);
}

int LuaFileDef::CreateOpen(lua_State *f_vm)
{
    // element File(str path, str mode)
    std::string l_path;
    std::string l_manage;
    bool l_readOnly = true;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_path);
    argStream.ReadText(l_manage);
    argStream.ReadNextBoolean(l_readOnly);
    if(!argStream.HasErrors() && !l_path.empty() && !l_manage.empty())
    {
        ROC::IFile *l_file = nullptr;
        switch(EnumUtils::ReadEnumVector(l_manage, g_FileManageTypes))
        {
            case FileManageMode::FMM_Create:
                l_file = LuaModule::GetModule()->GetEngineCore()->GetIElementManager()->CreateIFile(l_path);
                break;
            case FileManageMode::FMM_Open:
                l_file = LuaModule::GetModule()->GetEngineCore()->GetIElementManager()->OpenIFile(l_path, l_readOnly);
                break;
        }
        l_file ? argStream.PushElement(l_file) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaFileDef::Read(lua_State *f_vm)
{
    // str int File:read(int length)
    ROC::IFile *l_file;
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

int LuaFileDef::Write(lua_State *f_vm)
{
    // int File:write(str data)
    ROC::IFile *l_file;
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

int LuaFileDef::GetSize(lua_State *f_vm)
{
    // int File:getSize()
    ROC::IFile *l_file;
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

int LuaFileDef::SetPosition(lua_State *f_vm)
{
    // bool File:setPosition(int position)
    ROC::IFile *l_file;
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

int LuaFileDef::GetPosition(lua_State *f_vm)
{
    // int File:getPosition()
    ROC::IFile *l_file;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_file);
    !argStream.HasErrors() ? argStream.PushInteger(l_file->GetPosition()) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaFileDef::Flush(lua_State *f_vm)
{
    // bool File:flush()
    ROC::IFile *l_file;
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

int LuaFileDef::GetPath(lua_State *f_vm)
{
    // str File:getPath()
    ROC::IFile *l_file;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_file);
    !argStream.HasErrors() ? argStream.PushText(l_file->GetPath()) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaFileDef::IsEOF(lua_State *f_vm)
{
    // bool File:isEOF()
    ROC::IFile *l_file;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_file);
    argStream.PushBoolean(!argStream.HasErrors() ? l_file->IsEOF() : false);
    return argStream.GetReturnValue();
}

int LuaFileDef::Delete(lua_State *f_vm)
{
    // bool fileDelete(str path)
    std::string l_path;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_path);
    if(!argStream.HasErrors() && !l_path.empty())
    {
        bool l_result = !std::remove(l_path.c_str());
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaFileDef::Rename(lua_State *f_vm)
{
    // bool fileRename(str oldName, str newName)
    std::string l_old, l_new;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_old);
    argStream.ReadText(l_new);
    if(!argStream.HasErrors() && !l_old.empty() && !l_new.empty())
    {
        bool l_result = !std::rename(l_old.c_str(), l_new.c_str());
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
