#include "stdafx.h"

#include "Lua/LuaDefs/LuaUtilsDef.h"

#include "Core/Core.h"
#include "Core/TaskHandler.h"
#include "Lua/LuaArgReader.h"

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
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadText(l_text);
    if(!l_argStream.HasErrors())
    {
        Core::GetInstance()->GetEngineICore()->GetILogManager()->Log(l_text);
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaUtilsDef::GetTick(lua_State *f_vm)
{
    // int getTickCount()
    LuaArgReader l_argStream(f_vm);
    l_argStream.PushInteger(GetTickCount64());
    return l_argStream.GetReturnValue();
}

int LuaUtilsDef::GetTime(lua_State *f_vm)
{
    // float getTime()
    LuaArgReader l_argStream(f_vm);
    l_argStream.PushNumber(Core::GetInstance()->GetEngineICore()->GetISfmlManager()->GetTime());
    return l_argStream.GetReturnValue();
}

int LuaUtilsDef::IsTask(lua_State *f_vm)
{
    // bool isTask(userdata task)
    void *l_task = nullptr;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadPointer(l_task);
    bool l_result = Core::GetInstance()->GetTaskHandler()->IsTask(l_task);
    l_argStream.PushBoolean(l_result);
    return l_argStream.GetReturnValue();
}

int LuaUtilsDef::Base64Encode(lua_State *f_vm)
{
    // str base64Encode(str data)
    std::string l_data;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadText(l_data);
    if(!l_argStream.HasErrors() && !l_data.empty())
    {
        std::string l_encoded;
        Base64::Encode(l_data, &l_encoded) ? l_argStream.PushText(l_encoded) : l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaUtilsDef::Base64Decode(lua_State *f_vm)
{
    // str base64Decode(str data)
    std::string l_data;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadText(l_data);
    if(!l_argStream.HasErrors() && !l_data.empty())
    {
        std::string l_decoded;
        Base64::Decode(l_data, &l_decoded) ? l_argStream.PushText(l_decoded) : l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}
