#include "stdafx.h"

#include "Lua/LuaDefs/LuaUtilsDef.h"

#include "Module/LuaModule.h"
#include "Module/TaskHandler.h"
#include "Lua/ArgReader.h"

void LuaUtilsDef::Init(lua_State *f_vm)
{
    lua_register(f_vm, "dofile", DisabledFunction);
    lua_register(f_vm, "loadfile", DisabledFunction);
    lua_register(f_vm, "logPrint", LogPrint);
    lua_register(f_vm, "getTickCount", GetTick);
    lua_register(f_vm, "getTime", GetTime);
    lua_register(f_vm, "isTask", IsTask);
    lua_register(f_vm, "base64Encode", Base64Encode);
    lua_register(f_vm, "base64Decode", Base64Decode);
}

int LuaUtilsDef::DisabledFunction(lua_State *f_vm)
{
    lua_pushboolean(f_vm, 0);
    return 1;
}

int LuaUtilsDef::LogPrint(lua_State *f_vm)
{
    // bool logPrint(str text)
    std::string l_text;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_text);
    if(!argStream.HasErrors())
    {
        LuaModule::GetModule()->GetEngineCore()->GetLogManager()->Log(l_text);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaUtilsDef::GetTick(lua_State *f_vm)
{
    // int getTickCount()
    ArgReader argStream(f_vm);
    argStream.PushInteger(GetTickCount64());
    return argStream.GetReturnValue();
}
int LuaUtilsDef::GetTime(lua_State *f_vm)
{
    // float getTime()
    ArgReader argStream(f_vm);
    argStream.PushNumber(LuaModule::GetModule()->GetEngineCore()->GetSfmlManager()->GetTime());
    return argStream.GetReturnValue();
}

int LuaUtilsDef::IsTask(lua_State *f_vm)
{
    void *l_task = nullptr;
    ArgReader argStream(f_vm);
    argStream.ReadPointer(l_task);
    bool l_result = LuaModule::GetModule()->GetTaskHandler()->IsTask(l_task);
    argStream.PushBoolean(l_result);
    return argStream.GetReturnValue();
}

int LuaUtilsDef::Base64Encode(lua_State *f_vm)
{
    // str base64Encode(str data)
    std::string l_data;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_data);
    if(!argStream.HasErrors() && !l_data.empty())
    {
        std::string l_encoded;
        Base64::Encode(l_data, &l_encoded) ? argStream.PushText(l_encoded) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaUtilsDef::Base64Decode(lua_State *f_vm)
{
    // str base64Decode(str data)
    std::string l_data;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_data);
    if(!argStream.HasErrors() && !l_data.empty())
    {
        std::string l_decoded;
        Base64::Decode(l_data, &l_decoded) ? argStream.PushText(l_decoded) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
