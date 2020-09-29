#include "stdafx.h"

#include "Lua/LuaDefs/LuaFileDef.h"

#include "Lua/LuaArgReader.h"
#include "Lua/LuaDefs/LuaElementDef.h"
#include "Module/LuaModule.h"
#include "Utils/EnumUtils.h"
#include "Utils/LuaUtils.h"

const std::vector<std::string> g_fileManageTypes
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
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadText(l_path);
    l_argStream.ReadText(l_manage);
    l_argStream.ReadNextBoolean(l_readOnly);
    if(!l_argStream.HasErrors() && !l_path.empty() && !l_manage.empty())
    {
        ROC::IFile *l_file = nullptr;
        switch(EnumUtils::ReadEnumVector(l_manage, g_fileManageTypes))
        {
            case FileManageMode::FMM_Create:
                l_file = LuaModule::GetModule()->GetEngineCore()->GetIElementManager()->CreateIFile(l_path);
                break;
            case FileManageMode::FMM_Open:
                l_file = LuaModule::GetModule()->GetEngineCore()->GetIElementManager()->OpenIFile(l_path, l_readOnly);
                break;
        }
        l_file ? l_argStream.PushElement(l_file) : l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaFileDef::Read(lua_State *f_vm)
{
    // str int File:read(int length)
    ROC::IFile *l_file;
    lua_Integer l_length = 0;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_file);
    l_argStream.ReadInteger(l_length);
    if(!l_argStream.HasErrors() && l_length > 0)
    {
        std::string l_data;
        size_t l_read = l_file->Read(l_data, static_cast<size_t>(l_length));
        if(l_read > 0U)
        {
            l_argStream.PushText(l_data);
            l_argStream.PushInteger(l_read);
        }
        else l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaFileDef::Write(lua_State *f_vm)
{
    // int File:write(str data)
    ROC::IFile *l_file;
    std::string l_data;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_file);
    l_argStream.ReadText(l_data);
    if(!l_argStream.HasErrors() && !l_data.empty())
    {
        size_t l_written = l_file->Write(l_data);
        (l_written > 0U) ? l_argStream.PushInteger(l_written) : l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaFileDef::GetSize(lua_State *f_vm)
{
    // int File:getSize()
    ROC::IFile *l_file;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_file);
    if(!l_argStream.HasErrors())
    {
        size_t l_size = l_file->GetSize();
        l_argStream.PushInteger(l_size);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaFileDef::SetPosition(lua_State *f_vm)
{
    // bool File:setPosition(int position)
    ROC::IFile *l_file;
    lua_Integer l_pos;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_file);
    l_argStream.ReadInteger(l_pos);
    if(!l_argStream.HasErrors() && l_pos >= 0)
    {
        bool l_result = l_file->SetPosition(static_cast<size_t>(l_pos));
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaFileDef::GetPosition(lua_State *f_vm)
{
    // int File:getPosition()
    ROC::IFile *l_file;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_file);
    !l_argStream.HasErrors() ? l_argStream.PushInteger(l_file->GetPosition()) : l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaFileDef::Flush(lua_State *f_vm)
{
    // bool File:flush()
    ROC::IFile *l_file;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_file);
    if(!l_argStream.HasErrors())
    {
        l_file->Flush();
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaFileDef::GetPath(lua_State *f_vm)
{
    // str File:getPath()
    ROC::IFile *l_file;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_file);
    !l_argStream.HasErrors() ? l_argStream.PushText(l_file->GetPath()) : l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaFileDef::IsEOF(lua_State *f_vm)
{
    // bool File:isEOF()
    ROC::IFile *l_file;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_file);
    l_argStream.PushBoolean(!l_argStream.HasErrors() ? l_file->IsEOF() : false);
    return l_argStream.GetReturnValue();
}

int LuaFileDef::Delete(lua_State *f_vm)
{
    // bool fileDelete(str path)
    std::string l_path;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadText(l_path);
    if(!l_argStream.HasErrors() && !l_path.empty())
    {
        bool l_result = !std::remove(l_path.c_str());
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaFileDef::Rename(lua_State *f_vm)
{
    // bool fileRename(str oldName, str newName)
    std::string l_old, l_new;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadText(l_old);
    l_argStream.ReadText(l_new);
    if(!l_argStream.HasErrors() && !l_old.empty() && !l_new.empty())
    {
        bool l_result = !std::rename(l_old.c_str(), l_new.c_str());
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}
