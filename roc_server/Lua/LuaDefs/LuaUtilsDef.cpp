#include "stdafx.h"

#include "Lua/LuaDefs/LuaUtilsDef.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager/LuaManager.h"
#include "Managers/LogManager.h"
#include "Elements/Element.h"
#include "Lua/ArgReader.h"

void ROC::LuaUtilsDef::Init(lua_State *f_vm)
{
    lua_register(f_vm, "dofile", DisabledFunction);
    lua_register(f_vm, "loadfile", DisabledFunction);
    lua_register(f_vm, "logPrint", LogPrint);
    lua_register(f_vm, "getTickCount", GetTick);
    lua_register(f_vm, "base64Encode", Base64Encode);
    lua_register(f_vm, "base64Decode", Base64Decode);
}

int ROC::LuaUtilsDef::DisabledFunction(lua_State *f_vm)
{
    lua_pushboolean(f_vm, 0);
    return 1;
}

int ROC::LuaUtilsDef::LogPrint(lua_State *f_vm)
{
    // bool logPrint(str text)
    std::string l_text;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_text);
    if(!argStream.HasErrors())
    {
        Core::GetCore()->GetLogManager()->Log(l_text);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaUtilsDef::GetTick(lua_State *f_vm)
{
    // int getTickCount()
    ArgReader argStream(f_vm);
    lua_Integer l_tick = 0;
    l_tick = static_cast<lua_Integer>(GetTickCount());
    argStream.PushInteger(l_tick);
    return argStream.GetReturnValue();
}

int ROC::LuaUtilsDef::Base64Encode(lua_State *f_vm)
{
    // std base64Encode(str data)
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
int ROC::LuaUtilsDef::Base64Decode(lua_State *f_vm)
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
