#include "stdafx.h"

#include "Lua/LuaDefs/LuaUtilsDef.h"

#include "Lua/LuaArgReader.h"
#include "Module/LuaModule.h"

void LuaUtilsDef::Init(lua_State *f_vm)
{
    lua_register(f_vm, "dofile", DisabledFunction);
    lua_register(f_vm, "loadfile", DisabledFunction);
    lua_register(f_vm, "logPrint", LogPrint);
    lua_register(f_vm, "getTickCount", GetTick);
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
        LuaModule::GetModule()->GetEngineCore()->GetILogManager()->Log(l_text);
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaUtilsDef::GetTick(lua_State *f_vm)
{
    // int getTickCount()
    LuaArgReader l_argStream(f_vm);
    lua_Integer l_tick = 0;
    l_tick = static_cast<lua_Integer>(GetTickCount());
    l_argStream.PushInteger(l_tick);
    return l_argStream.GetReturnValue();
}

int LuaUtilsDef::Base64Encode(lua_State *f_vm)
{
    // std base64Encode(str data)
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
